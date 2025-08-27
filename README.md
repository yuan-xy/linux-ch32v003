# linux-ch32v003
Linux on a $0.15 RISC-V microcontroller

This project enables the CH32V003 microcontroller to run Linux. It achieves this by using an 8 megabyte SPI PSRAM chip and the [mini-rv32ima RISC-V emulator by cnlohr](https://github.com/cnlohr/mini-rv32ima), packaged into the [tiny-rv32ima library](https://github.com/tvlad1234/tiny-rv32ima). Emulating the system is necessary because the PSRAM cannot be mapped into the address space of the microcontroler. Storage for the Linux kernel, device tree binary and rootfs image is provided by an SD card.

This application is enabled by the [ch32fun](https://github.com/cnlohr/ch32fun) SDK.

## How to use

The PSRAM and SD card are connected to the hardware SPI interface of the CH32V003. The chip select pins are defined in the [pins.h](pins.h) file. The system console is exposed over the UART.

The SD card containing the Linux kernel and rootfs image must be formatted as FAT32 or FAT16, with the necessary files placed in the root directory.

A suggested schematic and corresponding single-layer PCB design is included in the [hardware](hardware) folder (it's a KiCad 7 project).

Boot time is around 7 minutes. The provided Linux image includes the coremark benchmark.

## Building the Linux image
The Linux distribution meant to be used with tiny-rv32ima is built from [buildroot-tiny-rv32ima](https://github.com/tvlad1234/buildroot-tiny-rv32ima.git). Pre-built images are available in the Releases section of the buildroot-tiny-rv32ima repo.