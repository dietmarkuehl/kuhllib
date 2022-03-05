cmake_minimum_required(VERSION 3.12.0)
project(kuhllib VERSION 0.1.0)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CTest)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wall -Werror -fconcepts-diagnostics-depth=4 -ftemplate-backtrace-limit=0")
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wall -Werror -stdlib=libc++")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX")
endif()

CHECK_INCLUDE_FILES(linux/io_uring.h NSTD_HAS_LINUX_IO_URING)
CHECK_INCLUDE_FILES(linux/time_types.h NSTD_HAS_LINUX_TIME_TYPES)
CHECK_SYMBOL_EXISTS("unistd.h;sys/mman.h" MAP_POPULATE NSTD_HAS_MAP_POPULATE)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/src/nstd/nstd-config.hpp.in ${CMAKE_BINARY_DIR}/include/nstd/nstd-config.hpp)

include_directories(${CMAKE_BINARY_DIR}/include ${CMAKE_SOURCE_DIR}/src)
enable_testing()

set(KUHLLIB_HEADER_SUFFIX .hpp)
set(KUHLLIB_SOURCE_SUFFIX .cpp)
set(KUHLLIB_TEST_SUFFIX .t.cpp)

list(APPEND kuhllib_test_components
    src/kuhl/test/assertions
    src/kuhl/test/context
    src/kuhl/test/kuhltest_test
    src/kuhl/test
    )

list(APPEND kuhllib_algorithm_components
    src/nstd/algorithm/all_of
    src/nstd/algorithm/any_of
    src/nstd/algorithm/distance
    src/nstd/algorithm/find
    src/nstd/algorithm/find_end
    src/nstd/algorithm/find_if
    src/nstd/algorithm/find_if_not
    src/nstd/algorithm/for_each
    src/nstd/algorithm/min
    src/nstd/algorithm/mismatch
    src/nstd/algorithm/none_of
    src/nstd/algorithm/search
)

list(APPEND kuhllib_atomic_components
    src/nstd/atomic/atomic
)

list(APPEND kuhllib_buffer_components
    src/nstd/buffer/const_buffer
    src/nstd/buffer/make_buffer
    src/nstd/buffer/mutable_buffer
)

list(APPEND kuhllib_concepts_components
    src/nstd/concepts/boolean_testable
    src/nstd/concepts/same_as
)

list(APPEND kuhllib_container_components
    src/nstd/container/intrusive_list
)

list(APPEND kuhllib_xcursor_components
    src/nstd/cursor/array
    src/nstd/cursor/category
    src/nstd/cursor/model_key
    src/nstd/cursor/model_single_pass
    src/nstd/cursor/single_pass
)

list(APPEND kuhllib_endian_components
    src/nstd/endian/big_to_native
    src/nstd/endian/literals
    src/nstd/endian/type
)

list(APPEND kuhllib_execution_components
    src/nstd/execution/get_env
    src/nstd/execution/no_env
    src/nstd/execution/receiver
)

list(APPEND kuhllib_execution_todo
    src/nstd/execution/forward_progress_guarantee
    src/nstd/execution/forwarding_env_query
    src/nstd/execution/forwarding_scheduler_query
    src/nstd/execution/get_allocator
    src/nstd/execution/get_delegatee_scheduler
    src/nstd/execution/get_scheduler
    src/nstd/execution/get_stop_token
    src/nstd/execution/scheduler
)

list(APPEND kuhllib_xexecution_components
    src/nstd/execution
    src/nstd/execution/completion_signatures
    src/nstd/execution/completion_signatures_of_t
    src/nstd/execution/connect
    src/nstd/execution/dependent_completion_signatures
    src/nstd/execution/execution
    src/nstd/execution/get_completion_scheduler
    src/nstd/execution/get_completion_signatures
    src/nstd/execution/inject_cancel
    src/nstd/execution/is_execution_policy
    src/nstd/execution/just
    src/nstd/execution/just_done
    src/nstd/execution/lazy_then
    src/nstd/execution/let_value
    src/nstd/execution/operation_state
    src/nstd/execution/parallel_policy
    src/nstd/execution/parallel_unsequenced_policy
    src/nstd/execution/receiver_of
    src/nstd/execution/repeat_effect
    src/nstd/execution/repeat_effect_until
    src/nstd/execution/run
    src/nstd/execution/schedule
    src/nstd/execution/sender
    src/nstd/execution/sender_base
    src/nstd/execution/sender_to
    src/nstd/execution/sender_traits
    src/nstd/execution/sequenced_policy
    src/nstd/execution/set_error
    src/nstd/execution/set_stopped
    src/nstd/execution/set_value
    src/nstd/execution/start
    src/nstd/execution/start_detached
    src/nstd/execution/stop_token_type
    src/nstd/execution/task
    src/nstd/execution/then
    src/nstd/execution/timeout
    src/nstd/execution/typed_sender
    src/nstd/execution/upon_done
    src/nstd/execution/value_types_of_t
    src/nstd/execution/when_all
)

