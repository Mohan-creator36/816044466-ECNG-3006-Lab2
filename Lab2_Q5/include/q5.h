
#ifndef Q2_H
#define Q2_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int q2_start(void);

void board_led_write(int on);
uint32_t board_time_us(void);
void board_status_print(void);
void board_log(const char *message);


void board_log_wait(uint32_t wait_us);

#ifdef __cplusplus
}
#endif

#endif
