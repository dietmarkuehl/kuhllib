// nstd/endian/type.hpp                                               -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2015 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_ENDIAN_TYPE
#define INCLUDED_NSTD_ENDIAN_TYPE

#include <stdint.h>

// ----------------------------------------------------------------------------

namespace nstd {
    namespace endian {
        enum class type {
            uint8,
            uint16,
            uint32,
            uint64,
            int8,
            int16,
            int32,
            int64,
            float32,
            float64
        };

        template <nstd::endian::type> struct type_map;
        template <> struct type_map<nstd::endian::type::uint8>   { using type = uint8_t;  };
        template <> struct type_map<nstd::endian::type::uint16>  { using type = uint16_t; };
        template <> struct type_map<nstd::endian::type::uint32>  { using type = uint32_t; };
        template <> struct type_map<nstd::endian::type::uint64>  { using type = uint64_t; };
        template <> struct type_map<nstd::endian::type::int8>    { using type = int8_t;   };
        template <> struct type_map<nstd::endian::type::int16>   { using type = int16_t;  };
        template <> struct type_map<nstd::endian::type::int32>   { using type = int32_t;  };
        template <> struct type_map<nstd::endian::type::int64>   { using type = int64_t;  };
        template <> struct type_map<nstd::endian::type::float32> { using type = float;    };
        template <> struct type_map<nstd::endian::type::float64> { using type = double;   };

        template <nstd::endian::type T>
        using type_map_t = typename ::nstd::endian::type_map<T>::type;
    }
}

// ----------------------------------------------------------------------------

#endif
