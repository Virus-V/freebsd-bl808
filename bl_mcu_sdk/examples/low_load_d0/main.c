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

#include <libfdt.h>

#include "md5.h"

static char path[256];

#if (__riscv_xlen == 64)
/* linux pmp setting */
const pmp_config_entry_t pmp_entry_tab[8] = {
    [0] =
        {
            .entry_flag =
                ENTRY_FLAG_ADDR_NAPOT | ENTRY_FLAG_PERM_W | ENTRY_FLAG_PERM_R,
            .entry_pa_base = 0x20000000,
            .entry_pa_length = 0x10000,
        },

    [1] =
        {
            .entry_flag =
                ENTRY_FLAG_ADDR_NAPOT | ENTRY_FLAG_PERM_W | ENTRY_FLAG_PERM_R,
            .entry_pa_base = 0x30000000,
            .entry_pa_length = PMP_REG_SZ_1M,
        },

    [2] =
        {
            .entry_flag = ENTRY_FLAG_ADDR_NAPOT | ENTRY_FLAG_PERM_X |
                          ENTRY_FLAG_PERM_W | ENTRY_FLAG_PERM_R,
            .entry_pa_base = 0x3eff0000,
            .entry_pa_length = 0x10000,
        },

    [3] =
        {
            .entry_flag = ENTRY_FLAG_ADDR_NAPOT | ENTRY_FLAG_PERM_X |
                          ENTRY_FLAG_PERM_W | ENTRY_FLAG_PERM_R,
            .entry_pa_base = 0x40000000,
            .entry_pa_length = PMP_REG_SZ_16K,
        },

    [4] =
        {
            .entry_flag = ENTRY_FLAG_ADDR_NAPOT | ENTRY_FLAG_PERM_X |
                          ENTRY_FLAG_PERM_W | ENTRY_FLAG_PERM_R,
            .entry_pa_base = 0x50000000,
            .entry_pa_length = 0x4000000,
        },

    [5] =
        {
            .entry_flag =
                ENTRY_FLAG_ADDR_NAPOT | ENTRY_FLAG_PERM_W | ENTRY_FLAG_PERM_R,
            .entry_pa_base = 0x58000000,
            .entry_pa_length = 0x4000000,
        },

    [6] =
        {
            .entry_flag =
                ENTRY_FLAG_ADDR_NAPOT | ENTRY_FLAG_PERM_W | ENTRY_FLAG_PERM_R,
            .entry_pa_base = 0xe0000000,
            .entry_pa_length = 0x8000000,
        },

    [7] =
        {
            .entry_flag = ENTRY_FLAG_ADDR_TOR,
            .entry_pa_base = 0xffffffffff, /* 40-bit PA */
            .entry_pa_length = 0,
        },
};

#endif

/* return loaded addr */
uint32_t fit_img_copy_node(void *fit, const char *name) {
  int offset, len;

  uint32_t load_addr;
  char *data_addr;
  uint32_t data_length;
  const struct fdt_property *prop;

  snprintf(path, sizeof(path), "/images/%s", name);
  offset = fdt_path_offset(fit, path);

  prop = fdt_get_property(fit, offset, "load", &len);
  load_addr = fdt32_to_cpu(*(uint32_t *)prop->data);

  prop = fdt_get_property(fit, offset, "data", &len);
  // printf("len:%d, prop->len:%d, tag:%d\n", len, fdt32_to_cpu(prop->len),
  // fdt32_to_cpu(prop->tag));
  data_addr = (char *)prop->data;
  data_length = (uint32_t)len;

  prop = fdt_get_property(fit, offset, "description", &len);
  MSG("process %s - 0x%x\r\n", prop->data, load_addr);

  memcpy((void *)(uintptr_t)load_addr, (void *)(uintptr_t)data_addr, data_length);

#if 0
  MD5Context ctx;

  md5Init(&ctx);

  md5Update(&ctx, (uint8_t *)(uintptr_t)load_addr, data_length);
  md5Finalize(&ctx);

  for (int i=0; i<16;i++) {
    MSG("%02x", ctx.digest[i]);
  }
  MSG("\r\n");
#endif

  return load_addr;
}

/* return loaded addr */
uint32_t fit_img_decomp_node(void *fit, const char *name) {
  int offset, len;

  uint32_t load_addr;
  char *data_addr;
  uint32_t data_length;
  const struct fdt_property *prop;

  snprintf(path, sizeof(path), "/images/%s", name);
  offset = fdt_path_offset(fit, path);

  prop = fdt_get_property(fit, offset, "load", &len);
  load_addr = fdt32_to_cpu(*(uint32_t *)prop->data);

  prop = fdt_get_property(fit, offset, "data", &len);
  // printf("len:%d, prop->len:%d, tag:%d\n", len, fdt32_to_cpu(prop->len),
  // fdt32_to_cpu(prop->tag));
  data_addr = (char *)prop->data;
  data_length = (uint32_t)len;

  prop = fdt_get_property(fit, offset, "description", &len);
  MSG("process lz4 decompress %s - 0x%x\r\n", prop->data, load_addr);

extern void unlz4(const void *aSource, void *aDestination, uint32_t FileLen);
  unlz4((const void *)data_addr, (void *)(uintptr_t)load_addr, data_length);

#if 0
  MD5Context ctx;

  md5Init(&ctx);

  md5Update(&ctx, (uint8_t *)(uintptr_t)load_addr, 5636900);
  md5Finalize(&ctx);

  for (int i=0; i<16;i++) {
    MSG("%02x", ctx.digest[i]);
  }
  MSG("\r\n");
#endif

  return load_addr;
}

