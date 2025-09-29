all : flash

TARGET:=linux-ch32v003

ADDITIONAL_C_FILES:= hw_spi/hw_spi.c tiny-rv32ima/pff/pff.c tiny-rv32ima/psram/psram.c tiny-rv32ima/cache/cache.c tiny-rv32ima/emulator/emulator.c
EXTRA_CFLAGS:= -Ihw_spi -Ihal -Itiny-rv32ima

ifeq ($(SPI_FLASH), 1)
	ADDITIONAL_C_FILES += tiny-rv32ima/pff/w25qxx.c
else
	ADDITIONAL_C_FILES += tiny-rv32ima/pff/mmcbbp.c
endif

CH32FUN:=ch32fun/ch32fun
TARGET_MCU?=CH32V003

MINICHLINK?=$(CH32FUN)/../minichlink

include $(CH32FUN)/ch32fun.mk

flash : cv_flash
clean : cv_clean

linux : 
	make -C linux

.PHONY: linux
