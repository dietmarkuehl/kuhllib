cmake_minimum_required(VERSION 3.0.0)
project(kuhllib VERSION 0.1.0)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Werror -std=c++2a")
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Werror -std=c++20")
endif()

include_directories(src)
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

list(APPEND temporary_removed
)

list(APPEND kuhllib_components
    src/nstd/algorithm/all_of
    src/nstd/algorithm/any_of
    src/nstd/algorithm/distance
    src/nstd/algorithm/find
    src/nstd/algorithm/find_end
    src/nstd/algorithm/find_if
    src/nstd/algorithm/find_if_not
    src/nstd/algorithm/for_each
    src/nstd/algorithm/mismatch
    src/nstd/algorithm/none_of
    src/nstd/algorithm/search
    src/nstd/atomic/atomic
    src/nstd/cursor/array
    src/nstd/cursor/category
    src/nstd/cursor/model_key
    src/nstd/cursor/model_single_pass
    src/nstd/cursor/single_pass
    src/nstd/endian/big_to_native
    src/nstd/endian/literals
    src/nstd/endian/type
    src/nstd/execution
    src/nstd/execution/connect
    src/nstd/execution/execution
    src/nstd/execution/is_execution_policy
    src/nstd/execution/movable_value
    src/nstd/execution/operation_state
    src/nstd/execution/parallel_policy
    src/nstd/execution/parallel_unsequenced_policy
    src/nstd/execution/receiver
    src/nstd/execution/receiver_of
    src/nstd/execution/schedule
    src/nstd/execution/scheduler
    src/nstd/execution/sender
    src/nstd/execution/sender_base
    src/nstd/execution/sender_traits
    src/nstd/execution/sequenced_policy
    src/nstd/execution/set_done
    src/nstd/execution/set_error
    src/nstd/execution/set_value
    src/nstd/execution/start
    src/nstd/execution/sync_wait
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
    src/nstd/file/descriptor
    src/nstd/file/mapped_memory
    src/nstd/file/open
    src/nstd/file/open_flags
    src/nstd/file/read
    src/nstd/file/ring
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
    src/nstd/iterator/bidirectional_iterator
    src/nstd/iterator/bidirectional_iterator_tag
    src/nstd/iterator/forward_iterator
    src/nstd/iterator/forward_iterator_tag
    src/nstd/iterator/iterator_traits
    src/nstd/memory/addressof
    src/nstd/memory/uninitialized_default_construct
    src/nstd/net/io_context
    src/nstd/net/netfwd
    src/nstd/new/new
    src/nstd/operators/equality
    src/nstd/operators/relational
    src/nstd/projection/identity
    src/nstd/projection/model_readable
    src/nstd/projection/model_value
    src/nstd/sender/just
    src/nstd/sender/just_done
    src/nstd/sender/just_error
    src/nstd/sender/then
    src/nstd/type_traits/add_lvalue_reference
    src/nstd/type_traits/add_rvalue_reference
    src/nstd/type_traits/choose_type
    src/nstd/type_traits/condition
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
    src/nstd/utility/equality_comparable
    src/nstd/utility/forward
    src/nstd/utility/integer_sequence
    src/nstd/utility/move
    src/nstd/utility/pair
    src/nstd/utility/swap
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
