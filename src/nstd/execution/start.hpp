// nstd/execution/start.hpp                                           -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_START
#define INCLUDED_NSTD_EXECUTION_START

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {
    template <typename State>
    concept has_member_start
        = requires(State& state){ state.start(); }
        ;

    void start();
    template <typename State>
    concept has_non_member_start
        =  !has_member_start<State>
        && requires(State& state){ start(state); }
        ;
}
namespace nstd::execution::inline customization_points {
    inline constexpr struct {
        template <::nstd::hidden_names::has_member_start State>
        auto operator()(State& state) const noexcept(noexcept(state.start()))
        {
            return state.start();
        }
        template <::nstd::hidden_names::has_non_member_start State>
        auto operator()(State& state) const noexcept(noexcept(start(state)))
        {
            return start(state);
        }
        auto operator()(auto&&) const = delete;
    } start;
}

// ----------------------------------------------------------------------------

#endif
