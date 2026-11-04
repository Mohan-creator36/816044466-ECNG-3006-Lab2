
#ifndef Q2_H
#define Q2_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int q2_start(void);

void board_led_write(int on);
uint32_t board_time_us(void);

/* Q4 CHANGE: Read the Idle Hook counter. */
uint32_t board_idle_count_read(void);

/* Q4 CHANGE: Report task and idle activity. */
void board_status_print(
    uint32_t idleDifference,
    uint32_t elapsedTicks,
    uint32_t task1Count,
    uint32_t task2Count,
    uint32_t task3Count
);

void board_log(const char *message);

#ifdef __cplusplus
}
#endif

#endif
