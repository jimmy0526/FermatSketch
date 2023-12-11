cmake_minimum_required(VERSION 3.10)
project(Fermat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

file(GLOB_RECURSE SOURCES "src/*.cc")

include_directories(include)

add_executable(Fermat ${SOURCES})

find_package(Threads REQUIRED)
find_library(LIB_PCAP pcap)
find_library(LIB_NET net)

if(NOT LIB_PCAP)
  message(FATAL_ERROR "libpcap not found")
endif()

if(NOT LIB_NET)
  message(FATAL_ERROR "libnet not found")
endif()

target_link_libraries(Fermat ${CMAKE_THREAD_LIBS_INIT} ${LIB_PCAP} ${LIB_NET})
