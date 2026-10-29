#ifndef Q2_H
#define Q2_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int q2_start(void);

/* Arduino functions made available to the C application. */
void board_led_write(int on);
uint32_t board_time_us(void);
void board_status_print(void);

#ifdef __cplusplus
}
#endif

#endif