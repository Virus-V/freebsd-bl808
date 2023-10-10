# FreeBSD IoT Project
The FreeBSD IoT Project is a research project that hopes to provide a business-friendly IoT standard solution based on FreeBSD's ultimate stability, security and complete ecology.

# Supported Boards
- [Sipeed M1s Dock](https://wiki.sipeed.com/hardware/zh/maix/m1s/m1s_dock.html)

## Environment setup
Currently **ONLY** FreeBSD environment is supported. You can use software such as Virtual Box to install FreeBSD 13.2 or higher.

```
# pkg update
# pkg install git gmake liblz4 u-boot-tools riscv64-none-elf-gcc cmake ninja
# git clone --recursive git@github.com:Virus-V/freebsd-bl808.git
# cd freebsd-bl808
# make
```
*NOTE*: **FreeBSD requires root permissions to deploy rootfs, so the build process needs to be carried out with root permissions.**

BL808 FreeBSD firmware is in the `out` directory: `bl808_rtos_m0.bin` and `bl808_freebsd_d0.bin`
```
out
├── bl808_freebsd_d0.bin
└── bl808_rtos_m0.bin

1 directory, 2 files
```

### Build Customer Applications
The custom Application is located in the `apps` directory, and you can build it through the make tool. For example:
```bash
make apps/helloworld/install # build and deploy helloworld app
make apps/bsdinfo/install # build and deploy bsdinfo app

make #Repackage images
```

## Download Firmware
1. Get the latest version of BouffaloLab DevCube from http://dev.bouffalolab.com/download
2. Follow the steps in the picture below:

![FlashTool](assets/BLDevCube_d0iDVi4UwD.png "Program BL808")

After the burning is completed, reset the board and you can see the following log, which means FreeBSD is running successfully.

![PuTTY](assets/putty_PLyYiMSmWe.png "FreeBSD Log")

# TODOs
1. Adapt to the native serial port driver instead of using the functions provided by OpenSBI.
2. Enable net80211 wireless protocol stack (experimental)
3. Porting the LVGL graphics library.
4. Port rpmsg-lite to kernel and design rpmsg-lite bus driver. (Establish communication with RTOS core)
5. Bring up BLE and Zigbee.
6. Porting WebRTC and Matter.

# License
BSD-3-Clause license
