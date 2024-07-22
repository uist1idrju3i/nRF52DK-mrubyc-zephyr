#include "hal.h"

#include <stdlib.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "../../../mrubyc/src/mrubyc.h"

#define HAL_WRITE_BUFFER_SIZE 255
#define MEMORY_SIZE (1024 * 20)
static uint8_t memory_pool[MEMORY_SIZE];

/* ===== prototype ===== */
static int mrubyc_init(void);
static int mrubyc_main(void);
void mrubyc_tick(struct k_work *const work);

/* ===== kernel ===== */
SYS_INIT(mrubyc_init, APPLICATION, 0);
K_WORK_DEFINE(mrubyc_work, mrubyc_tick);
K_THREAD_DEFINE(mrubyc_thread, 384, mrubyc_main, NULL, NULL, NULL, -2, 0, 0);

/* ===== mrubyc_init ===== */
static int mrubyc_init(void) {
  mrbc_init(memory_pool, MEMORY_SIZE);
  return EXIT_SUCCESS;
}

/* ===== mrubyc_main ===== */
static int mrubyc_main(void) {
  while (1) {
    k_work_submit(&mrubyc_work);
    k_msleep(1);
  }
  return EXIT_FAILURE;
}

/* ===== mrubyc_tick ===== */
void mrubyc_tick(struct k_work *const work) { mrbc_tick(); }

/* ===== hal_write ===== */
int hal_write(int fd, const void *buf, int nbytes) {
  char buffer[HAL_WRITE_BUFFER_SIZE] = {0};
  if (HAL_WRITE_BUFFER_SIZE < nbytes) {
    return -1;
  }
  for (int i = 0; i < nbytes; i++) {
    buffer[i] = ((char *)buf)[i];
  }
  printk("%s", buffer);
  return nbytes;
}
