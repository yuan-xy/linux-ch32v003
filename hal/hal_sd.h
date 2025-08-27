#include "ch32fun.h"
#include "pins.h"
#include "hw_spi.h"

#define sd_select() SD_CS_GPIO->BSHR = (1 << (16 + SD_CS_PIN))
#define sd_deselect() SD_CS_GPIO->BSHR = (1 << SD_CS_PIN)

#define sd_spi_byte(b) SPI_transfer_8(b)

#define sd_led_off()
#define sd_led_on()
