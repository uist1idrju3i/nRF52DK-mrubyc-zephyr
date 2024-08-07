/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#include "mrubyc.h"

/* ===== miniz ===== */
#define MINIZ_NO_STDIO
#define MINIZ_NO_TIME
#define MINIZ_NO_ARCHIVE_APIS
#define MINIZ_NO_ARCHIVE_WRITING_APIS
#define MINIZ_NO_ZLIB_APIS
#define MINIZ_NO_ZLIB_COMPATIBLE_NAMES
#define MINIZ_NO_MALLOC
#include "lib/miniz/miniz.h"
/* ================= */

#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

tdefl_compressor g_deflator;

/* ===== prototype ===== */
extern const uint8_t sample1[];
extern const uint8_t sample2[];
static void c_sw1_read(mrb_vm *vm, mrb_value *v, int argc);
static void c_led1_write(mrb_vm *vm, mrb_value *v, int argc);
static int bytecode_compress(void);
static int bytecode_decompress(void);

/* ===== main ===== */
int main(void) {
  int ret;
  bytecode_compress();
  if (!gpio_is_ready_dt(&led)) {
    return 0;
  }
  ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
  if (ret < 0) {
    return 0;
  }

  mrbc_define_method(0, mrbc_class_object, "sw1_read", c_sw1_read);
  mrbc_define_method(0, mrbc_class_object, "led1_write", c_led1_write);

  mrbc_create_task(sample1, 0);
  //  mrbc_create_task(sample2, 0);

  mrbc_run();

  return 0;
}

/* ===== c_sw1_read ===== */
static void c_sw1_read(mrb_vm *vm, mrb_value *v, int argc) {
  SET_INT_RETURN(1);  // DUMMY
}

/* ===== c_led1_write ===== */
static void c_led1_write(mrb_vm *vm, mrb_value *v, int argc) {
  gpio_pin_set_dt(&led, GET_INT_ARG(1));
}

/* ===== bytecode_compress ===== */
static int bytecode_compress(void) {
  tdefl_status status;
  const mz_uint kCompFlags =
      TDEFL_WRITE_ZLIB_HEADER | 1500 | TDEFL_GREEDY_PARSING_FLAG;
  if (TDEFL_STATUS_OKAY != tdefl_init(&g_deflator, NULL, NULL, kCompFlags)) {
    return EXIT_FAILURE;
  }
}

/* ===== bytecode_decompress ===== */
static int bytecode_decompress(void);
