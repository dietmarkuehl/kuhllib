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
#include <atomic>
#include <functional>
#include <iostream>
#include <memory>
#include <span>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>
#include <cstddef>

namespace EX = ::nstd::execution;

// ----------------------------------------------------------------------------

namespace audio
{
    class context
    {
    public:
        struct start_desc {};
        struct end_desc {};

    private:
        ::std::vector<::std::byte>                            d_buffer{10u};
        ::std::function<auto(start_desc const&)->void>        d_start;
        ::std::function<auto(::std::span<::std::byte>)->void> d_process;
        ::std::function<auto(end_desc const&)->void>          d_end;
        ::std::unique_ptr<::std::jthread>                     d_thread;

    public:
        template <typename Start, typename Process, typename End>
        auto start(Start&& s, Process&& p, End&& e) ->void {
            this->d_start   = ::std::forward<Start>(s);
            this->d_process = ::std::forward<Process>(p);
            this->d_end     = ::std::forward<End>(e);
            this->d_thread = ::std::make_unique<::std::jthread>([this]{ this->run(3u); });
        }

        auto run(::std::size_t count) -> void {
            this->d_start(start_desc());
            for (::std::size_t i{0u}; i != count; ++i) {
                this->d_process(this->d_buffer);
            }
            this->d_end(end_desc());
        }
    };

    class buffer_processor {
    protected:
        virtual auto do_process(::std::span<::std::byte>) -> void = 0;
    public:
        virtual ~buffer_processor() {}
        auto process(::std::span<::std::byte> buffer) -> void {
            this->do_process(buffer);
        }
    };

    template <EX::receiver Receiver>
    struct buffer_state
        : buffer_processor
    {
        ::std::remove_cvref_t<Receiver>            d_receiver;
        ::std::atomic<::audio::buffer_processor*>* d_hook;

        template <EX::receiver R>
        buffer_state(R&& r, ::std::atomic<::audio::buffer_processor*>* hook)
            : d_receiver(::std::forward<R>(r))
            , d_hook(hook)
        {
        }

        friend auto tag_invoke(EX::start_t, buffer_state& self) noexcept -> void {
            *self.d_hook = &self;
        }

        auto do_process(::std::span<::std::byte> buffer) -> void override {
            if (buffer.empty()) {
                EX::set_done(::std::move(this->d_receiver));
            }
            else {
                EX::set_value(::std::move(this->d_receiver), buffer);
            }
        }
    };

    struct buffer_sender
    {
        using completion_signatures = EX::completion_signatures<
                EX::set_value_t(::std::span<::std::byte>)
            >;
        template <template <typename...> class T, template <typename...> class V>
        using value_types = V<T<::std::span<::std::byte>>>;
        template <template <typename...> class V>
        using error_types = V<::std::exception_ptr>;
        static constexpr bool sends_done = false;

        ::std::atomic<::audio::buffer_processor*>* d_hook;

        buffer_sender(::std::atomic<::audio::buffer_processor*>* hook): d_hook(hook) {}

        template <EX::receiver Receiver>
        friend auto tag_invoke(EX::connect_t, buffer_sender const& self, Receiver&& receiver) noexcept
            -> audio::buffer_state<Receiver> {
            return audio::buffer_state<Receiver>(::std::forward<Receiver>(receiver), self.d_hook);
        }
    };

    struct start_description {
        ::std::atomic<::audio::buffer_processor*>* d_hook;
        auto process() const -> audio::buffer_sender {
            return audio::buffer_sender(this->d_hook);
        }
    };

    template <EX::receiver Receiver>
    struct state
    {
        ::std::remove_cvref_t<Receiver>           d_receiver;
        audio::context*                           d_context;
        ::std::atomic<::audio::buffer_processor*> d_hook;
        friend auto tag_invoke(EX::start_t, state& self) noexcept -> void {
            self.d_context->start(
                [&self](::audio::context::start_desc){
                    EX::set_value(::std::move(self.d_receiver), start_description{ &self.d_hook });
                },
                [&self](::std::span<::std::byte> buffer) {
                    // === problem ===
                    while (self.d_hook == nullptr) {
                    }
                    audio::buffer_processor* hook = self.d_hook;
                    self.d_hook = nullptr;
                    ::std::cout << "need to fill in bytes\n";
                    hook->process(buffer);
                },
                [&self](::audio::context::end_desc){
                    while (self.d_hook == nullptr) {
                    }
                    audio::buffer_processor* hook = self.d_hook;
                    self.d_hook = nullptr;
                    static ::std::byte bytes[1];
                    hook->process(::std::span<::std::byte>(+bytes, 0u));
                }
            );
        }
    };

    struct sender
    {
        using completion_signatures = EX::completion_signatures<
                EX::set_value_t(audio::start_description)
            >;
        template <template <typename...> class T, template <typename...> class V>
        using value_types = V<T<audio::start_description>>;
        template <template <typename...> class V>
        using error_types = V<std::exception_ptr>;
        static constexpr bool sends_done = false;

        ::audio::context* d_context;

        template <EX::receiver Receiver>
        friend auto tag_invoke(EX::connect_t, sender const& self, Receiver&& receiver) -> state<Receiver>
        {
            return state<Receiver>{ ::std::forward<Receiver>(receiver), self.d_context, {} };
        }
        template <EX::receiver Receiver>
        friend auto tag_invoke(EX::connect_t, sender&& self, Receiver&& receiver) -> state<Receiver>
        {
            return state<Receiver>{ ::std::forward<Receiver>(receiver), self.d_context, {} };
        }
    };

    struct scheduler
    {
        ::audio::context* d_context;
        friend auto tag_invoke(EX::schedule_t, scheduler const& self) noexcept -> audio::sender {
            return audio::sender{ self.d_context };
        }
    };

    auto get_configured_audio_device(::audio::context& context) -> scheduler {
        return audio::scheduler{&context};
    }
}

// ----------------------------------------------------------------------------

int main()
{
    audio::context context;
    auto s = audio::get_configured_audio_device(context);

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
