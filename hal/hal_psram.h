#include "ch32fun.h"
#include "pins.h"
#include "hw_spi.h"

#define psram_select() PSRAM_GPIO->BSHR = (1 << (16 + PSRAM_CS_PIN))
#define psram_deselect() PSRAM_GPIO->BSHR = (1 << PSRAM_CS_PIN)

#define psram_spi_write(a, b) SPI_write_buf(a, b)
#define psram_spi_read(a, b) SPI_read_buf(a, b)
