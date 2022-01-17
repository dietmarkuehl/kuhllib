// audio.cpp                                                          -*-C++-*-
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
// This file contains a mock-up of how the use of P2300 with an audio
// interface might look like.
// ----------------------------------------------------------------------------

#include "nstd/execution.hpp"
#include "nstd/thread/sync_wait.hpp"
#include <iostream>
#include <span>
#include <type_traits>
#include <utility>
#include <vector>
#include <cstddef>

namespace EX = ::nstd::execution;

// ----------------------------------------------------------------------------

namespace audio
{
    template <EX::receiver Receiver>
    struct buffer_state {
        ::std::remove_cvref_t<Receiver> d_receiver;
        ::std::span<std::byte>          d_buffer;

        friend auto tag_invoke(EX::start_t, buffer_state& self) noexcept -> void {
            if (self.d_buffer.empty()) {
                EX::set_done(::std::move(self.d_receiver));
            }
            else {
                EX::set_value(::std::move(self.d_receiver), self.d_buffer);
            }
        }
    };

    struct buffer_sender {
        template <template <typename...> class T, template <typename...> class V>
        using value_types = V<T<::std::span<::std::byte>>>;
        template <template <typename...> class V>
        using error_types = V<::std::exception_ptr>;
        static constexpr bool sends_done = false;

        mutable ::std::size_t               d_count;
        mutable ::std::vector<::std::byte>  d_buffer;

        buffer_sender(::std::size_t count): d_count(count), d_buffer(10) {}

        template <EX::receiver Receiver>
        friend auto tag_invoke(EX::connect_t, buffer_sender const& self, Receiver&& receiver) noexcept
            -> audio::buffer_state<Receiver> {
            return audio::buffer_state<Receiver>{
                ::std::forward<Receiver>(receiver),
                self.d_count--? ::std::span(self.d_buffer): ::std::span(self.d_buffer.begin(), self.d_buffer.begin())
                };
        }
    };

    struct start_description {
        std::size_t d_count;
        auto process() const -> audio::buffer_sender {
            return audio::buffer_sender(this->d_count);
        }
    };

    template <EX::receiver Receiver>
    struct state
    {
        ::std::remove_cvref_t<Receiver> d_receiver;
        audio::start_description       d_start_description;
        friend auto tag_invoke(EX::start_t, state& self) noexcept -> void {
            EX::set_value(::std::move(self.d_receiver), self.d_start_description);
        }
    };

    struct sender
    {
        template <template <typename...> class T, template <typename...> class V>
        using value_types = V<T<audio::start_description&>>;
        template <template <typename...> class V>
        using error_types = V<std::exception_ptr>;
        static constexpr bool sends_done = false;

        std::size_t d_count;

        template <EX::receiver Receiver>
        friend auto tag_invoke(EX::connect_t, sender const& self, Receiver&& receiver) -> state<Receiver>
        {
            return state<Receiver>{ ::std::forward<Receiver>(receiver), { self.d_count } };
        }
        template <EX::receiver Receiver>
        friend auto tag_invoke(EX::connect_t, sender&& self, Receiver&& receiver) -> state<Receiver>
        {
            return state<Receiver>{ ::std::forward<Receiver>(receiver), { self.d_count } };
        }
    };

    struct scheduler
    {
        ::std::size_t d_count;
        friend auto tag_invoke(EX::schedule_t, scheduler const& self) noexcept -> audio::sender {
            return audio::sender{ self.d_count };
        }
    };

    auto get_configured_audio_device(::std::size_t count) -> scheduler {
        return audio::scheduler{count};
    }
}

// ----------------------------------------------------------------------------

int main()
{
    auto s = audio::get_configured_audio_device(3);

    auto sender
        = EX::schedule(s)
        | EX::let_value([](audio::start_description const& d){
            return EX::repeat_effect(
                       d.process()
                       | EX::then([](::std::span<::std::byte>) {
                           ::std::cout << "filling buffer\n";
                       })
                )
                ;
        })
        ;

#if 0
Auto sender
   = Schedule(s)
   | let_value([entity = AudioState()](start_description const& d) {
         entity.setup(d);
         return schedule(d.process())
             | repeat_until(then(overload([&entity](buffer& b) { entity.fill(b); },
                                                               [&entity](stop_description& d) { entity.clean_up(d); }),
                                      [&entity]{ return entity.done(); }) 
             ;
   })
  ;
#endif

    ::nstd::this_thread::sync_wait(::std::move(sender));
}