list(APPEND kuhllib_xexecutor_components
    src/nstd/executor/associated_allocator
    src/nstd/executor/associated_executor
    src/nstd/executor/async_completion
    src/nstd/executor/async_result
    src/nstd/executor/execution_context
    src/nstd/executor/executor
    src/nstd/executor/executor_binder
    src/nstd/executor/fork_event
    src/nstd/executor/service_already_exists
    src/nstd/executor/system_executor
)

list(APPEND kuhllib_xfile_components
    src/nstd/file/context
    src/nstd/file/descriptor
    src/nstd/file/io_base
    src/nstd/file/mapped_memory
    src/nstd/file/observer_context
    src/nstd/file/open
    src/nstd/file/open_flags
    src/nstd/file/operation
    src/nstd/file/read
    src/nstd/file/poll_context
    src/nstd/file/ring
    src/nstd/file/ring_context
    src/nstd/file/test_context
)

list(APPEND kuhllib_functional_components
    src/nstd/functional/bind
    src/nstd/functional/equal_to
    src/nstd/functional/invoke
    src/nstd/functional/is_bind_expression
    src/nstd/functional/mem_fn
    src/nstd/functional/model_predicate
    src/nstd/functional/not_
    src/nstd/functional/placeholders
    src/nstd/functional/reference_wrapper
    src/nstd/functional/tag_invoke
    src/nstd/functional/tuple_invoke
)

list(APPEND kuhllib_hidden_components
    src/nstd/hidden_names/movable_value
)
list(APPEND kuhllib_xhidden_components
    src/nstd/hidden_names/add_signatures
    src/nstd/hidden_names/boolean_socket_option
    src/nstd/hidden_names/callable
    src/nstd/hidden_names/check_type_alias_exists
    src/nstd/hidden_names/constexpr_expression
    src/nstd/hidden_names/decayed_tuple
    src/nstd/hidden_names/derives_from_sender_base
    src/nstd/hidden_names/has_sender_types
    src/nstd/hidden_names/is_instance_of
    src/nstd/hidden_names/message_flags
    src/nstd/hidden_names/no_completion_signatures
    src/nstd/hidden_names/sender_base
    src/nstd/hidden_names/type_list
    src/nstd/hidden_names/valid_completion_signatures
    src/nstd/hidden_names/variant_or_empty
)

list(APPEND kuhllib_iterator_components
    src/nstd/iterator/bidirectional_iterator
    src/nstd/iterator/bidirectional_iterator_tag
    src/nstd/iterator/forward_iterator
    src/nstd/iterator/forward_iterator_tag
    src/nstd/iterator/iterator_traits
)

list(APPEND kuhllib_memory_components
    src/nstd/memory/addressof
    src/nstd/memory/uninitialized_default_construct
)

list(APPEND kuhllib_xnet_components
    src/nstd/net/async_accept
    src/nstd/net/async_connect
    src/nstd/net/async_io
    src/nstd/net/async_read_some
    src/nstd/net/async_send
    src/nstd/net/async_write_some
    src/nstd/net/ip/address
    src/nstd/net/ip/address_v4
    src/nstd/net/ip/address_v6
    src/nstd/net/ip/basic_endpoint
    src/nstd/net/ip/tcp
    src/nstd/net/ip/types
    src/nstd/net/ip/make_address_v4
    src/nstd/net/ip/v4_mapped
    src/nstd/net/basic_socket
    src/nstd/net/basic_socket_acceptor
    src/nstd/net/basic_stream_socket
    src/nstd/net/io_context
    src/nstd/net/netfwd
    src/nstd/net/socket_base
    src/nstd/net/timer
)

list(APPEND kuhllib_new_components
    src/nstd/new/new
)

list(APPEND kuhllib_operators_components
    src/nstd/operators/equality
    src/nstd/operators/relational
)

list(APPEND kuhllib_xprojection_components
    src/nstd/projection/identity
    src/nstd/projection/model_readable
    src/nstd/projection/model_value
)

list(APPEND kuhllib_stop_token_components
    src/nstd/stop_token/in_place_stop_token
    src/nstd/stop_token/never_stop_token
    src/nstd/stop_token/stoppable_token
    src/nstd/stop_token/stoppable_token_for
    src/nstd/stop_token/unstoppable_token
)

