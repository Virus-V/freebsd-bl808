list(REMOVE_ITEM GLOBAL_LD_FLAGS --specs=nano.specs)
list(APPEND GLOBAL_C_FLAGS -I${CMAKE_CURRENT_SOURCE_DIR}/libfdt)

set(TARGET_REQUIRED_SRCS rv32i_xtheade_lz4.S md5.c
  libfdt/fdt.c
  libfdt/fdt_addresses.c
  libfdt/fdt_empty_tree.c
  libfdt/fdt_overlay.c
  libfdt/fdt_ro.c
  libfdt/fdt_rw.c
  libfdt/fdt_strerror.c
  libfdt/fdt_sw.c
  libfdt/fdt_wip.c
)

set(mains main.c)
if(${CPU_ID} STREQUAL "d0" )
    set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/bl808_flash_d0.ld)
endif()
generate_bin()
