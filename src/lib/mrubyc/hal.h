/*! @file
  @brief
  Hardware abstraction layer
        for Zephyr.

  <pre>
  Copyright (C) 2015- Kyushu Institute of Technology.
  Copyright (C) 2015- Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.
  </pre>
*/

#ifndef MRBC_SRC_HAL_H_
#define MRBC_SRC_HAL_H_

#include <zephyr/kernel.h>

#define MRBC_TICK_UNIT 1
#define MRBC_TIMESLICE_TICK_COUNT 10

#if !defined(MRBC_NO_TIMER)

#define hal_init() ((void)0)
void hal_enable_irq(void);
void hal_disable_irq(void);
#define hal_idle_cpu() ((k_msleep(MRBC_TICK_UNIT)))  // delay 1ms

#else

#define hal_init() ((void)0)
#define hal_enable_irq() (k_sched_unlock())
#define hal_disable_irq() (k_sched_lock())
#define hal_idle_cpu() ((k_msleep(MRBC_TICK_UNIT)))  // delay 1ms

#endif

int hal_write(int fd, const void *buf, int nbytes);
#define hal_flush(fd) ((void)0)
#define hal_abort(s) ((void)0)

#endif
