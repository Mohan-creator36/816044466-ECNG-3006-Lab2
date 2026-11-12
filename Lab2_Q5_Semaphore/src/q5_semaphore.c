
/*
 * Name: Mohan Latchman
 * Student ID: 816044466
 * Lab 2 - Question 5
 * Binary Semaphore vs Mutex
 */

#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "q2.h"

/*
 * 1 = Use a binary semaphore
 * 0 = Use the original mutex
 */
#define Q5_USE_BINARY_SEMAPHORE 1

/* Keep the original task priorities. */
static const int TASK1_PRIORITY = 1;
static const int TASK2_PRIORITY = 2;
static const int TASK3_PRIORITY = 3;

/* Shared synchronization handle. */
static SemaphoreHandle_t led_access = NULL;

/* Wait actively for 500 milliseconds. */

static void active_wait_500ms(void)
{
    uint32_t start = board_time_us();

    while ((uint32_t)(board_time_us() - start) < 500000UL)
    {
        __asm__ volatile("nop");
    }
}


/* Task 1: Turn the LED ON and actively wait. */
static void task1(void *parameters)
{
    (void)parameters;

    for (;;)
    {
        if (xSemaphoreTake(led_access,
                           portMAX_DELAY) == pdTRUE)
        {
            board_led_write(1);

            active_wait_500ms();

            xSemaphoreGive(led_access);

            board_log("Task 1: LED ON completed");
        }

        taskYIELD();
    }
}

/* Task 2: Turn the LED OFF and delay. */
static void task2(void *parameters)
{
    (void)parameters;

    for (;;)
    {
        /*
         * Measure how long Task 2 waits
         * to obtain the shared resource.
         */
        uint32_t start = board_time_us();

        if (xSemaphoreTake(led_access,
                           portMAX_DELAY) == pdTRUE)
        {
            uint32_t waitTime =
                board_time_us() - start;

            board_led_write(0);

            xSemaphoreGive(led_access);

            /* Report the measured waiting time. */
            board_log_wait(waitTime);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* Task 3: Send a status message every second. */
static void task3(void *parameters)
{
    (void)parameters;

    for (;;)
    {
        board_status_print();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* Create the tasks and synchronization mechanism. */
int q2_start(void)
{
    /*
     * Select either the semaphore or mutex.
     */
#if Q5_USE_BINARY_SEMAPHORE == 1

    led_access = xSemaphoreCreateBinary();

    if (led_access == NULL)
    {
        return 0;
    }

    /*
     * A newly created binary semaphore
     * is empty, so give it once before use.
     */
    if (xSemaphoreGive(led_access) != pdTRUE)
    {
        return 0;
    }

    board_log("Q5: Binary semaphore selected");

#else

    led_access = xSemaphoreCreateMutex();

    if (led_access == NULL)
    {
        return 0;
    }

    board_log("Q5: Mutex selected");

#endif

    /* Create Task 1. */
    if (xTaskCreate(task1, "LED_ON",
                    256, NULL,
                    TASK1_PRIORITY,
                    NULL) != pdPASS)
    {
        return 0;
    }

    /* Create Task 2. */
    if (xTaskCreate(task2, "LED_OFF",
                    256, NULL,
                    TASK2_PRIORITY,
                    NULL) != pdPASS)
    {
        return 0;
    }

    /* Create Task 3. */
    if (xTaskCreate(task3, "STATUS",
                    256, NULL,
                    TASK3_PRIORITY,
                    NULL) != pdPASS)
    {
        return 0;
    }

    vTaskStartScheduler();

    /* Scheduler should not return. */
    return 0;
}
