// hazptr.hpp                                                         -*-C++-*-
// ----------------------------------------------------------------------------

#ifndef INCLUDED_HAZPTR
#define INCLUDED_HAZPTR

#include <atomic>
#include <memory>
#include <cstddef>

namespace std {
    namespace pmr {
        template <typename T>
        using polymorphic_allocator = std::allocator<T>;
    }
}

// ----------------------------------------------------------------------------

namespace nstd {
    namespace experimental {
        class hazptr_domain;
        hazptr_domain& default_hazptr_domain() noexcept;
        void hazptr_cleanup();
        void hazptr_cleanup(hazptr_domain& domain);
        template <typename T, typename D = std::default_delete<T>>
        class hazptr_obj_base;
    }
    namespace p0566r5 {
        class hazptr_holder;
        hazptr_holder make_hazptr();
        hazptr_holder make_hazptr(experimental::hazptr_domain& domain);
        void swap(hazptr_holder&, hazptr_holder&) noexcept;
    }
}

// ----------------------------------------------------------------------------

class nstd::experimental::hazptr_domain {
public:
    explicit hazptr_domain( std::pmr::polymorphic_allocator<std::byte> poly_alloc = {});
    hazptr_domain(hazptr_domain const&) = delete;
    hazptr_domain(hazptr_domain&&) = delete;
    hazptr_domain& operator=(hazptr_domain const&) = delete;
    hazptr_domain& operator=(hazptr_domain&&) = delete;
    ~hazptr_domain();

private:
    std::pmr::polymorphic_allocator<std::byte> alloc_;
};

// ----------------------------------------------------------------------------

template <typename T, typename D>
class nstd::experimental::hazptr_obj_base {
public:
    void retire(D reclaim = {},
                hazptr_domain& domain = default_hazptr_domain());
    void retire(hazptr_domain& domain);
protected:
    hazptr_obj_base() = default;
};

// ----------------------------------------------------------------------------

class nstd::p0566r5::hazptr_holder {
public:
    hazptr_holder() noexcept;
    hazptr_holder(hazptr_holder&&) noexcept;
    hazptr_holder(hazptr_holder const&) = delete;
    hazptr_holder& operator=(hazptr_holder const&) = delete;
    ~hazptr_holder();
    hazptr_holder& operator=(hazptr_holder&&) noexcept;

    bool empty() const noexcept;
    template <typename T>
    T* protect(std::atomic<T*> const& src) noexcept;
    template <typename T>
    bool try_protect(T*& ptr, std::atomic<T*> const& src) noexcept;
    template <typename T>
    void reset_protected(T const* ptr) noexcept;
    void reset_protected(std::nullptr_t = nullptr) noexcept;
    void swap(hazptr_holder&) noexcept;
};

// ----------------------------------------------------------------------------

template <typename T, typename D>
void nstd::experimental::hazptr_obj_base<T, D>::retire(D              reclaim,
                                                       hazptr_domain& domain) {
    (void)reclaim;
    (void)domain;
}

template <typename T, typename D>
void nstd::experimental::hazptr_obj_base<T, D>::retire(hazptr_domain& domain) {
    return this->retire(D{}, domain);
}

// ----------------------------------------------------------------------------

template <typename T>
bool
nstd::p0566r5::hazptr_holder::try_protect(T*& ptr, std::atomic<T*> const& src) noexcept {
    ptr = src.load();
    return true;
}

template <typename T>
T*
nstd::p0566r5::hazptr_holder::protect(std::atomic<T*> const& src) noexcept {
    return src.load();
}

template <typename T>
void
nstd::p0566r5::hazptr_holder::reset_protected(T const*) noexcept {
}

// ----------------------------------------------------------------------------

#endif