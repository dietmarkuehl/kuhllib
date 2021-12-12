// nstd/container/intrusive_list.hpp                                  -*-C++-*-
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

#ifndef INCLUDED_NSTD_CONTAINER_INTRUSIVE_LIST
#define INCLUDED_NSTD_CONTAINER_INTRUSIVE_LIST

#include <iterator>

// ----------------------------------------------------------------------------

namespace nstd::container {
    template <typename> class intrusive_list_node;
    template <typename T, typename = decltype([](T& e)->auto&{ return e.link; })> 
    class intrusive_list;
}

// ----------------------------------------------------------------------------

template <typename T>
class nstd::container::intrusive_list_node
{
public:
    T* d_next;
    T* d_prev;
};

// ----------------------------------------------------------------------------

template <typename T, typename Access>
class nstd::container::intrusive_list
    : private Access
{
private:
    mutable T d_link;

    intrusive_list(intrusive_list&&) = delete;

    auto link(T& obj) const -> auto& { return (*this)(obj); }

public:
    struct iterator
        : private Access
    {
        using value_type = T;
        using pointer    = T*;
        using reference  = T&;
        using difference_type = ::std::ptrdiff_t;
        using iterator_category = ::std::forward_iterator_tag;

    private:
        T* d_node;
        auto& link(T& obj) const { return (*this)(obj); }

    public:
        iterator(T* node = nullptr): d_node(node) {}

        auto operator*()  const -> T& { return *this->d_node; }
        auto operator->() const -> T* { return this->d_node; }
        auto operator== (iterator const& ) const -> bool  = default;
        auto operator++() -> iterator& {
            this->d_node = this->link(*this->d_node).d_next;
            return *this;
        }
        auto operator++(int) -> iterator{
            iterator rc(*this);
            ++*this;
            return rc;
        }
    };

    intrusive_list() {
        auto& link(this->link(this->d_link));
        link.d_prev = link.d_next = &this->d_link;
    }

    iterator insert(iterator pos, T& obj) {
        auto& next_link(this->link(*pos));
        auto& obj_link(this->link(obj));

        obj_link.d_next = &*pos;
        obj_link.d_prev = next_link.d_prev;
        next_link.d_prev = &obj;
        this->link(*obj_link.d_prev).d_next = &obj;

        return {&obj};
    }
    void erase(iterator pos) {
        auto& link(this->link(*pos));
        this->link(*link.d_prev).d_next = link.d_next;
        this->link(*link.d_next).d_prev = link.d_prev;
    }

    auto begin() const -> iterator { return this->link(this->d_link).d_next; }
    auto end() const   -> iterator { return &this->d_link; }
};

// ----------------------------------------------------------------------------

#endif
