#ifndef HW_SPI_H
#define HW_SPI_H

#include <stdint.h>

void SPI_init(void);
void SPI_begin_8(void);
void SPI_end(void);
uint8_t SPI_transfer_8(uint8_t data);
void SPI_write_buf(void const *buf, uint32_t size);
void SPI_read_buf(uint8_t *buf, uint32_t size);
void SPI_set_prescaler(uint8_t presc);

#endif