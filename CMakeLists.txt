  cmake_minimum_required(VERSION 3.10)
   
  project(main)
   
  add_library(lib STATIC bigdigits.c bigdigits.h)
   
  add_executable(main main.c)
   
  target_link_libraries(main lib)
