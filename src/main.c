/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#include "mrubyc.h"

#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

extern const uint8_t sample1[];
extern const uint8_t sample2[];

#define MEMORY_SIZE (1024 * 20)
static uint8_t memory_pool[MEMORY_SIZE];

static void c_sw1_read(mrb_vm *vm, mrb_value *v, int argc) {
  SET_INT_RETURN(1);
}

static void c_led1_write(mrb_vm *vm, mrb_value *v, int argc) {
  gpio_pin_set_dt(&led, GET_INT_ARG(1));
}

int main(void) {
  int ret;

  mrbc_init(memory_pool, MEMORY_SIZE);

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
