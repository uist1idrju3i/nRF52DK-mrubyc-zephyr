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

/* ===== prototype ===== */
extern const uint8_t sample1[];
extern const uint8_t sample2[];
static void c_sw1_read(mrb_vm *vm, mrb_value *v, int argc);
static void c_led1_write(mrb_vm *vm, mrb_value *v, int argc);

/* ===== main ===== */
int main(void) {
  int ret;

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
