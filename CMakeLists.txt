cmake_minimum_required(VERSION 3.17)
project(dtan_project C)

add_executable(dtan ${CMAKE_CURRENT_SOURCE_DIR}/src/dtan.c)

target_compile_options(dtan PUBLIC
  $<$<C_COMPILER_ID:Windows>:/link /OUT:dtan.exe /SUBSYSTEM:WINDOWS /entry:mainCRTStartup>
  $<$<C_COMPILER_ID:GNU>:-Wl,-subsystem,windows>
  $<$<C_COMPILER_ID:Clang>:-Wl,-subsystem,windows>
)
