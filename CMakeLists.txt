# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(nRF52DK-mrubyc-zephyr)

target_sources(app PRIVATE
                    src/main.c
                    src/sample1.c
                    src/sample2.c
                    heatshrink/heatshrink_encoder.c
                    heatshrink/heatshrink_decoder.c
                    src/lib/mrubyc/hal.c
                    mrubyc/src/alloc.c
                    mrubyc/src/c_array.c
                    mrubyc/src/c_hash.c
                    mrubyc/src/c_math.c
                    mrubyc/src/c_numeric.c
                    mrubyc/src/c_object.c
                    mrubyc/src/c_range.c
                    mrubyc/src/c_string.c
                    mrubyc/src/class.c
                    mrubyc/src/console.c
                    mrubyc/src/error.c
                    mrubyc/src/global.c
                    mrubyc/src/keyvalue.c
                    mrubyc/src/load.c
                    mrubyc/src/mrblib.c
                    mrubyc/src/rrt0.c
                    mrubyc/src/symbol.c
                    mrubyc/src/value.c
                    mrubyc/src/vm.c)

target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/mrubyc ${CMAKE_CURRENT_SOURCE_DIR}/mrubyc/src)
