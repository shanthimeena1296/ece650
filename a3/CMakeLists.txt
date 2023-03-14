cmake_minimum_required(VERSION 2.8...3.13)
project(ece650-a3)

## ECE650 Specific configuration start here
## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/run)

option(WITH_ASAN "Enabled AddressSanitizer to detect memory faults" OFF)
if(WITH_ASAN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined" )
endif()


# create the main executable
## add additional .cpp files if needed
add_executable(ece650-a2 ece650-a2.cpp )
add_executable(ece650-a3 ece650-a3.cpp )
add_executable(rgen rgen.cpp )

# create the executable for tests
add_executable(tests test.cpp)
install(PROGRAMS ece650-a1.py DESTINATION bin)
install(TARGETS ece650-a2 ece650-a3 rgen RUNTIME DESTINATION bin)
