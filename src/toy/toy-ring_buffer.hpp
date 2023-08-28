// toy-ring_buffer.hpp                                                -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2023 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_TOY_RING_BUFFER
#define INCLUDED_TOY_RING_BUFFER

#include "toy-sender.hpp"
#include "toy-networking.hpp"
#include <span>
#include <type_traits>
#include <utility>
#include <cstdint>
#include <cstdlib>
#include <cassert>

// ----------------------------------------------------------------------------

namespace toy
{
    template <std::uint64_t Size, typename T = char> class ring_buffer;
}

template <std::uint64_t Size, typename T>
class toy::ring_buffer
{
public:
    enum class side { producer = 0, consumer = 1 };
    template <side Side>
    class result
    {
    private:
        static constexpr side other_side{Side == side::producer? side::consumer: side::producer};
        ring_buffer* ring;
    public:
        std::span<T> const buffer;

        result(ring_buffer* ring, std::span<T> buffer)
            : ring(ring)
            , buffer(buffer)
        {
        }
        T*          data()        { return buffer.data(); }
        std::size_t size() const  { return buffer.size(); }
        bool        empty() const { return buffer.empty(); }
        void advance(std::uint64_t n)
        {
            this->ring->pos[int(Side)] += n;
            if (this->ring->awaiting[int(other_side)] && 0 < n)
            {
                std::exchange(this->ring->awaiting[int(other_side)], nullptr)->complete();
            }
        }
    };
    struct state_base
    {
        virtual void complete() = 0;
        virtual void stop() = 0;
    };
    template <side Side, typename Receiver>
    struct state final
        : state_base
    {
        struct stop_callback
        {
            state_base* obj;
            void operator()() { this->obj->stop(); }
        };
        using stop_token_t = decltype(get_stop_token(std::declval<Receiver>()));
        using callback_t = typename stop_token_t::template callback_type<stop_callback>;

        ring_buffer*                 ring;
        std::remove_cvref_t<Receiver> receiver;
        std::optional<callback_t>     callback;
        template <typename R>
        state(ring_buffer* ring, R&& receiver)
            : ring(ring)
            , receiver(std::forward<R>(receiver))
        {
        }
        friend void start(state& self)
        {
            std::uint64_t diff{self.ring->pos[int(side::producer)] - self.ring->pos[int(side::consumer)]};
            std::uint64_t size{Side == side::producer? Size - diff: diff};
            if (0u < size)
            {
                self.complete();
            }
            else
            {
                self.ring->awaiting[int(Side)] = &self;
                self.callback.emplace(get_stop_token(self.receiver), stop_callback{&self});
            }
        }
        void complete() override final
        {
            std::uint64_t diff{this->ring->pos[int(side::producer)] - this->ring->pos[int(side::consumer)]};
            std::uint64_t size{Side == side::producer? Size - diff: diff};
            std::uint64_t offset{this->ring->pos[int(Side)] % Size};
            callback.reset();
            set_value(std::move(this->receiver), result<Side>{this->ring, std::span<T>(this->ring->buffer + offset, std::min(Size - offset, size))});
        }
        void stop() override final
        {
            set_stopped(std::move(this->receiver));
        }
    };
    template <side Side>
    struct sender
    {
        using result_t = result<Side>;

        ring_buffer* ring;

        template <typename Receiver>
        friend state<Side, Receiver> connect(sender const& self, Receiver&& receiver)
        {
            return state<Side, Receiver>( self.ring, std::forward<Receiver>(receiver) );
        }
    };

private:
    T             buffer[Size];
    std::uint64_t pos[2]{};
    state_base*   awaiting[2]{};

public:
    sender<side::producer> produce() { return sender<side::producer>{this}; }
    sender<side::consumer> consume() { return sender<side::consumer>{this}; }
};

// ----------------------------------------------------------------------------

#endif
