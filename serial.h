#include "hardware/uart.h"

#define p_size 8

uint8_t boot_msg[p_size] = {0x4, 0x0, 0x0, 0x0, 'b', 'o', 'o', 't'};
uint8_t state_tx[p_size] = {0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 't', 'x'};

void usend(uint8_t tx_buff[]){
    uart_write_blocking(uart0,tx_buff,p_size);
}