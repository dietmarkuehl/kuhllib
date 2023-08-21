cmake_minimum_required(VERSION 3.12.0)
project(toy-networking VERSION 1.0.0)
include(CheckIncludeFile)
include(CheckCXXSourceCompiles)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_COMPILER_IS_GNUCXX)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -W -Wall -Werror -Wno-error=subobject-linkage -Wno-subobject-linkage -Wno-missing-field-initializers -fconcepts-diagnostics-depth=4 -ftemplate-backtrace-limit=0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wall -Werror -Wno-error=subobject-linkage -Wno-subobject-linkage -Wno-missing-field-initializers -fconcepts-diagnostics-depth=4 -ftemplate-backtrace-limit=0")
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
    toy-echo
    toy-udp-client
    toy-udp-server
    toy-http-client
    toy-cancel
    toy-sender
    toy-time
)

set(CMAKE_REQUIRED_INCLUDES "/usr/local/include")
CHECK_INCLUDE_FILE("ares.h" TOY_HAS_ARES)
if (TOY_HAS_ARES)
    include_directories("/usr/local/include")
    link_directories("/usr/local/lib")
    set(TOY_LIB_CARES cares)

    list(APPEND TOY_DEMOS toy-resolve)
    add_compile_definitions(TOY_HAS_ARES)
endif()

CHECK_INCLUDE_FILE("arpa/inet.h" TOY_HAS_ARPA_INET)
if (TOY_HAS_ARPA_INET)
    add_compile_definitions(TOY_HAS_ARPA_INET)
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
CHECK_INCLUDE_FILE("sys/fcntl.h" TOY_HAS_SYS_FCNTL)
if (TOY_HAS_SYS_FCNTL)
    add_compile_definitions(TOY_HAS_SYS_FCNTL)
endif()
CHECK_INCLUDE_FILE("unistd.h" TOY_HAS_UNISTD)
if (TOY_HAS_UNISTD)
    add_compile_definitions(TOY_HAS_UNISTD)
endif()
CHECK_INCLUDE_FILE("winsock2.h" TOY_HAS_WINSOCK2)
if (TOY_HAS_WINSOCK2)
    add_compile_definitions(TOY_HAS_WINSOCK2)
endif()
CHECK_INCLUDE_FILE(sys/event.h TOY_HAS_KQUEUE)
if (TOY_HAS_KQUEUE)
    add_compile_definitions(TOY_HAS_KQUEUE)
    CHECK_CXX_SOURCE_COMPILES("
    #include <sys/event.h>
    #include <stdint.h>
    int main() {
        struct kevent ev{::uintptr_t(), ::int16_t(), EV_DELETE, ::uint32_t(), ::intptr_t(), nullptr, {}};
        (void)ev;
    }
    " TOY_KEVENT_HAS_EXT)
    if (TOY_KEVENT_HAS_EXT)
        add_compile_definitions(TOY_KEVENT_HAS_EXT)
    endif()
endif()

# try to use the POSIX interface
if(TOY_HAS_UNISTD)
    foreach(demo ${TOY_DEMOS})
        set(demo_executable ${demo}-${CMAKE_CXX_COMPILER_ID}-poll)
        add_executable(${demo_executable} ${demo}.cpp)
        target_compile_definitions(${demo_executable} PRIVATE TOY_CONTEXT=toy::poll::context)
        target_link_libraries(${demo_executable} ${TOY_LIB_CARES})
    endforeach()

    add_executable(toy-cppcon-2022-${CMAKE_CXX_COMPILER_ID} toy-cppcon-2022.cpp)
endif()

# use a generic context
foreach(demo ${TOY_DEMOS})
    set(demo_executable ${demo}-${CMAKE_CXX_COMPILER_ID}-generic)
    add_executable(${demo_executable} ${demo}.cpp)
        target_compile_definitions(${demo_executable} PRIVATE TOY_CONTEXT=toy::generic::context)
    target_link_libraries(${demo_executable} ${TOY_LIB_CARES})
endforeach()

# try to use liburing.h
CHECK_INCLUDE_FILE(liburing.h TOY_HAS_LIBURING)
IF(TOY_HAS_LIBURING)
    add_compile_definitions(TOY_HAS_IO_URING)
    foreach(demo ${TOY_DEMOS})
        set(demo_executable ${demo}-${CMAKE_CXX_COMPILER_ID}-io_uring)
        add_executable(${demo_executable} ${demo}.cpp)
        target_compile_definitions(${demo_executable} PRIVATE TOY_NETWORKING_HPP="toy-networking-io_uring.hpp")
        target_compile_definitions(${demo_executable} PRIVATE TOY_CONTEXT=toy::io_uring::context)
        target_link_libraries(${demo_executable} uring ${TOY_LIB_CARES})
    endforeach()
ENDIF()

# try to use sys/epoll.h
CHECK_INCLUDE_FILE(sys/epoll.h TOY_HAS_EPOLL)
IF(TOY_HAS_EPOLL)
    add_compile_definitions(TOY_HAS_EPOLL)
    foreach(demo ${TOY_DEMOS})
        set(demo_executable ${demo}-${CMAKE_CXX_COMPILER_ID}-epoll)
        add_executable(${demo_executable} ${demo}.cpp)
        target_compile_definitions(${demo_executable} PRIVATE TOY_NETWORKING_HPP="toy-networking-epoll.hpp")
        target_compile_definitions(${demo_executable} PRIVATE TOY_CONTEXT=toy::epoll::context)
        target_link_libraries(${demo_executable} ${TOY_LIB_CARES})
    endforeach()
ENDIF()

# try to use sys/event.h
IF(TOY_HAS_KQUEUE)
    foreach(demo ${TOY_DEMOS})
        set(demo_executable ${demo}-${CMAKE_CXX_COMPILER_ID}-kqueue)
        add_executable(${demo_executable} ${demo}.cpp)
        target_compile_definitions(${demo_executable} PRIVATE TOY_NETWORKING_HPP="toy-networking-kqueue.hpp")
        target_compile_definitions(${demo_executable} PRIVATE TOY_CONTEXT=toy::kqueue::context)
        if(TOY_HAS_ARES)
            target_link_libraries(${demo_executable} cares)
        endif()
    endforeach()
ENDIF()

# try to use IOCP
CHECK_INCLUDE_FILE(winsock2.h TOY_HAS_IOCP)
IF(TOY_HAS_IOCP)
    add_compile_definitions(TOY_HAS_IOCP)
    foreach(demo ${TOY_DEMOS})
        set(demo_executable ${demo}-${CMAKE_CXX_COMPILER_ID}-iocp)
        add_executable(${demo_executable} ${demo}.cpp)
        target_compile_definitions(${demo_executable} PRIVATE TOY_NETWORKING_HPP="toy-networking-iocp.hpp")
        target_compile_definitions(${demo_executable} PRIVATE TOY_CONTEXT=toy::iocp::context)
    endforeach()
ENDIF()
