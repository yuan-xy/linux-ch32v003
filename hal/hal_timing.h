#include "ch32fun.h"

#define timing_delay_ms(n) Delay_Ms(n)
#define timing_delay_us(n) Delay_Us(n)

extern volatile uint64_t systick_micros;
#define timing_micros() (systick_micros)
