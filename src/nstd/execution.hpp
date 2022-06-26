// nstd/execution.hpp                                                 -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION
#define INCLUDED_NSTD_EXECUTION

// ----------------------------------------------------------------------------

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/get_allocator.hpp"
#include "nstd/execution/get_scheduler.hpp"
#include "nstd/execution/get_stop_token.hpp"
#include "nstd/hidden_names/movable_value.hpp"
#include "nstd/execution/operation_state.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/receiver_of.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/sender_to.hpp"
#include "nstd/hidden_names/has_sender_types.hpp"
#include "nstd/execution/typed_sender.hpp"
#include "nstd/execution/sender_base.hpp"
#include "nstd/execution/sender_traits.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/execution/let_value.hpp"
#include "nstd/execution/start_detached.hpp"
#include "nstd/execution/when_all.hpp"

#include "nstd/execution/repeat_effect.hpp"
#include "nstd/execution/repeat_effect_until.hpp"
#include "nstd/execution/run.hpp"

#if 0
//-dk:TODO
#include "nstd/execution/transfer_just.hpp"
#include "nstd/execution/on.hpp"
#include "nstd/execution/lazy_on.hpp"
#include "nstd/execution/transfer.hpp"
#include "nstd/execution/lazy_transfer.hpp"
#include "nstd/execution/schedule_from.hpp"
#include "nstd/execution/lazy_schedule_from.hpp"
#include "nstd/execution/upon_error.hpp"
#include "nstd/execution/lazy_upon_error.hpp"
#include "nstd/execution/upon_done.hpp"
#include "nstd/execution/lazy_upon_done.hpp"
#include "nstd/execution/lazy_let_value.hpp"
#include "nstd/execution/let_error.hpp"
#include "nstd/execution/lazy_let_error.hpp"
#include "nstd/execution/let_done.hpp"
#include "nstd/execution/lazy_let_done.hpp"
#include "nstd/execution/bulk.hpp"
#include "nstd/execution/lazy_bulk.hpp"
#include "nstd/execution/split.hpp"
#include "nstd/execution/lazy_split.hpp"
#include "nstd/execution/when_all_with_variant.hpp"
#include "nstd/execution/transfer_when_all.hpp"
#include "nstd/execution/lazy_transfer_when_all.hpp"
#include "nstd/execution/transfer_when_all_with_variant.hpp"
#include "nstd/execution/lazy_transfer_when_all_with_variant.hpp"
#include "nstd/execution/into_variant.hpp"
#include "nstd/execution/unschedule.hpp"
#include "nstd/execution/ensure_started.hpp"
#include "nstd/thread/sync_wait_with_variant.hpp"
#include "nstd/execution/execute.hpp"
#endif

// ----------------------------------------------------------------------------

#endif
