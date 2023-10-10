/**
 * @file main.c
 * @brief
 *
 * Copyright (c) 2022 Bouffalolab team
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 */
#include "bflb_platform.h"
#include "hal_mtimer.h"
#include "rv_hpm.h"
#include "rv_pmp.h"
// #include "bl808_lz4d.h"
#include "bl808_glb.h"
#include "bl808_gpio.h"
#include "bl808_psram_uhs.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  bflb_platform_init(0);

  MSG("E907 start...\r\n");
  mtimer_init();
  MSG("mtimer clk:%d\r\n", CPU_Get_MTimer_Clock());

  MSG("psram clk init ok!\r\n");
  // MSG("m0 main! size_t:%d\r\n", sizeof(size_t));

  csi_dcache_disable();
#ifdef DUALCORE
  BL_WR_WORD(IPC_SYNC_ADDR1, IPC_SYNC_FLAG);
  BL_WR_WORD(IPC_SYNC_ADDR2, IPC_SYNC_FLAG);
  L1C_DCache_Clean_By_Addr(IPC_SYNC_ADDR1, 8);
#endif

  while (1) {
#ifdef __riscv_muldiv
    int dummy;
    /* In lieu of a halt instruction, induce a long-latency stall. */
    __asm__ __volatile__("div %0, %0, zero" : "=r"(dummy));
#endif
  }
}
