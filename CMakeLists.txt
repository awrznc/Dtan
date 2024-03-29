cmake_minimum_required(VERSION 3.10)
project(dtan_project C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_executable(dtan_application ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)
SET_TARGET_PROPERTIES(dtan_application PROPERTIES OUTPUT_NAME dtan)

if(WIN32)
  target_compile_options(dtan_application PUBLIC
    $<$<C_COMPILER_ID:Windows>:/link /OUT:dtan.exe /SUBSYSTEM:WINDOWS /entry:mainCRTStartup>
    $<$<C_COMPILER_ID:GNU>:-Wl,-subsystem,windows>
    $<$<C_COMPILER_ID:Clang>:-Wl,-subsystem,windows>
  )
  add_library(dtan_library STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/dtan_windows.c ${CMAKE_CURRENT_SOURCE_DIR}/src/dtan.c)
  SET_TARGET_PROPERTIES(dtan_library PROPERTIES OUTPUT_NAME dtan)
  target_link_libraries(dtan_application PRIVATE dtan_library)

elseif(UNIX AND NOT APPLE)
  find_package(X11 REQUIRED)
  add_library(dtan_library STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/dtan_linux.c ${CMAKE_CURRENT_SOURCE_DIR}/src/dtan.c)
  SET_TARGET_PROPERTIES(dtan_library PROPERTIES OUTPUT_NAME dtan)
  target_include_directories(dtan_library PRIVATE "/usr/include/freetype2")
  target_link_libraries(dtan_library PRIVATE X11 Xrender Xft m)
  target_link_libraries(dtan_application PRIVATE dtan_library)

elseif(APPLE)
  target_compile_options(dtan_application PUBLIC
    $<$<C_COMPILER_ID:AppleClang>:-fobjc-arc -x objective-c>
  )
  add_library(dtan_library STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/dtan_macos.m ${CMAKE_CURRENT_SOURCE_DIR}/src/dtan.c)
  SET_TARGET_PROPERTIES(dtan_library PROPERTIES OUTPUT_NAME dtan)
  target_link_libraries(dtan_library PRIVATE "-framework Cocoa")
  target_link_libraries(dtan_application PRIVATE dtan_library)

endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/example/dtpn)
