#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "q2.h"

/* Each task has its own priority, defined using a const int. */
static const int TASK1_PRIORITY = 1;
static const int TASK2_PRIORITY = 2;
static const int TASK3_PRIORITY = 3;

static SemaphoreHandle_t led_mutex;

/* Actively wait: this does not put the task into the Blocked state. */
static void active_wait_500ms(void)
{
    uint32_t start = board_time_us();

    while ((uint32_t)(board_time_us() - start) < 500000UL)
    {
        /* Busy waiting. */
    }
}

/* Task 1: LED ON, actively wait 0.5 seconds, then yield. */
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
        }

        taskYIELD();
    }
}

/* Task 2: LED OFF, then task-delay for 1 second. */
static void task2(void *parameters)
{
    (void)parameters;

    for (;;)
    {
        if (xSemaphoreTake(led_mutex, portMAX_DELAY) == pdTRUE)
        {
            board_led_write(0);

            xSemaphoreGive(led_mutex);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* Task 3: print a status message, then task-delay for 1 second. */
static void task3(void *parameters)
{
    (void)parameters;

    for (;;)
    {
        board_status_print();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int q2_start(void)
{
    led_mutex = xSemaphoreCreateMutex();

    if (led_mutex == NULL)
    {
        return 0;
    }

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

    /* Reaching here means the scheduler could not start. */
    return 0;
}