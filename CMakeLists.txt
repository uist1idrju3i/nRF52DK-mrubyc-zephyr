# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(nRF52DK-mrubyc-zephyr)

target_sources(app PRIVATE src/main.c)

add_dependencies(app mrubyc)
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/mrubyc/src)
target_link_libraries(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mrubyc/build/libmrubyc.a)
include(ExternalProject)
ExternalProject_Add(mrubyc
#  GIT_REPOSITORY https://github.com/mrubyc/mrubyc
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mrubyc/src
  CONFIGURE_COMMAND ""
  BUILD_COMMAND MRBC_USE_HAL=${CMAKE_CURRENT_SOURCE_DIR}/src make -C ${CMAKE_CURRENT_SOURCE_DIR}/mrubyc/src
  INSTALL_COMMAND ""
  TEST_COMMAND ""
  BUILD_BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/mrubyc/build/libmrubyc.a
)
