
#include "stdio.h"
#include "string.h"
#include "vm_config.h"

#define console_putc(c) putchar(c)
#define console_puts(c) puts(c)

#define console_available() (keys_num > 0)

volatile extern char key_queue[KEY_QUEUE_LEN + 1];
volatile extern uint8_t keys_num;

#define pwr_button console_available

void console_panic(char *s)
{
    console_puts(s);
    while (1)
        ;
}

uint8_t console_read()
{
    char c = key_queue[0];
    keys_num--;
    strcpy(key_queue, key_queue + 1);
    return c;
}
