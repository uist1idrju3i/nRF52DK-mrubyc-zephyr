/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "../heatshrink/heatshrink_common.h"
#include "../heatshrink/heatshrink_config.h"
#include "../heatshrink/heatshrink_decoder.h"
#include "../heatshrink/heatshrink_encoder.h"
#include "mrubyc.h"

#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

/* ===== prototype ===== */
extern const uint8_t sample1[];
extern const uint8_t sample2[];
static void c_sw1_read(mrb_vm *vm, mrb_value *v, int argc);
static void c_led1_write(mrb_vm *vm, mrb_value *v, int argc);
static int compress_test(void);

/* ===== main ===== */
int main(void) {
  int ret;

  compress_test();

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

const uint8_t kTestBytecode[] = {
    0x52, 0x49, 0x54, 0x45, 0x30, 0x33, 0x30, 0x30, 0x00, 0x00, 0x00, 0xa6,
    0x4d, 0x41, 0x54, 0x5a, 0x30, 0x30, 0x30, 0x30, 0x49, 0x52, 0x45, 0x50,
    0x00, 0x00, 0x00, 0x77, 0x30, 0x33, 0x30, 0x30, 0x00, 0x00, 0x00, 0x6b,
    0x00, 0x02, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37,
    0x06, 0x01, 0x01, 0x02, 0x01, 0x07, 0x03, 0x42, 0x02, 0x27, 0x02, 0x00,
    0x05, 0x06, 0x01, 0x25, 0x00, 0x02, 0x07, 0x01, 0x2d, 0x02, 0x00, 0x00,
    0x06, 0x03, 0x42, 0x02, 0x27, 0x02, 0x00, 0x02, 0x07, 0x01, 0x01, 0x03,
    0x01, 0x2d, 0x02, 0x01, 0x01, 0x07, 0x03, 0x2d, 0x02, 0x02, 0x01, 0x25,
    0xff, 0xd0, 0x11, 0x02, 0x38, 0x02, 0x69, 0x00, 0x00, 0x00, 0x03, 0x00,
    0x08, 0x73, 0x77, 0x31, 0x5f, 0x72, 0x65, 0x61, 0x64, 0x00, 0x00, 0x0a,
    0x6c, 0x65, 0x64, 0x31, 0x5f, 0x77, 0x72, 0x69, 0x74, 0x65, 0x00, 0x00,
    0x05, 0x73, 0x6c, 0x65, 0x65, 0x70, 0x00, 0x4c, 0x56, 0x41, 0x52, 0x00,
    0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x6c, 0x65, 0x64,
    0x00, 0x00, 0x45, 0x4e, 0x44, 0x00, 0x00, 0x00, 0x00, 0x08,
};

static int compress_test(void) {
  heatshrink_encoder *hse = heatshrink_encoder_alloc(12, 6);
  //  heatshrink_decoder *hsd = heatshrink_decoder_alloc(256, 8, 3);
  uint8_t *input = (uint8_t *)kTestBytecode;
  size_t input_size = sizeof(kTestBytecode) / sizeof(kTestBytecode[0]);

  size_t comp_sz = input_size + (input_size / 2) + 4;
  //  size_t decomp_sz = input_size + (input_size / 2) + 4;
  uint8_t *comp = malloc(comp_sz);
  //  uint8_t *decomp = malloc(decomp_sz);
  memset(comp, 0, comp_sz);
  //  memset(decomp, 0, decomp_sz);

  size_t count = 0;

  size_t sunk = 0;
  size_t polled = 0;
  while (sunk < input_size) {
    HSE_sink_res esres =
        heatshrink_encoder_sink(hse, &input[sunk], input_size - sunk, &count);
    sunk += count;
    printk("^^ sunk %zd\n", count);
    if (sunk == input_size) {
      printk("%d\n", heatshrink_encoder_finish(hse));
    }

    HSE_poll_res pres;
    do { /* "turn the crank" */
      pres =
          heatshrink_encoder_poll(hse, &comp[polled], comp_sz - polled, &count);
      polled += count;
      printk("^^ polled %zd\n", count);
    } while (pres == HSER_POLL_MORE);
    if (sunk == input_size) {
      printk("%d\n", heatshrink_encoder_finish(hse));
    }
  }
  printk("in: %zu compressed: %zu ", input_size, polled);
  size_t compressed_size = polled;
  sunk = 0;
  polled = 0;
}