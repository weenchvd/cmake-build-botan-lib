
# Copyright (c) 2022 Vitaly Dikov
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required (VERSION 3.20)

if(WIN32)
  if(NOT (${CMAKE_BUILD_TYPE} STREQUAL "Release"))
    message(FATAL_ERROR "This configuration only works for a Release build")
  endif()

  # set paths
  set(BOTAN_LIB_ROOT_DIR    "${CMAKE_SOURCE_DIR}/external/botan")
  set(BOTAN_LIB_REPOS_DIR   "${BOTAN_LIB_ROOT_DIR}/repos")
  set(BOTAN_LIB_FCSTUFF_DIR "${BOTAN_LIB_ROOT_DIR}/cmake-fetchcontent-stuff")
  set(BOTAN_LIB_INSTALL_DIR "${BOTAN_LIB_ROOT_DIR}-install")

  # download and unpack Botan library
  include(FetchContent)
  FetchContent_Declare(
    botan
    GIT_REPOSITORY  https://github.com/randombit/botan.git
    GIT_TAG         2.19.1
    PREFIX          ${BOTAN_LIB_FCSTUFF_DIR}
    SOURCE_DIR      ${BOTAN_LIB_REPOS_DIR}
  )
  set(FETCHCONTENT_QUIET OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(botan)

  # find Python3 Interpreter and run build, testing and installation
  if(${botan_POPULATED} AND MSVC AND NOT EXISTS "${BOTAN_LIB_INSTALL_DIR}/lib/botan.lib")
    find_package(Python3 COMPONENTS Interpreter)
    if(NOT ${Python3_Interpreter_FOUND})
      message(FATAL_ERROR "Python3 Interpreter NOT FOUND")
    endif()

    execute_process(
      COMMAND           ${Python3_EXECUTABLE} configure.py --cc=msvc --os=windows --prefix=${BOTAN_LIB_INSTALL_DIR}
      WORKING_DIRECTORY ${BOTAN_LIB_REPOS_DIR}
      COMMAND_ECHO      STDOUT
    )
    execute_process(
      COMMAND           nmake
      WORKING_DIRECTORY ${BOTAN_LIB_REPOS_DIR}
      COMMAND_ECHO      STDOUT
    )
    execute_process(
      COMMAND           nmake check
      WORKING_DIRECTORY ${BOTAN_LIB_REPOS_DIR}
      COMMAND_ECHO      STDOUT
    )
    execute_process(
      COMMAND           nmake install
      WORKING_DIRECTORY ${BOTAN_LIB_REPOS_DIR}
      COMMAND_ECHO      STDOUT
    )
  endif()
endif()


add_executable (main "../main.cpp")

if(WIN32)
  target_include_directories(main PUBLIC "${BOTAN_LIB_INSTALL_DIR}/include/botan-2")
  target_link_libraries(main PUBLIC "${BOTAN_LIB_INSTALL_DIR}/lib/botan.lib")

  configure_file("${BOTAN_LIB_INSTALL_DIR}/bin/botan.dll"
                 "${CMAKE_CURRENT_BINARY_DIR}/botan.dll"
                 COPYONLY
  )

  install(TARGETS main DESTINATION bin)
  install(FILES "${BOTAN_LIB_INSTALL_DIR}/bin/botan.dll" DESTINATION bin)
endif()
