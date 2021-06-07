#include <exception>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include "nstd/sender/just.hpp"
#include "nstd/sender/then.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/file/file.hpp"

// ----------------------------------------------------------------------------

namespace EX  = ::nstd::execution;
namespace NET = ::nstd::net;

// ----------------------------------------------------------------------------

namespace
{
    struct receiver
    {
        std::mutex*              bottleneck;
        std::condition_variable* condition;
        bool*                    flag;
        void complete() {
            {
                std::lock_guard kerberos(*this->bottleneck);
                *this->flag = true;
            }
            this->condition->notify_one();
        }

        template <typename... A>
        void set_value(A&&...) noexcept {
            std::cout << "success\n";
            this->complete();
        }
        void set_error(::std::exception_ptr const& ptr) {
            try {
                throw ptr;
            }
            catch (::std::exception const& ex) {
                std::cout << "exptr error: " << ex.what() << "\n";
            }
            this->complete();
        }
        #if 1
        template <typename E>
        void set_error(E&& ptr) noexcept {
            try {
                throw ptr;
            }
            catch (::std::exception const& ex) {
                std::cout << "error: " << ex.what() << "\n";
            }
            catch (...) {
                std::cout << "unknown error\n";
            }
            this->complete();
        }
        #endif
        void set_done() noexcept {
            std::cout << "canceled\n";
            this->complete();
        }
            
    };

    template <typename Sender>
    void sync_wait(Sender&& s)
    {
        std::mutex             bottleneck;
        std::condition_variable condition;
        bool                    flag(false);

        auto operation = EX::connect(std::forward<Sender>(s),
                                     receiver{ &bottleneck, &condition, &flag });
        operation.start();

        std::unique_lock kerberos(bottleneck);
        condition.wait(kerberos, [&flag]{ return flag; });
    }
}

// ----------------------------------------------------------------------------

int main(int ac, char* av[])
{
    try
    {
        ::NET::io_context context;
        ::std::jthread    t0([&]{
            using namespace ::std::chrono_literals;
            //::std::this_thread::sleep_for(200ms);
            context.run_one();
            ::std::cout << "runner thread existing\n" << ::std::flush;
            });

        auto op = ::nstd::net::just(ac == 1? "/dev/null": av[1])
                | ::nstd::net::then([](auto const& name){
                    ::std::cout << "name='" << name << "'\n";
                    return name;
                  })
                | ::nstd::file::open(context, ::nstd::file::open_flags::in)
                | ::nstd::net::then([](int fd){ ::std::cout << "open completed: fd=" << fd << "\n"; })
                ;
        sync_wait(op);
        ::std::cout << "done\n";
    }
    catch(std::exception const& e)
    {
        std::cerr << e.what() << '\n';
    }
    try {
        throw std::make_exception_ptr<std::runtime_error>(std::runtime_error("hello"));
    }
    catch (std::exception const& ex) {
        std::cout << "ex=" << ex.what() << "\n";
    }
    catch (...) {
        std::cout << "unknown exception\n";
    }
}
