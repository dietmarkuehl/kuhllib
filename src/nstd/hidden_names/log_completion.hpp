// nstd/hidden_names/log_completion.hpp                               -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_NSTD_HIDDEN_NAMES_LOG_COMPLETION
#define INCLUDED_NSTD_HIDDEN_NAMES_LOG_COMPLETION

#include "nstd/execution/connect.hpp"
#include "nstd/execution/get_completion_signatures.hpp"
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"

#include <ostream>
#include <string>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {
    inline constexpr struct log_completion_t {
        static auto default_stream() -> ::std::ostream&;

        template <::nstd::execution::receiver Receiver>
        struct receiver {
            Receiver        d_receiver;
            ::std::string   d_msg;
            ::std::ostream& d_stream;

            friend auto tag_invoke(::nstd::execution::get_env_t, receiver const& self) noexcept {
                return ::nstd::execution::get_env(self.d_receiver);
            }
            template <typename... Args>
            friend auto tag_invoke(::nstd::execution::set_value_t, receiver&& self, Args&&... args) noexcept -> void {
                self.d_stream << self.d_msg << (self.d_msg.empty()? "": " ") << "set_value(...)\n";
                ::nstd::execution::set_value(::nstd::utility::move(self.d_receiver),
                                             ::nstd::utility::forward<Args>(args)...);
            }
            template <typename Error>
            friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& self, Error&& error) noexcept -> void {
                self.d_stream << self.d_msg << (self.d_msg.empty()? "": " ") << "set_error(E)\n";
                ::nstd::execution::set_error(::nstd::utility::move(self.d_receiver),
                                             ::nstd::utility::forward<Error>(error));
            }
            friend auto tag_invoke(::nstd::execution::set_stopped_t, receiver&& self) noexcept -> void {
                self.d_stream << self.d_msg << (self.d_msg.empty()? "": " ") << "set_stopped()\n";
                ::nstd::execution::set_stopped(::nstd::utility::move(self.d_receiver));
            }
        };

        template <::nstd::execution::sender Sender>
        struct sender
            : ::nstd::execution::sender_tag {
            Sender          d_sender;
            ::std::string   d_msg;
            ::std::ostream& d_stream;

            template <::nstd::execution::receiver Receiver>
            friend auto tag_invoke(::nstd::execution::connect_t, sender const& self, Receiver&& receiver) {
                using receiver_t = ::nstd::hidden_names::log_completion_t::receiver<::nstd::type_traits::remove_cvref_t<Receiver>>;
                return ::nstd::execution::connect(
                    self.d_sender,
                    receiver_t{
                        ::nstd::utility::forward<Receiver>(receiver),
                        self.d_msg,
                        self.d_stream
                    }
                );
            }
            template <::nstd::execution::receiver Receiver>
            friend auto tag_invoke(::nstd::execution::connect_t, sender&& self, Receiver&& receiver) {
                using receiver_t = ::nstd::hidden_names::log_completion_t::receiver<::nstd::type_traits::remove_cvref_t<Receiver>>;
                return ::nstd::execution::connect(
                    ::nstd::utility::move(self.d_sender),
                    receiver_t{
                        ::nstd::utility::forward<Receiver>(receiver),
                        ::nstd::utility::move(self.d_msg),
                        self.d_stream
                    }
                );
            }
            template <typename Env>
            friend auto tag_invoke(::nstd::execution::get_completion_signatures_t, sender const& self, Env env) noexcept {
                return ::nstd::execution::get_completion_signatures(self.d_sender, env);
            }
        };

        template <::nstd::execution::sender Sender>
        auto operator()(Sender&&             sender,
                        ::std::string const& msg,
                        ::std::ostream&      stream = ::nstd::hidden_names::log_completion_t::default_stream()) const
            -> ::nstd::hidden_names::log_completion_t::sender<::nstd::type_traits::remove_cvref_t<Sender>>
        {
            return { {}, ::nstd::utility::forward<Sender>(sender), msg, stream };
        }
        template <::nstd::execution::sender Sender>
        auto operator()(Sender&&             sender,
                        ::std::ostream&      stream = ::nstd::hidden_names::log_completion_t::default_stream()) const
        {
            return (*this)(::nstd::utility::forward<Sender>(sender), ::std::string(), stream);
        }
        struct closure
            : ::nstd::execution::sender_tag {
            ::std::string   msg;
            ::std::ostream& stream;

            template <::nstd::execution::sender Sender>
            auto operator()(Sender&& sender) const {
                return ::nstd::hidden_names::log_completion_t()(::nstd::utility::forward<Sender>(sender),
                                                              this->msg, this->stream);
            }
        };
        auto operator()(::std::ostream& stream = ::nstd::hidden_names::log_completion_t::default_stream()) const {
            return closure{ {}, ::std::string(), stream };
        }
        auto operator()(::std::string const& msg, ::std::ostream& stream = ::nstd::hidden_names::log_completion_t::default_stream()) const {
            return closure{ {}, msg, stream };
        }
    } log_completion;
}

// ----------------------------------------------------------------------------

#endif
