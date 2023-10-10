list(REMOVE_ITEM GLOBAL_LD_FLAGS --specs=nano.specs)

set(mains main.c)
generate_bin()
