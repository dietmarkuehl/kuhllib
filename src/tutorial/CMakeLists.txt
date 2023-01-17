cmake_minimum_required(VERSION 3.12.0)
project(tutorial VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

# set(tutorial_sanitizer address)
# set(tutorial_sanitizer memory)
# set(tutorial_sanitizer thread)
set(tutorial_sanitizer undefined)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-g -W -Wall -Werror)
    add_compile_options(-fsanitize=${tutorial_sanitizer})
    add_compile_options(-fno-sanitize-recover)
    add_link_options(-fsanitize=${tutorial_sanitizer})
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-g -W -Wall -Werror)
    add_compile_options(-stdlib=libc++)
    add_compile_options(-fsanitize=${tutorial_sanitizer})
    add_compile_options(-fno-sanitize-recover)
    add_link_options(-fsanitize=${tutorial_sanitizer})
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(-DNOMINMAX)
endif()


list(APPEND TUTORIAL_EXAMPLES
    getting-started-1
    getting-started-2
    getting-started-3
)

foreach(example ${TUTORIAL_EXAMPLES})
    add_executable(${example} ${example}.cpp)
endforeach()
