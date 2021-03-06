// nstd/projection/model_readable.hpp                                 -*-C++-*-
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

#ifndef INCLUDED_NSTD_PROJECTION_MODEL_READABLE
#define INCLUDED_NSTD_PROJECTION_MODEL_READABLE

#include "nstd/cursor/model_key.hpp"
#include "nstd/projection/model_value.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace projection {
        template <typename...>
        struct model_readable;
    }

}

// ----------------------------------------------------------------------------
  
template <typename... P>
struct nstd::projection::model_readable
{
    constexpr model_readable() noexcept(true) {}
    template <typename T>
    auto operator()(nstd::cursor::model_key<T> const& key) const -> nstd::projection::model_value<T, P...>;
};

// ----------------------------------------------------------------------------

template <typename... P>
    template <typename T>
auto nstd::projection::model_readable<P...>::operator()(nstd::cursor::model_key<T> const& key) const
    -> nstd::projection::model_value<T, P...> {
    return nstd::projection::model_value<T, P...>(key.get_key());
}

// ----------------------------------------------------------------------------

#endif
