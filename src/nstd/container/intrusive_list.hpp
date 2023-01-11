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

#include "nstd/utility/swap.hpp"
#include <iterator>

// ----------------------------------------------------------------------------

namespace nstd::container {
    struct intrusive_list_link {
        template <typename T>
        auto operator()(T& e) const -> auto& { return e.link; }
        auto operator== (intrusive_list_link const&) const -> bool = default;
    };
    template <typename> class intrusive_list_node;
    template <typename T, typename = intrusive_list_link>
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
    mutable T d_head;

    intrusive_list(intrusive_list const&) = delete;

    auto link(T& obj) const -> auto& { return (*this)(obj); }

    auto connect(T& prev, T& next) {
        this->link(prev).d_next = &next;
        this->link(next).d_prev = &prev;
    }
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
        connect(this->d_head, this->d_head);
    }
    intrusive_list(intrusive_list&& other)
    {
        connect(this->d_head, *link(other.d_head).d_next);
        connect(*link(other.d_head).d_prev, this->d_head);
    }
    auto operator= (intrusive_list&& other) -> intrusive_list& {
        connect(this->d_head, *link(other.d_head).d_next);
        connect(*link(other.d_head).d_prev, this->d_head);
        return *this;
    }

    auto insert(iterator pos, T& obj) -> iterator {
        auto& next_link(this->link(*pos));
        auto& obj_link(this->link(obj));

        obj_link.d_next = &*pos;
        obj_link.d_prev = next_link.d_prev;
        next_link.d_prev = &obj;
        this->link(*obj_link.d_prev).d_next = &obj;

        return {&obj};
    }
    auto push_back(T& obj) -> void {
        this->insert(this->end(), obj);
    }
    auto erase(iterator pos) -> void {
        auto& link(this->link(*pos));
        this->link(*link.d_prev).d_next = link.d_next;
        this->link(*link.d_next).d_prev = link.d_prev;
        link.d_next = link.d_prev = &*pos;
    }
    auto pop_front() -> void {
        this->erase(this->begin());
    }

    auto front() -> T& { return *this->link(this->d_head).d_next; }
    auto empty() const -> bool { return &this->d_head == this->link(this->d_head).d_next; }
    auto make_iterator(T* ptr) const -> iterator { return ptr; }
    auto begin() const -> iterator { return this->link(this->d_head).d_next; }
    auto end() const   -> iterator { return &this->d_head; }

    auto swap(intrusive_list& other) -> void {
        ::nstd::utility::swap(this->link(this->d_head).d_next, this->link(other.d_head).d_next);
        ::nstd::utility::swap(this->link(this->d_head).d_prev, this->link(other.d_head).d_prev);
    }
};

// ----------------------------------------------------------------------------

#endif
