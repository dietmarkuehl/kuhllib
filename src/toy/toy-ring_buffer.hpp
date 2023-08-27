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
#include <type_traits>
#include <utility>
#include <cstdint>
#include <cstdlib>

// ----------------------------------------------------------------------------

namespace toy {
    template <std::uint64_t Size>
    class ring_buffer
    {
    private:
        static constexpr int producer_id{0};
        static constexpr int consumer_id{1};
        struct state_base
        {
            virtual void complete() = 0;
        };
        template <typename Receiver, int Id, typename SizeFun>
        struct state
            : state_base
        {
            struct stop_callback
            {
                state* d_state;
                void operator()() { this->d_state->stop(); }
            };
            using callback_t = typename decltype(get_stop_token(std::declval<Receiver>()))::template callback_type<stop_callback>;
            ring_buffer*                  ring;
            std::remove_cvref_t<Receiver> receiver;
            std::optional<callback_t>     cancel;

            template <typename R>
            state(ring_buffer* ring, R&& receiver)
                : ring(ring)
                , receiver(std::forward<R>(receiver))
            {
            }
            friend void start(state& self)
            {
                if (0u != SizeFun()(self.ring))
                {
                    self.actual_complete();
                }
                else
                {
                    self.ring->d_awaiting[Id] = &self;
                    self.cancel.emplace(get_stop_token(self.receiver), stop_callback{&self});;
                }
            }
            void complete() override
            {
                this->cancel.reset();
                this->actual_complete();
            }
            void actual_complete()
            {
                std::uint64_t size = std::min(Size - this->ring->d_pos[Id] % Size, SizeFun()(this->ring));
                set_value(std::move(receiver), toy::buffer(this->ring->d_buf + this->ring->d_pos[Id] % Size, size));
            }
            void stop()
            {
                set_stopped(std::move(receiver));
            }
        };
        template <int Id, typename SizeFun>
        struct sender
        {
            using result_t = decltype(toy::buffer(std::declval<ring_buffer>().d_buf, 0u));

            ring_buffer* ring;
            template <typename Receiver>
            friend state<Receiver, Id, SizeFun> connect(sender const& self, Receiver&& receiver)
            {
                return state<Receiver, Id, SizeFun>(self.ring, std::forward<Receiver>(receiver));
            }
        };

        char          d_buf[Size];
        state_base*   d_awaiting[2]{ nullptr, nullptr };
        std::uint64_t d_pos[2]{ 0u, 0u };


    public:
        template <typename ProtoSender>
        auto produce(ProtoSender&& proto) {
            using size_fun_t = decltype([](ring_buffer* r){ return Size - (r->d_pos[producer_id] - r->d_pos[consumer_id]); });
            return toy::then(
                std::forward<ProtoSender>(proto)(sender<producer_id, size_fun_t>{this}),
                [this](auto n)
                {
                    std::uint64_t base(this->d_pos[producer_id]);
                    this->d_pos[producer_id] += n;
                    if (this->d_awaiting[consumer_id])
                    {
                        std::exchange(this->d_awaiting[consumer_id], nullptr)->complete();
                    }
                    return std::string_view(d_buf + (base % Size), n);
                }
                );
        }
        template <typename ProtoSender>
        auto consume(ProtoSender&& proto) {
            using size_fun_t = decltype([](ring_buffer* r){ return r->d_pos[producer_id] - r->d_pos[consumer_id]; });
            return toy::then(
                std::forward<ProtoSender>(proto)(sender<consumer_id, size_fun_t>{this}),
                [this](auto n)
                {
                    std::uint64_t base(this->d_pos[consumer_id]);
                    this->d_pos[consumer_id] += n;
                    if (this->d_awaiting[producer_id])
                    {
                        std::exchange(this->d_awaiting[producer_id], nullptr)->complete();
                    }
                    return std::string_view(d_buf + (base % Size), n);
                }
                );
        }
    };
}

// ----------------------------------------------------------------------------

#endif
