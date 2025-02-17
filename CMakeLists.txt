set(PROJECT fhx_manager)
cmake_minimum_required(VERSION 3.5.0)
project(${PROJECT} VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE sources src/*.cpp)
file(GLOB_RECURSE headers include/*.h)

add_executable(${PROJECT} ${sources})

target_include_directories(${PROJECT} 
   PUBLIC 
      include
   PRIVATE
      "../../build/lirc-0.9.4c/debian/tmp/usr/include"
   )

add_library(liblirc_client STATIC IMPORTED)

set_target_properties(liblirc_client PROPERTIES
   IMPORTED_LOCATION "../../build/lirc-0.9.4c/debian/tmp/usr/lib/arm-linux-gnueabihf/liblirc_client.so")

target_link_libraries(${PROJECT} 
   PUBLIC 
      stdc++fs 
      wiringPi 
      pthread 
      m 
      curl 
      liblirc_client)

add_custom_command(
   TARGET ${PROJECT} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy
      fhx_manager
      ../../fhx_manager/fhx_manager)
