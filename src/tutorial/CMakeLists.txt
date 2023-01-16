cmake_minimum_required(VERSION 3.12.0)
project(tutorial VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

list(APPEND TUTORIAL_EXAMPLES
    getting-started-1
)

foreach(example ${TUTORIAL_EXAMPLES})
    add_executable(${example} ${example}.cpp)
endforeach()
