
#ifndef Q2_H
#define Q2_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int q2_start(void);

/* Functions used to communicate with the Arduino hardware. */
void board_led_write(int on);
uint32_t board_time_us(void);
void board_status_print(void);

/*
 * Q3 CHANGE:
 * Added a function to record task activity
 * with timestamps in the Serial Monitor.
 */
void board_log(const char *message);

#ifdef __cplusplus
}
#endif

#endif