#define MUST(x,msg) \
  if (!(x)) { \
    MSG msg;  \
    while(1); \
  }

int main(void) {
  bflb_platform_init(0);

  // #define GLB_AHB_CLOCK_LZ4 (0x0008000000000000UL)

  MSG("C906 start...\r\n");
  uint64_t start_time, stop_time;
  mtimer_init();
  MSG("mtimer clk:%d\r\n", CPU_Get_MTimer_Clock());

  //bflb_platform_delay_ms(100);

  extern uint32_t __mark_start[];

  void *mapped = (void *)__mark_start;

  int len;
  int config_offset, offset;
  const char *default_config;
  const char *opensbi, *kernel, *rootfs, *fdt;

  const struct fdt_property *prop;

  MUST (fdt_check_header(mapped) == 0, ("Address %p not a vaild FIT!\r\n", mapped));

  config_offset = fdt_path_offset(mapped, "/configurations");
  MUST(config_offset >= 0, ("%s:%d\r\n", __func__, __LINE__));
  prop = fdt_get_property(mapped, config_offset, "default", &len);
  MUST(prop != NULL, ("%s:%d\r\n", __func__, __LINE__));
  default_config = prop->data;
  MSG("default config is: %s\r\n", default_config);

  snprintf(path, 256, "/configurations/%s", default_config);
  config_offset = fdt_path_offset(mapped, path);

  prop = fdt_get_property(mapped, config_offset, "firmware", &len);
  opensbi = prop->data;
  MSG("opensbi: /images/%s\r\n", opensbi);

  prop = fdt_get_property(mapped, config_offset, "kernel", &len);
  kernel = prop->data;
  MSG("kernel: /images/%s\r\n", kernel);

  prop = fdt_get_property(mapped, config_offset, "ramdisk", &len);
  rootfs = prop->data;
  MSG("ramdisk: /images/%s\r\n", rootfs);

  prop = fdt_get_property(mapped, config_offset, "fdt", &len);
  fdt = prop->data;
  MSG("fdt: /images/%s\r\n", fdt);

  start_time = bflb_platform_get_time_us();

  void (*opensbi_entry)(int hart_id, void *fdt_addr)  = (void *)(uintptr_t)fit_img_copy_node((void *)mapped, opensbi);

  /* process kernel */
  (void)fit_img_decomp_node((void *)mapped, kernel);

  /* process dtb */
  void *fdt_addr = (void *)(uintptr_t)fit_img_copy_node((void *)mapped, fdt);

  /* process rootfs */
  do {
    uint32_t rootfs_cell[2];

    snprintf(path, sizeof(path), "/images/%s", rootfs);
    offset = fdt_path_offset(mapped, path);

    prop = fdt_get_property(mapped, offset, "data", &len);
    rootfs_cell[0] = cpu_to_fdt32((uintptr_t)prop->data);
    rootfs_cell[1] = cpu_to_fdt32((uint32_t)len);

    prop = fdt_get_property(mapped, offset, "description", &len);
    MSG("rootfs: <0x%x 0x%x>\r\n", fdt32_to_cpu(rootfs_cell[0]), fdt32_to_cpu(rootfs_cell[1]));

    /* patch xip node */
    MUST (fdt_check_header(fdt_addr) == 0, ("Address %p not a vaild FDT!\r\n", fdt_addr));

    offset =  fdt_path_offset(fdt_addr, "/xip");
    MUST(offset >= 0, ("%s:%d\r\n", __func__, __LINE__));
    MUST(fdt_setprop(fdt_addr, offset, "reg", rootfs_cell, sizeof(rootfs_cell)) == 0, ("update xip failed\r\n"));
  } while (0);

  stop_time = bflb_platform_get_time_us();
  MSG("\r\nload time: %ld us \r\n", (stop_time - start_time));

  csi_dcache_clean_invalid();

  __ASM volatile("csrw mcor, %0" : : "r"(0x30013));

  // csi_dcache_disable();
  // csi_icache_disable();
  //  __set_MHINT(0x450c);

  uint64_t mxstatus = __get_MXSTATUS();
  mxstatus &= ~((0 << 22) | (1 << 21));
  __set_MXSTATUS(mxstatus);

  rvpmp_init(pmp_entry_tab, sizeof(pmp_entry_tab) / sizeof(pmp_config_entry_t));
  __ISB();

#if 0
  /* c906 clk and rtc clk setting */
  unsigned int reg;
  reg = *(volatile unsigned int *)0x30007004;
  reg |= (0x0 << 0); // pll div --> 400Mhz
  *(volatile unsigned int *)0x30000018 = 0x8000017b;
#endif

  /* set core volt 1.2 */
  // *(volatile unsigned int *)0x2000f814 = 0x14476c20;

  opensbi_entry(0, fdt_addr);

  while (1) {
#ifdef __riscv_muldiv
    int dummy;
    /* In lieu of a halt instruction, induce a long-latency stall. */
    __asm__ __volatile__("div %0, %0, zero" : "=r"(dummy));
#endif
  }
}
