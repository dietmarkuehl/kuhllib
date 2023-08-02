cmake_minimum_required(VERSION 3.12.0)
project(toy-networking VERSION 1.0.0)
include(CheckIncludeFile)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wall -Werror -Wno-missing-field-initializers -fconcepts-diagnostics-depth=4 -ftemplate-backtrace-limit=0")
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wall -Werror -stdlib=libc++")
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wall -Werror -stdlib=libc++ -DTOY_EXPERIMENTAL_COROUTINES")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX")
endif()


# doesn't compile with gcc: toy-coro
list(APPEND TOY_DEMOS
    toy-client
)
list(APPEND xTOY_DEMOS
    toy-cancel
    toy-echo
    toy-sender
    toy-udp-client
    toy-udp-server
    toy-http-client
)

CHECK_INCLUDE_FILE("arpa/inet.h" TOY_HAS_ARPA_INET)
if (TOY_HAS_APRA_INET)
    add_compile_definitions(TOY_HAS_APRA_INET)
endif()
CHECK_INCLUDE_FILE("netinet/in.h" TOY_HAS_NETINET_IN)
if (TOY_HAS_NETINET_IN)
    add_compile_definitions(TOY_HAS_NETINET_IN)
endif()
CHECK_INCLUDE_FILE("poll.h" TOY_HAS_POLL)
if (TOY_HAS_POLL)
    add_compile_definitions(TOY_HAS_POLL)
endif()
CHECK_INCLUDE_FILE("sys/socket.h" TOY_HAS_SYS_SOCKET)
if (TOY_HAS_SYS_SOCKET)
    add_compile_definitions(TOY_HAS_SYS_SOCKET)
endif()
CHECK_INCLUDE_FILE("unistd.h" TOY_HAS_UNISTD)
if (TOY_HAS_UNISTD)
    add_compile_definitions(TOY_HAS_UNISTD)
endif()
CHECK_INCLUDE_FILE("winsock2.h" TOY_HAS_WINSOCK2)
if (TOY_HAS_WINSOCK2)
    add_compile_definitions(TOY_HAS_WINSOCK2)
endif()

# try to use the POSIX interface
CHECK_INCLUDE_FILE(unistd.h TOY_HAS_UNISTD)
if(TOY_HAS_UNISTD)
    foreach(demo ${TOY_DEMOS})
        add_executable(${demo}-${CMAKE_CXX_COMPILER_ID}-poll ${demo}.cpp)
    endforeach()

    add_executable(toy-cppcon-2022-${CMAKE_CXX_COMPILER_ID} toy-cppcon-2022.cpp)
endif()

# try to use liburing.h
CHECK_INCLUDE_FILE(liburing.h TOY_HAS_LIBURING)
IF(TOY_HAS_LIBURING)
    foreach(demo ${TOY_DEMOS})
        set(demo_executable ${demo}-${CMAKE_CXX_COMPILER_ID}-io_uring)
        add_executable(${demo_executable} ${demo}.cpp)
        target_compile_definitions(${demo_executable} PRIVATE TOY_NETWORKING_HPP="toy-networking-io_uring.hpp")
        target_link_libraries(${demo_executable} uring)
    endforeach()
ENDIF()

# try to use sys/epoll.h
CHECK_INCLUDE_FILE(sys/epoll.h TOY_HAS_EPOLL)
IF(TOY_HAS_EPOLL)
    foreach(demo ${TOY_DEMOS})
        set(demo_executable ${demo}-${CMAKE_CXX_COMPILER_ID}-epoll)
        add_executable(${demo_executable} ${demo}.cpp)
        target_compile_definitions(${demo_executable} PRIVATE TOY_NETWORKING_HPP="toy-networking-epoll.hpp")
    endforeach()
ENDIF()

# try to use sys/event.h
CHECK_INCLUDE_FILE(sys/event.h TOY_HAS_KQUEUE)
IF(TOY_HAS_KQUEUE)
    foreach(demo ${TOY_DEMOS})
        set(demo_executable ${demo}-${CMAKE_CXX_COMPILER_ID}-kqueue)
        add_executable(${demo_executable} ${demo}.cpp)
        target_compile_definitions(${demo_executable} PRIVATE TOY_NETWORKING_HPP="toy-networking-kqueue.hpp")
    endforeach()
ENDIF()

# try to use IOCP
CHECK_INCLUDE_FILE(winsock2.h TOY_HAS_IOCP)
IF(TOY_HAS_IOCP)
    add_executable(toy-echo-iocp toy-echo.cpp)
    target_compile_definitions(toy-echo-iocp PRIVATE TOY_NETWORKING_HPP="toy-networking-iocp.hpp")
    add_executable(toy-client-iocp toy-client.cpp)
    target_compile_definitions(toy-client-iocp PRIVATE TOY_NETWORKING_HPP="toy-networking-iocp.hpp")
ENDIF()