list(APPEND kuhllib_xthread_components
    src/nstd/thread/sync_wait
)

list(APPEND kuhllib_type_traits_components
    src/nstd/type_traits/add_const
    src/nstd/type_traits/add_lvalue_reference
    src/nstd/type_traits/add_rvalue_reference
    src/nstd/type_traits/choose_type
    src/nstd/type_traits/conditional
    src/nstd/type_traits/decay
    src/nstd/type_traits/declval
    src/nstd/type_traits/enable_if
    src/nstd/type_traits/integral_constant
    src/nstd/type_traits/is_array
    src/nstd/type_traits/is_assignable
    src/nstd/type_traits/is_constructible
    src/nstd/type_traits/is_convertible
    src/nstd/type_traits/is_function
    src/nstd/type_traits/is_member_function_pointer
    src/nstd/type_traits/is_member_object_pointer
    src/nstd/type_traits/is_member_pointer
    src/nstd/type_traits/is_move_assignable
    src/nstd/type_traits/is_nothrow_constructible
    src/nstd/type_traits/is_nothrow_move_assignable
    src/nstd/type_traits/is_nothrow_move_constructible
    src/nstd/type_traits/is_same
    src/nstd/type_traits/is_void
    src/nstd/type_traits/remove_const
    src/nstd/type_traits/remove_cv
    src/nstd/type_traits/remove_cvref
    src/nstd/type_traits/remove_extent
    src/nstd/type_traits/remove_reference
    src/nstd/type_traits/remove_volatile
    src/nstd/type_traits/result_of
    src/nstd/type_traits/type_identity
)

list(APPEND kuhllib_utility_components
    src/nstd/utility/as_const
    src/nstd/utility/equality_comparable
    src/nstd/utility/forward
    src/nstd/utility/integer_sequence
    src/nstd/utility/move
    src/nstd/utility/pair
    src/nstd/utility/swap
)

list(APPEND kuhllib_components
    ${kuhllib_algorithm_components}
    ${kuhllib_atomic_components}
    ${kuhllib_buffer_components}
    ${kuhllib_concepts_components}
    ${kuhllib_cursor_components}
    ${kuhllib_endian_components}
    ${kuhllib_execution_components}
    ${kuhllib_file_components}
    ${kuhllib_functional_components}
    ${kuhllib_hidden_components}
    ${kuhllib_iterator_components}
    ${kuhllib_memory_components}
    ${kuhllib_net_components}
    ${kuhllib_new_components}
    ${kuhllib_operators_components}
    ${kuhllib_projection_components}
    ${kuhllib_stop_token_components}
    ${kuhllib_thread_components}
    ${kuhllib_type_traits_components}
    ${kuhllib_utility_components}
    )

list(TRANSFORM kuhllib_test_components APPEND ${KUHLLIB_SOURCE_SUFFIX} OUTPUT_VARIABLE kuhllib_test_libfiles)
list(TRANSFORM kuhllib_test_components APPEND ${KUHLLIB_TEST_SUFFIX}   OUTPUT_VARIABLE kuhllib_test_tstfiles)
add_library(kuhltest STATIC ${kuhllib_test_libfiles})
add_executable(kuhltest_test ${kuhllib_test_tstfiles})
target_link_libraries(kuhltest_test PUBLIC kuhltest)
add_test(NAME kuhltest_test COMMAND $<TARGET_FILE:kuhltest_test>)

list(TRANSFORM kuhllib_components APPEND ${KUHLLIB_SOURCE_SUFFIX} OUTPUT_VARIABLE kuhllib_libfiles)
list(TRANSFORM kuhllib_components APPEND ${KUHLLIB_TEST_SUFFIX}   OUTPUT_VARIABLE kuhllib_tstfiles)
add_library(kuhllib STATIC ${kuhllib_libfiles})
add_executable(kuhllib_test ${kuhllib_tstfiles})
target_link_libraries(kuhllib_test PUBLIC kuhllib kuhltest pthread)
add_test(NAME kuhllib_test COMMAND $<TARGET_FILE:kuhllib_test>)


list(APPEND kuhllib_disabled_examples
    audio
)
list(APPEND kuhllib_examples
    es
    cs2
    coroutine_echo_server
    simple_echo_server
    chat_server
    echo_server
    audio
)
#foreach(example ${kuhllib_examples})
#    add_executable(${example} src/examples/${example}.cpp)
#    target_link_libraries(${example} PUBLIC kuhllib pthread)
#endforeach()
