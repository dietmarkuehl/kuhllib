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

#include "nstd/execution/sender.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/utility/forward.hpp"
#include <ostream>
#include <string>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {
    inline constexpr struct log_completion_t {
        static auto default_stream() -> ::std::ostream&;

        template <::nstd::execution::sender Sender>
        auto operator()(Sender&&             sender,
                        ::std::string const& msg,
                        ::std::ostream&      stream = ::nstd::hidden_names::log_completion_t::default_stream()) const
        {
            return ::nstd::execution::then(::nstd::utility::forward<Sender>(sender),
                                           [&stream, msg](auto&&...){
                                                stream << msg << (msg.empty()? "": " ") << "set_value(...)\n";
                                            })
                |  ::nstd::execution::upon_error([&stream, msg](auto&&){
                                                    stream << msg << (msg.empty()? "": " ") << "set_error(E)\n";
                                                 })
                |  ::nstd::execution::upon_stopped([&stream, msg](){
                                                        stream << msg << (msg.empty()? "": " ") << "set_stopped()\n";
                                                   })
                ;
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
