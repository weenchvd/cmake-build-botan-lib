
# Copyright (c) 2022 Vitaly Dikov
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required (VERSION 3.20)

option(BUILD_BOTAN_LIB "Download and build Botan library" ON)
option(BUILD_STATIC_BOTAN_LIB "Build Botan as a static library" OFF)

set(EXTERNAL_PROJECTS_DIR "${CMAKE_SOURCE_DIR}/external")

if(${BUILD_BOTAN_LIB} AND WIN32)
  cmake_path(GET CMAKE_BINARY_DIR FILENAME CONFIG_NAME)

  # set paths
  set(BOTAN_LIB_ROOT_DIR    "${EXTERNAL_PROJECTS_DIR}/botan")
  set(BOTAN_LIB_REPOS_DIR   "${BOTAN_LIB_ROOT_DIR}/repos")
  set(BOTAN_LIB_FCSTUFF_DIR "${BOTAN_LIB_ROOT_DIR}/cmake-fetchcontent-stuff")
  if (CONFIG_NAME)
    set(BOTAN_LIB_INSTALL_DIR "${BOTAN_LIB_ROOT_DIR}-install/${CONFIG_NAME}")
  else()
    set(BOTAN_LIB_INSTALL_DIR "${BOTAN_LIB_ROOT_DIR}-install")
  endif()

  set(BOTAN_LIB_INCLUDE_DIR "${BOTAN_LIB_INSTALL_DIR}/include")
  set(BOTAN_LIB_BIN_DIR     "${BOTAN_LIB_INSTALL_DIR}/bin")
  set(BOTAN_LIB_LIB_DIR     "${BOTAN_LIB_INSTALL_DIR}/lib")

  # set script options and commands for the build
  set(BOTAN_LIB_CONFIGURE_PY_ARGS         "--prefix=${BOTAN_LIB_INSTALL_DIR}")
  list(APPEND BOTAN_LIB_CONFIGURE_PY_ARGS "--includedir=${BOTAN_LIB_INCLUDE_DIR}")
  list(APPEND BOTAN_LIB_CONFIGURE_PY_ARGS "--bindir=${BOTAN_LIB_BIN_DIR}")
  list(APPEND BOTAN_LIB_CONFIGURE_PY_ARGS "--libdir=${BOTAN_LIB_LIB_DIR}")

  if(${BUILD_STATIC_BOTAN_LIB})
    list(PREPEND BOTAN_LIB_CONFIGURE_PY_ARGS "--disable-shared-library")
  else()
    list(PREPEND BOTAN_LIB_CONFIGURE_PY_ARGS "--disable-static-library")
  endif()
  if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    list(PREPEND BOTAN_LIB_CONFIGURE_PY_ARGS "--debug-mode")
  endif()
  if(MSVC)
    list(PREPEND BOTAN_LIB_CONFIGURE_PY_ARGS "--cc=msvc;--os=windows")
    set(BOTAN_LIB_MAKE_COMMAND    "nmake")
    set(BOTAN_LIB_TEST_COMMAND    "nmake;check")
    set(BOTAN_LIB_INSTALL_COMMAND "nmake;install")
  else()
    set(BOTAN_LIB_MAKE_COMMAND    " ")
    set(BOTAN_LIB_TEST_COMMAND    " ")
    set(BOTAN_LIB_INSTALL_COMMAND " ")
  endif()

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
  if(${botan_POPULATED} AND NOT EXISTS "${BOTAN_LIB_LIB_DIR}/botan.lib")
    find_package(Python3 COMPONENTS Interpreter)
    if(NOT ${Python3_Interpreter_FOUND})
      message(FATAL_ERROR "Python3 Interpreter NOT FOUND")
    endif()

    execute_process(
      COMMAND           ${Python3_EXECUTABLE} configure.py ${BOTAN_LIB_CONFIGURE_PY_ARGS}
      WORKING_DIRECTORY ${BOTAN_LIB_REPOS_DIR}
      COMMAND_ECHO      STDOUT
    )
    execute_process(
      COMMAND           ${BOTAN_LIB_MAKE_COMMAND}
      WORKING_DIRECTORY ${BOTAN_LIB_REPOS_DIR}
      COMMAND_ECHO      STDOUT
    )
    execute_process(
      COMMAND           ${BOTAN_LIB_TEST_COMMAND}
      WORKING_DIRECTORY ${BOTAN_LIB_REPOS_DIR}
      COMMAND_ECHO      STDOUT
    )
    execute_process(
      COMMAND           ${BOTAN_LIB_INSTALL_COMMAND}
      WORKING_DIRECTORY ${BOTAN_LIB_REPOS_DIR}
      COMMAND_ECHO      STDOUT
    )
  endif()
endif()


add_executable (main "main.cpp")

if(${BUILD_BOTAN_LIB} AND WIN32)
  target_include_directories(main PUBLIC "${BOTAN_LIB_INCLUDE_DIR}/botan-2")
  target_link_libraries(main PUBLIC "${BOTAN_LIB_LIB_DIR}/botan.lib")

  if(NOT ${BUILD_STATIC_BOTAN_LIB})
    configure_file("${BOTAN_LIB_BIN_DIR}/botan.dll" "${CMAKE_CURRENT_BINARY_DIR}/botan.dll" COPYONLY)
    install(FILES "${BOTAN_LIB_BIN_DIR}/botan.dll" DESTINATION bin)
  endif()

  install(TARGETS main DESTINATION bin)
  install(FILES "${BOTAN_LIB_REPOS_DIR}/license.txt" DESTINATION bin RENAME "botan_license.txt")
endif()
