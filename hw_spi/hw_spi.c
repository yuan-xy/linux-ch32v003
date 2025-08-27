#include "hw_spi.h"
#include "ch32fun.h"
#include <stdint.h>

static inline void SPI_poweroff(void);
static inline void SPI_poweron(void);
static inline void SPI_wait_TX_complete(void);
static inline uint8_t SPI_is_RX_empty(void);
static inline void SPI_wait_RX_available(void);
static inline uint8_t SPI_read_8(void);
static inline void SPI_write_8(uint8_t data);

void SPI_set_prescaler(uint8_t presc)
{
    SPI1->CTLR1 &= ~SPI_CTLR1_BR;
    SPI1->CTLR1 |= SPI_CTLR1_BR & (presc << 3);
}

void SPI_init(void)
{
    SPI_poweron();

    // reset control register
    SPI1->CTLR1 = 0;

    SPI_set_prescaler(0);

    SPI1->CTLR1 |= (SPI_CPOL_Low | SPI_CPHA_1Edge);

    SPI1->CTLR1 |= SPI_NSS_Soft; // SSM NSS software control mode

    // CH32V003 is master
    SPI1->CTLR1 |= SPI_Mode_Master;

    // set data direction and configure data pins
    SPI1->CTLR1 |= SPI_Direction_2Lines_FullDuplex;
    SPI1->HSCR |= SPI_HSCR_HSRXEN;
}

void SPI_begin_8(void)
{
    SPI1->CTLR1 &= ~(SPI_CTLR1_DFF); // DFF 16bit data-length enable, writable only when SPE is 0
    SPI1->CTLR1 |= SPI_CTLR1_SPE;
}

void SPI_end(void)
{
    SPI1->CTLR1 &= ~(SPI_CTLR1_SPE);
}

inline uint8_t SPI_transfer_8(uint8_t data)
{
    SPI1->DATAR = data;
    while (!(SPI1->STATR & SPI_STATR_RXNE))
        ;
    return SPI1->DATAR;
}

inline void SPI_write_buf(void const *buf, uint32_t size)
{
    while (size--)
    {
        SPI1->DATAR = *(uint8_t *)(buf++);
        while (!(SPI1->STATR & SPI_STATR_RXNE))
            ;
        SPI1->DATAR;
    }
    // SPI_transfer_8( *(uint8_t *)( buf++ ) );
}

inline void SPI_read_buf(uint8_t *buf, uint32_t size)
{
    while (size--)
    {
        SPI1->DATAR = 0xff;
        while (!(SPI1->STATR & SPI_STATR_RXNE))
            ;
        *(buf++) = SPI1->DATAR;
    }
    // *( buf++ ) = SPI_transfer_8( 0xFF );
}

// ########  small internal function definitions, static inline

static inline void SPI_poweroff(void)
{
    SPI_end();
    RCC->APB2PCENR &= ~RCC_APB2Periph_SPI1;
}

static inline void SPI_poweron(void)
{
    RCC->APB2PCENR |= RCC_APB2Periph_SPI1;
}

static inline uint8_t SPI_read_8(void)
{
    return SPI1->DATAR;
}

static inline void SPI_write_8(uint8_t data)
{
    SPI1->DATAR = data;
}

static inline void SPI_wait_TX_complete(void)
{
    while (!(SPI1->STATR & SPI_STATR_TXE))
        ;
}

static inline uint8_t SPI_is_RX_empty(void)
{
    return SPI1->STATR & SPI_STATR_RXNE;
}

static inline void SPI_wait_RX_available(void)
{
    while (!(SPI1->STATR & SPI_STATR_RXNE))
        ;
}

static inline void SPI_wait_not_busy(void)
{
    while ((SPI1->STATR & SPI_STATR_BSY) != 0)
        ;
}

static inline void SPI_wait_transmit_finished(void)
{
    SPI_wait_TX_complete();
    SPI_wait_not_busy();
}
