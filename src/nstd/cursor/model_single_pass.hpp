// nstd/cursor/model_single_pass.hpp                                  -*-C++-*-
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

#ifndef INCLUDED_NSTD_CURSOR_MODEL_SINGLE_PASS
#define INCLUDED_NSTD_CURSOR_MODEL_SINGLE_PASS

#include "nstd/cursor/category.hpp"
#include "nstd/cursor/model_key.hpp"
#include "nstd/cheaders/cstddef.hpp"

// ----------------------------------------------------------------------------
// The model_single_pass class template is providing a single-pass cursor for
// testing.

namespace nstd
{
    namespace cursor
    {
        template <typename T> class model_single_pass_end;
        template <typename T> class model_single_pass;

        template <typename T>
        auto cursor_category(model_single_pass<T> const& cursor) -> nstd::cursor::single_pass;
        template <typename T>
        auto cursor_step(model_single_pass<T>& cursor) -> void;
        template <typename T>
        auto cursor_key(model_single_pass<T> const& cursor) -> model_key<T>;
        template <typename T>
        auto cursor_at_same_pos(model_single_pass<T> const& cursor0,
                                model_single_pass<T> const& cursor1) -> bool;
        template <typename T>
        auto cursor_at_same_pos(model_single_pass<T> const&     cursor,
                                model_single_pass_end<T> const& end) -> bool;

        template <typename T, ::nstd::size_t Size>
        auto single_pass_begin(T (&array)[Size]) -> model_single_pass<T>;
        template <typename T, ::nstd::size_t Size>
        auto single_pass_end(T (&array)[Size]) -> model_single_pass_end<T>;
    }
}

// ----------------------------------------------------------------------------

template <typename T>
class nstd::cursor::model_single_pass
{
    T* pointer;
public:
    explicit model_single_pass(T* pointer): pointer(pointer) {}
    auto get_pointer() const -> T const* { return this->pointer; }
    auto get_key() const -> model_key<T> { return model_key<T>(*pointer); }
    auto next() -> void { ++this->pointer; }
};

// ----------------------------------------------------------------------------

template <typename T>
class nstd::cursor::model_single_pass_end
{
    T* pointer;
public:
    explicit model_single_pass_end(T* pointer): pointer(pointer) {}
    auto get_pointer() const -> T const* { return this->pointer; }
};

// ----------------------------------------------------------------------------

template <typename T>
auto nstd::cursor::cursor_step(model_single_pass<T>& cursor) -> void
{
    cursor.next();
}

template <typename T>
auto nstd::cursor::cursor_key(model_single_pass<T> const& cursor) -> nstd::cursor::model_key<T>
{
    return cursor.get_key();
}

template <typename T>
auto nstd::cursor::cursor_at_same_pos(model_single_pass<T> const& cursor0,
                                      model_single_pass<T> const& cursor1) -> bool
{
    return cursor0.get_pointer() == cursor1.get_pointer();
}

template <typename T>
auto nstd::cursor::cursor_at_same_pos(model_single_pass<T> const&     cursor,
                                      model_single_pass_end<T> const& end) -> bool
{
    return cursor.get_pointer() == end.get_pointer();
}

// ----------------------------------------------------------------------------

template <typename T, ::nstd::size_t Size>
auto nstd::cursor::single_pass_begin(T (&array)[Size]) -> nstd::cursor::model_single_pass<T>
{
    return nstd::cursor::model_single_pass<T>{array};
}

template <typename T, ::nstd::size_t Size>
auto nstd::cursor::single_pass_end(T (&array)[Size]) -> nstd::cursor::model_single_pass_end<T>
{
    return nstd::cursor::model_single_pass_end<T>{array + Size};
}

// ----------------------------------------------------------------------------

#endif
