
/*
 * Name: Mohan Latchman
 * Student ID: 816044466
 * Lab 2 - Question 3
 * Technique: Run to Completion (Option A)
 */

#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "q2.h"

/*
 * Set Q3_MODE to 0 for equal priorities.
 * Set Q3_MODE to 1 for Option A.
 */
#define Q3_MODE 0

/*
 * Use different priority settings for the two tests.
 * Higher numbers mean higher task priorities.
 */
#if Q3_MODE == 0

/* Test 1: All three tasks have the same priority. */
static const int TASK1_PRIORITY = 2;
static const int TASK2_PRIORITY = 2;
static const int TASK3_PRIORITY = 2;

#else

/* Test 2: Task 1 highest, then Task 2, then Task 3. */
static const int TASK1_PRIORITY = 3;
static const int TASK2_PRIORITY = 2;
static const int TASK3_PRIORITY = 1;

#endif

/* The two LED tasks share the same mutex. */
static SemaphoreHandle_t led_mutex = NULL;

/* Actively wait for 500 ms without blocking the task. */
static void active_wait_500ms(void)
{
    uint32_t start = board_time_us();

    while ((uint32_t)(board_time_us() - start) < 500000UL)
    {
        /* Continue checking until 500 ms have passed. */
    }
}

/* Task 1: Turn LED ON, wait actively, then yield. */
static void task1(void *parameters)
{
    (void)parameters;

    for (;;)
    {
        if (xSemaphoreTake(led_mutex, portMAX_DELAY) == pdTRUE)
        {
            board_led_write(1);

            active_wait_500ms();

            xSemaphoreGive(led_mutex);

            /* Q3 CHANGE: Record Task 1 execution. */
            board_log("Task 1: LED ON, active wait completed");
        }

        /*
         * Yield after the 500 ms active wait.
         * No extra task delay is added because
         * we are testing run-to-completion scheduling.
         */
        taskYIELD();
    }
}

/* Task 2: Turn LED OFF and delay for one second. */
static void task2(void *parameters)
{
    (void)parameters;

    for (;;)
    {
        if (xSemaphoreTake(led_mutex, portMAX_DELAY) == pdTRUE)
        {
            board_led_write(0);

            xSemaphoreGive(led_mutex);

            /* Record Task 2 execution. */
            board_log("Task 2: LED OFF");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* Task 3: Print the system status every second. */
static void task3(void *parameters)
{
    (void)parameters;

    for (;;)
    {
        board_status_print();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* Create the mutex and the three FreeRTOS tasks. */
int q2_start(void)
{
    led_mutex = xSemaphoreCreateMutex();

    if (led_mutex == NULL)
    {
        return 0;
    }

    /*
     * Display which scheduling test is running.
     */
#if Q3_MODE == 0
    board_log("Q3 Test 1: All tasks have equal priority");
#else
    board_log("Q3 Test 2: Task 1 > Task 2 > Task 3");
#endif

    if (xTaskCreate(task1, "LED_ON", 256, NULL,
                    TASK1_PRIORITY, NULL) != pdPASS)
    {
        return 0;
    }

    if (xTaskCreate(task2, "LED_OFF", 256, NULL,
                    TASK2_PRIORITY, NULL) != pdPASS)
    {
        return 0;
    }

    if (xTaskCreate(task3, "STATUS", 256, NULL,
                    TASK3_PRIORITY, NULL) != pdPASS)
    {
        return 0;
    }

    vTaskStartScheduler();

    /* Reaching here means the scheduler failed to start. */
    return 0;
}
