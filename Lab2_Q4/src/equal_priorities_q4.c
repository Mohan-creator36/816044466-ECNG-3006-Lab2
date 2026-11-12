
/*
 * Name: Mohan Latchman
 * Student ID: 816044466
 * Lab 2 - Question 4
 * Mechanism: IDLE Hook
 */

#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "q2.h"

/*
 * 0 = Equal task priorities
 * 1 = Different task priorities
 */
#define Q4_MODE 0

#if Q4_MODE == 0

static const int TASK1_PRIORITY = 2;
static const int TASK2_PRIORITY = 2;
static const int TASK3_PRIORITY = 2;

#else

static const int TASK1_PRIORITY = 3;
static const int TASK2_PRIORITY = 2;
static const int TASK3_PRIORITY = 1;

#endif

/* Mutex shared by the two LED tasks. */
static SemaphoreHandle_t led_mutex = NULL;

/*
 * Keep track of how many times each task runs.
 */
static volatile uint32_t task1Runs = 0U;
static volatile uint32_t task2Runs = 0U;
static volatile uint32_t task3Runs = 0U;

/* Actively wait for half a second. */

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
        if (xSemaphoreTake(led_mutex, portMAX_DELAY) == pdTRUE)
        {
            board_led_write(1);
            task1Runs++;

            active_wait_500ms();

            xSemaphoreGive(led_mutex);
        }

        taskYIELD();

        /*
         * Block Task 1 for one second.
         * This allows the Idle task to run when
         * the other tasks are also blocked.
         */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* Task 2: Turn the LED OFF and delay. */
static void task2(void *parameters)
{
    (void)parameters;

    for (;;)
    {
        if (xSemaphoreTake(led_mutex, portMAX_DELAY) == pdTRUE)
        {
            board_led_write(0);
            task2Runs++;

            xSemaphoreGive(led_mutex);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/*
 * Task 3 reports Idle Hook activity
 * and the number of task executions.
 */
static void task3(void *parameters)
{
    (void)parameters;

    uint32_t previousIdle = board_idle_count_read();
    TickType_t previousTick = xTaskGetTickCount();

    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));

        uint32_t currentIdle = board_idle_count_read();
        TickType_t currentTick = xTaskGetTickCount();

        uint32_t idleDifference =
            currentIdle - previousIdle;

        uint32_t elapsedTicks =
            (uint32_t)(currentTick - previousTick);

        previousIdle = currentIdle;
        previousTick = currentTick;

        task3Runs++;

        /* Report the results through UART and USB. */
        board_status_print(
            idleDifference,
            elapsedTicks,
            task1Runs,
            task2Runs,
            task3Runs
        );
    }
}

/* Create the mutex and start FreeRTOS. */
int q2_start(void)
{
    led_mutex = xSemaphoreCreateMutex();

    if (led_mutex == NULL)
    {
        return 0;
    }

    /*
     * Identify the current scheduling test.
     */
#if Q4_MODE == 0
    board_log("Q4 Test 1: Equal priorities");
#else
    board_log("Q4 Test 2: Different priorities");
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

    return 0;
}
