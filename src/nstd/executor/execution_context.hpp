// nstd/executor/execution_context.hpp                                -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
//                                                                       
//  Permission is hereby granted, free of charge, to any person          
//  obtaining a copy of this software and associated documentation       
//  files (the "Software"), to deal in the Software without restriction, 
//  including without limitation the rights to use, copy, modify,        
//  merge, publish, distribute, sublicense, and/or sell copies of        
//  the Software, and to permit persons to whom the Software is          
//  furnished to do so, subject to the following conditions:             
//                                                                       
//  The above copyright notice and this permission notice shall be       
//  included in all copies or substantial portions of the Software.      
//                                                                       
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND             
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT          
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,         
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING         
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR        
//  OTHER DEALINGS IN THE SOFTWARE. 
// ----------------------------------------------------------------------------

#ifndef INCLUDED_NSTD_EXECUTOR_EXECUTION_CONTEXT
#define INCLUDED_NSTD_EXECUTOR_EXECUTION_CONTEXT

#include "nstd/executor/fork_event.hpp"
#include "nstd/executor/service_already_exists.hpp"
#include <memory>
#include <mutex>
#include <type_traits>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------

namespace nstd::net {
    class execution_context;
}

// ----------------------------------------------------------------------------

class nstd::net::execution_context
{
public:
    class service;

    execution_context();
    execution_context(execution_context const&) = delete;
    execution_context& operator=(execution_context const&) = delete;
    virtual ~execution_context();

    void notify_fork(::nstd::net::fork_event);

    template <typename Service> static auto key() -> ::std::size_t;
        //-dk:TODO requires ::nstd::net::service<Service>
    template <typename Service>
    auto has_service() const noexcept -> bool {
        ::std::size_t index(this->key<Service>());
        ::std::lock_guard<::std::mutex> kerberos(this->d_bottleneck);
        return this->intern_has_service<Service>(index);
    }
    
    template <typename Service, typename... Args>
    auto make_service(Args&&... args) -> Service& {
        ::std::size_t index(this->key<Service>());
        ::std::lock_guard<::std::mutex> kerberos(this->d_bottleneck);
        if (this->intern_has_service<Service>(index)) {
            throw ::nstd::net::service_already_exists();
        }
        return intern_make_service<Service>(index, ::std::forward<Args>(args)...);
    }
    template <typename Service, typename... Args>
    auto use_service(Args&&... /*-dk:TODO: use args*/) -> typename Service::key_type& {
        ::std::size_t index(this->key<Service>());
        ::std::lock_guard<::std::mutex> kerberos(this->d_bottleneck);
        return this->intern_has_service<Service>(index)
            ? *static_cast<typename Service::key_type*>(this->d_services[index])
            : this->intern_make_service<Service>(index);
            ;
    }

protected:
    void shutdown() noexcept;
    void destroy() noexcept;

private:
    static ::std::size_t next_key();
    template <typename KeyType>
    static auto get_key() -> ::std::size_t;

    template <typename Service>
    auto intern_has_service(::std::size_t index) const noexcept -> bool {
        return index < this->d_services.size() && this->d_services[index];
    }
    template <typename Service, typename... Args>
    auto intern_make_service(::std::size_t index, Args&&... args) -> Service& {
        if (this->d_services.size() <= index) {
            this->d_services.resize(index + 1u);
        }
        Service* rc(new Service(*this, ::std::forward<Args>(args)...));
        this->d_services[index] = static_cast<typename Service::key_type*>(rc);
        return *rc;
    }

    mutable ::std::mutex                                    d_bottleneck;
    bool                                                    d_running = true;
    ::std::vector<::nstd::net::execution_context::service*> d_services;
};

// ----------------------------------------------------------------------------

class nstd::net::execution_context::service
{
    friend class ::nstd::net::execution_context;

protected:
    explicit service(::nstd::net::execution_context&);
    service(service const&) = delete;
    service& operator= (service const&) = delete;
    virtual ~service();

    auto context() noexcept -> ::nstd::net::execution_context&;

private:
    virtual auto shutdown() noexcept -> void = 0;
    virtual auto notify_fork(::nstd::net::fork_event) -> void;

    ::nstd::net::execution_context& d_context;
};

namespace nstd::net {
    template <typename Service>
    concept service
        =  std::is_base_of_v<::nstd::net::execution_context::service, Service>
        && std::is_base_of_v<typename Service::key_type, Service>
        ;

    template<typename Service>
    auto use_service(::nstd::net::execution_context&)
        -> typename Service::key_type&
        requires service<Service>;
    template<typename Service, typename... Args>
    auto make_service(::nstd::net::execution_context&, Args&&...)
        -> Service&;
    template<typename Service>
    auto has_service(::nstd::net::execution_context&)
        -> bool;
}

// ----------------------------------------------------------------------------

template <typename KeyType>
inline auto nstd::net::execution_context::get_key() -> ::std::size_t
{
    static std::size_t const rc{::nstd::net::execution_context::next_key()};
    return rc;
}

template <typename Service>
inline auto nstd::net::execution_context::key() -> ::std::size_t
{
    return ::nstd::net::execution_context::get_key<typename Service::key_type>();
}

// ----------------------------------------------------------------------------

template<typename Service>
inline auto nstd::net::use_service(::nstd::net::execution_context& ctxt)
    -> typename Service::key_type&
    requires service<Service>
{
    return ctxt.use_service<Service>();
}
template<typename Service, typename... Args>
inline auto nstd::net::make_service(::nstd::net::execution_context& ctxt, Args&&... args)
    -> Service&
{
    return ctxt.make_service<Service>(::std::forward<Args>(args)...);
}
template<typename Service>
inline auto nstd::net::has_service(::nstd::net::execution_context& ctxt)
    -> bool
{
    return ctxt.has_service<Service>();
}

// ----------------------------------------------------------------------------

#endif
