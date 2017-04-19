#  CMakeLists.txt                                                -*-makefile-*-
#  ----------------------------------------------------------------------------
#   Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
#                                                                        
#   Permission is hereby granted, free of charge, to any person          
#   obtaining a copy of this software and associated documentation       
#   files (the "Software"), to deal in the Software without restriction, 
#   including without limitation the rights to use, copy, modify,        
#   merge, publish, distribute, sublicense, and/or sell copies of        
#   the Software, and to permit persons to whom the Software is          
#   furnished to do so, subject to the following conditions:             
#                                                                        
#   The above copyright notice and this permission notice shall be       
#   included in all copies or substantial portions of the Software.      
#                                                                        
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      
#   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      
#   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND             
#   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT          
#   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,         
#   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING         
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR        
#   OTHER DEALINGS IN THE SOFTWARE. 
#  ----------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8.2)
project(kuhllib)

include_directories(${CMAKE_SOURCE_DIR})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
else()
    add_definitions(-std=c++14)
endif()

SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH "/opt/intel/lib:/opt/intel/compilers_and_libraries_2017.2.163/mac/tbb/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -DKUHLLIB_CLANG")
    include_directories(AFTER STSTEM "/opt/llvm-4.0.0/include")
    link_directories("/opt/llvm-4.0.0/lib")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /DKUHLLIB_MSVC")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -Wall -Werror -DKUHLLIB_GCC")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w2 -Werror -fopenmp -DKUHLLIB_INTEL")
endif()

function(runtests)
    foreach(test ${ARGN})
      add_executable(${test} ${test})
      target_link_libraries(${test} PUBLIC kuhltest nstd-execution tbb)
      add_test(NAME ${test} COMMAND $<TARGET_FILE:${test}>)
      if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_custom_command(TARGET ${test}
          POST_BUILD COMMAND
          ${CMAKE_INSTALL_NAME_TOOL} -change "@rpath/libtbb.dylib" "/opt/gcc-6.3.0/lib/libtbb.dylib"
          $<TARGET_FILE:${test}>)
      elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        add_custom_command(TARGET ${test}
          POST_BUILD COMMAND
          ${CMAKE_INSTALL_NAME_TOOL} -change "@rpath/libiomp5.dylib" "/opt/intel/lib/libiomp5.dylib"
          $<TARGET_FILE:${test}>)
        add_custom_command(TARGET ${test}
          POST_BUILD COMMAND
          ${CMAKE_INSTALL_NAME_TOOL} -change "@rpath/libtbb.dylib" "/opt/intel/compilers_and_libraries_2017.2.163/mac/tbb/lib/libtbb.dylib"
          $<TARGET_FILE:${test}>)
        endif()
    endforeach(test)
endfunction(runtests)
enable_testing()

add_subdirectory(kuhl)
add_subdirectory(nstd)
