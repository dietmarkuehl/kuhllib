// nstd/functional/reference_wrapper.hpp                              -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_FUNCTIONAL_REFERENCE_WRAPPER
#define INCLUDED_NSTD_FUNCTIONAL_REFERENCE_WRAPPER

#include "nstd/functional/invoke.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/type_traits/result_of.hpp"
#include "nstd/type_traits/integral_constant.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace functional {
        template <typename> class reference_wrapper;
        template <typename> struct is_reference_wrapper;
        template <typename T> struct is_reference_wrapper<reference_wrapper<T> >;

        template <typename T>
        auto ref(T&) noexcept(true) -> nstd::functional::reference_wrapper<T>;
        template <typename T>
        auto ref(nstd::functional::reference_wrapper<T>) noexcept(true) -> nstd::functional::reference_wrapper<T>;
        template <typename T>
        auto ref(T const&&) -> void = delete;

        template <typename T>
        auto cref(T const&) noexcept(true) -> nstd::functional::reference_wrapper<T const>;
        template <typename T>
        auto cref(nstd::functional::reference_wrapper<T>) noexcept(true) -> nstd::functional::reference_wrapper<T const>;
        template <typename T>
        auto cref(T const&&) -> void = delete;
    }
}

// ----------------------------------------------------------------------------

template <typename T>
class nstd::functional::reference_wrapper
{
    T* pointer;
public:
    using type = T;
    //-dk:TODO function related typedefs

    reference_wrapper(T& object) noexcept(true): pointer(&object) {}
    reference_wrapper(T&&) = delete;

    operator T&() const noexcept(true) { return *this->pointer; }
    auto get() const noexcept(true) -> T& { return *this->pointer; }

    template <typename... Args>
    auto operator()(Args&&... args) const -> nstd::type_traits::result_of_t<T&(Args...)> {
        return ::nstd::functional::invoke(this->get(), nstd::utility::forward<Args>(args)...);
    }

    // extension
    auto operator==(nstd::functional::reference_wrapper<T> other) const noexcept(true) -> bool;
    auto operator!=(nstd::functional::reference_wrapper<T> other) const noexcept(true) -> bool;
};

// ----------------------------------------------------------------------------

template <typename>
struct nstd::functional::is_reference_wrapper
    : nstd::type_traits::false_type {
};
template <typename T>
struct nstd::functional::is_reference_wrapper<nstd::functional::reference_wrapper<T> >
    : nstd::type_traits::true_type {
};

// ----------------------------------------------------------------------------

template <typename T>
auto nstd::functional::ref(T& object) noexcept(true)
-> nstd::functional::reference_wrapper<T> {
    return nstd::functional::reference_wrapper<T>(object);
}
template <typename T>
auto nstd::functional::ref(nstd::functional::reference_wrapper<T> value) noexcept(true)
    -> nstd::functional::reference_wrapper<T>  {
    return value;
}

// ----------------------------------------------------------------------------

template <typename T>
auto nstd::functional::cref(T const& object) noexcept(true)
-> nstd::functional::reference_wrapper<T const> {
    return nstd::functional::reference_wrapper<T const>(object);
}
template <typename T>
auto nstd::functional::cref(nstd::functional::reference_wrapper<T> value) noexcept(true)
    -> nstd::functional::reference_wrapper<T const>  {
    return nstd::functional::reference_wrapper<T const>(value.get());
}

// ----------------------------------------------------------------------------

template <typename T>
auto nstd::functional::reference_wrapper<T>::operator==(nstd::functional::reference_wrapper<T> other) const noexcept(true)
    -> bool {
    return this->pointer == other.pointer;
}

template <typename T>
auto nstd::functional::reference_wrapper<T>::operator!=(nstd::functional::reference_wrapper<T> other) const noexcept(true)
    -> bool {
    return !(*this == other);
}

// ----------------------------------------------------------------------------

#endif
