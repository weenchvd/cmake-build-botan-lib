
# Copyright (c) 2022 Vitaly Dikov
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required (VERSION 3.20)

project ("CMakeBuildBotanLib")

option(BUILD_MINIMAL "Build Botan library with minimal configuration" OFF)

if(${BUILD_MINIMAL})
  add_subdirectory ("src/minimal")
else()
  add_subdirectory ("src")
endif()
