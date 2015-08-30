cmake_minimum_required(VERSION 3.0.0)
project(Decimal)
add_definitions(-std=c++11) #-dk:TODO should really be target_compile_features(decimal PRIVATE cxx_constexpr)

add_library(decimal
        decimal
        decimal_num_put
        uint_t
)

enable_testing()
foreach(test
        decimal.t
        decimal-class.t
        decimal_base.t 
        decimal_config.t 
        decimal-literals.t 
        decimal_num_put.t 
        uint_t.t 
        )
  add_executable(${test} ${test})
  target_link_libraries(${test} PUBLIC decimal)
  target_link_libraries(${test} PUBLIC kuhltest)
  add_test(NAME ${test} COMMAND $<TARGET_FILE:${test}>)
endforeach(test)
