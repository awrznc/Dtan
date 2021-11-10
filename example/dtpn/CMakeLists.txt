cmake_minimum_required(VERSION 3.17)
project(dtan_search_project C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set_target_properties(dtan_library PROPERTIES OUTPUT_NAME dtan)
add_executable(dtansp ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)

if(WIN32)
  add_custom_command(
    TARGET dtansp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/bin/dtpn.bat ${CMAKE_BINARY_DIR}/bin/dtpn.bat
  )
elseif(UNIX)
  add_custom_command(
    TARGET dtansp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/bin/dtpn ${CMAKE_BINARY_DIR}/bin/dtpn
  )
elseif(APPLE)
  add_custom_command(
    TARGET dtansp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/bin/dtpn ${CMAKE_BINARY_DIR}/bin/dtpn
  )
endif()
