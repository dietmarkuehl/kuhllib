#include "io_ring.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <unistd.h>
#include <linux/io_uring.h> // requires a new enough kernel

#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <fcntl.h>

// This may be gcc specific - use C++ barriers instead?
// These should really use barriers matching what is done in the kernel
#define read_barrier()  __sync_synchronize()
#define write_barrier() __sync_synchronize()

// ----------------------------------------------------------------------------

int io_uring_setup(std::uint32_t size, io_uring_params* params) {
    return syscall(__NR_io_uring_setup, size, params);
}

int io_uring_enter(int fd, unsigned int count, unsigned int min_complete, unsigned int flags, sigset_t *)
{
    //-dk:TODO do something with the sigset?
    return syscall(__NR_io_uring_enter, fd, count, min_complete, flags, NULL, 0);
}

// ----------------------------------------------------------------------------

io_ring::ring::~ring()
{
    if (0 <= this->d_fd) {
        ::close(this->d_fd);
    }
}

io_ring::io_ring(int queue_size)
{
    if (this->setup(queue_size) < 0) {
        throw std::runtime_error("ioring setup failed");
    }
}

int io_ring::setup(int queue_size)
{
    io_uring_params params{};
    this->d_ring.d_fd = io_uring_setup(queue_size, &params);
    if (*this->d_ring < 0) {
        return *this->d_ring;
    }

    std::size_t smap = params.sq_off.array + params.sq_entries * sizeof(std::uint32_t);
    std::size_t sqmap = params.sq_entries * sizeof(io_uring_sqe);
    std::size_t cmap = params.cq_off.cqes + params.cq_entries * sizeof(io_uring_cqe);

    bool  single_mmap(params.features & IORING_FEAT_SINGLE_MMAP);
    if (!this->d_sbase.map(single_mmap? std::max(smap, cmap): smap,
                           *this->d_ring, IORING_OFF_SQ_RING)
        || (!single_mmap && !this->d_cbase.map(cmap, *this->d_ring, IORING_OFF_CQ_RING))
        || !this->d_sqbase.map(sqmap, *this->d_ring, IORING_OFF_SQES)) {
        return -1;
    }
    
    auto const& sbase(this->d_sbase);
    auto const& sq_off(params.sq_off);
    auto&       sring(this->d_sring);
    sring.head = sbase.at_offset<std::uint32_t>(sq_off.head);
    sring.tail = sbase.at_offset<std::uint32_t>(sq_off.tail);
    sring.ring_mask = sbase.at_offset<std::uint32_t>(sq_off.ring_mask);
    sring.array = sbase.at_offset<std::uint32_t>(sq_off.array);

    this->d_sqes = this->d_sqbase.at_offset<io_uring_sqe>(0u);

    auto const& cbase(single_mmap? this->d_sbase: this->d_cbase);
    auto const& cq_off(params.cq_off);
    auto&       cring(this->d_cring);
    cring.head = cbase.at_offset<std::uint32_t>(cq_off.head);
    cring.tail = cbase.at_offset<std::uint32_t>(cq_off.tail);
    cring.ring_mask = cbase.at_offset<std::uint32_t>(cq_off.ring_mask);
    cring.cqes = cbase.at_offset<io_uring_cqe>(cq_off.cqes);

    return *this->d_ring;
}

// ----------------------------------------------------------------------------

struct work
{
    static constexpr std::size_t block_size = 256u;
    explicit work(std::size_t size)
        : size(size) {
        this->iovecs.reserve((size + 1u) / block_size);
        for (std::size_t done(0); done < size; done += block_size) {
            this->iovecs.push_back(iovec{ .iov_base = operator new(block_size),
                                          .iov_len = std::min(block_size, size - done)});
        }
    }
    ~work() {
        for (auto& vec: iovecs) {
            ::operator delete(vec.iov_base);
        }
    }
    std::size_t        size;
    std::vector<iovec> iovecs;
};

unsigned int io_ring::chead()
{
    unsigned int head = *this->d_cring.head;
    read_barrier();
    return head;
}

void process_result(io_ring& ring)
{
    unsigned int head = ring.chead();
    for (; head != *ring.d_cring.tail; ++head)
    {
        auto const& cqe = ring.d_cring.cqes[head & *ring.d_cring.ring_mask];
        if (cqe.res < 0) {
            std::cerr << "ERROR: operation failed\n";
            continue;
        }
        work* w = reinterpret_cast<work*>(cqe.user_data);
        for (auto const& iovec: w->iovecs) {
            std::cout.write(static_cast<char const*>(iovec.iov_base), iovec.iov_len);
        }
    }
    *ring.d_cring.head = head;
    write_barrier();
}

int send_work(io_ring& ring, char const* name)
{
    std::size_t size(std::filesystem::file_size(name));
    int fd(::open(name, O_RDONLY));
    if (fd < 0) {
        std::cerr << "ERROR: failed to pen '" << name << "' for reading\n";
        return -1;
    }
    
    work*         w(new work(size));
    auto          index = *ring.d_sring.tail & *ring.d_sring.ring_mask;
    io_uring_sqe& sqe = ring.d_sqes[index];
    sqe.fd = fd;
    sqe.flags = 0;
    sqe.opcode = IORING_OP_READV;
    sqe.addr = reinterpret_cast<std::uint64_t>(w->iovecs.data());
    sqe.len  = w->iovecs.size();
    sqe.off = 0;
    sqe.user_data = reinterpret_cast<std::uint64_t>(w);
    ring.d_sring.array[index] = index;
    write_barrier();
    ++*ring.d_sring.tail;
    write_barrier();

    sigset_t sig{};
    return io_uring_enter(*ring.d_ring, 1,1, IORING_ENTER_GETEVENTS, &sig);
}

int main(int ac, char *av[])
{
    try {
        if (ac < 2) {
            std::cerr << "usage: " << av[0] << " <filename>\n";
            return EXIT_FAILURE;
        }
        io_ring ring(2);
        for (int i(1); i != ac; ++i) {
            if (send_work(ring, av[i]) < 0) {
                std::cerr << "ERORR: work submission failed\n";
            }
            process_result(ring);
        }
    }
    catch (std::exception const& ex) {
        std::cerr << "ERROR: " << ex.what() << '\n';
    }
    return 0;
}
