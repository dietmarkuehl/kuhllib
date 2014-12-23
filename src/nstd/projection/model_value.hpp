// nstd/projection/model_value.hpp                                    -*-C++-*-
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

#ifndef INCLUDED_NSTD_PROJECTION_MODEL_VALUE
#define INCLUDED_NSTD_PROJECTION_MODEL_VALUE

#include "nstd/utility/forward.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace projection {
        template <typename T, typename...> class model_value;
        template <typename S, typename...SP, typename T, typename... TP>
        auto compare(model_value<S, SP...> const&, model_value<T, TP...> const&) -> int;
        template <typename S, typename...SP>
        auto compare(model_value<S, SP...> const&, S const&) -> int;
        template <typename S, typename...SP>
        auto compare(S const&, model_value<S, SP...> const&) -> int;
    }

}

// ----------------------------------------------------------------------------

template <typename T, typename...>
class nstd::projection::model_value
{
    T value;
public:
    model_value(model_value const& other): value(other.value) {}
    model_value(model_value&& other): value(other.value) {}
    
    template <typename S, typename... P>
    model_value(model_value<S, P...>& other): value(other.get_value()) {}
    template <typename S, typename... P>
    model_value(model_value<S, P...> const& other): value(other.get_value()) {}
    template <typename S, typename... P>
    model_value(model_value<S, P...>&& other): value(other.get_value()) {}

    template <typename S>
    explicit model_value(S&& value): value(nstd::utility::forward<S>(value)) {}
    T const& get_value() const { return this->value; }
};

// ----------------------------------------------------------------------------

template <typename S, typename...SP, typename T, typename... TP>
auto nstd::projection::compare(model_value<S, SP...> const& v0, model_value<T, TP...> const& v1)
    -> int {
    return v0.get_value() - v1.get_value();
}

template <typename S, typename...SP>
auto nstd::projection::compare(nstd::projection::model_value<S, SP...> const& v0, S const& v1)
    -> int {
    return v0.get_value() - v1;
}
template <typename S, typename...SP>
auto nstd::projection::compare(S const& v0, nstd::projection::model_value<S, SP...> const& v1)
    -> int {
    return v0 - v1.get_value();
}

// ----------------------------------------------------------------------------

#endif
