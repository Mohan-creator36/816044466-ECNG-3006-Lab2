
/*
 * Name: Mohan Latchman
 * Student ID: 816044466
 * Lab 2 - Question 4
 * Mechanism: IDLE Hook
 */

#include <Arduino.h>
#include <FreeRTOS_SAMD21.h>

#include "q2.h"

/* Use the MKR Zero's built-in LED. */
static const int LED_PIN = LED_BUILTIN;

/*
 * Q4 CHANGE:
 * Count how many times the Idle Hook executes.
 */
static volatile uint32_t idleCount = 0U;

extern "C" void board_led_write(int on)
{
    digitalWrite(LED_PIN, on ? HIGH : LOW);
}

extern "C" uint32_t board_time_us(void)
{
    return micros();
}

/* Q4 CHANGE: Read the current idle count. */
extern "C" uint32_t board_idle_count_read(void)
{
    return idleCount;
}

extern "C" void board_log(const char *message)
{
    SerialUSB.println(message);
}

/*
 * Q4 CHANGE:
 * Display the Idle Hook results and task counts.
 * Send results through Serial1 and SerialUSB.
 */
extern "C" void board_status_print(
    uint32_t idleDifference,
    uint32_t elapsedTicks,
    uint32_t task1Count,
    uint32_t task2Count,
    uint32_t task3Count)
{
    SerialUSB.println("---- Q4 STATUS ----");

    SerialUSB.print("Idle Hook calls: ");
    SerialUSB.println((unsigned long)idleDifference);

    SerialUSB.print("Elapsed RTOS ticks: ");
    SerialUSB.println((unsigned long)elapsedTicks);

    SerialUSB.print("Task 1 executions: ");
    SerialUSB.println((unsigned long)task1Count);

    SerialUSB.print("Task 2 executions: ");
    SerialUSB.println((unsigned long)task2Count);

    SerialUSB.print("Task 3 executions: ");
    SerialUSB.println((unsigned long)task3Count);

    SerialUSB.println("-------------------");

    /* Also transmit the measurements through hardware UART. */
    Serial1.print("Idle calls: ");
    Serial1.print((unsigned long)idleDifference);

    Serial1.print(" | Ticks: ");
    Serial1.print((unsigned long)elapsedTicks);

    Serial1.print(" | T1: ");
    Serial1.print((unsigned long)task1Count);

    Serial1.print(" | T2: ");
    Serial1.print((unsigned long)task2Count);

    Serial1.print(" | T3: ");
    Serial1.println((unsigned long)task3Count);
}

void setup(void)
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial1.begin(115200);
    SerialUSB.begin(115200);

    /* Give the USB Serial Monitor time to connect. */
    unsigned long start = millis();

    while (!SerialUSB && (millis() - start < 5000UL))
    {
    }

    vSetErrorSerial(&SerialUSB);

    SerialUSB.println("Starting Q4 FreeRTOS application");

    if (!q2_start())
    {
        SerialUSB.println("ERROR: FreeRTOS failed to start");

        while (true)
        {
        }
    }
}

/*
 * Q4 CHANGE:
 * FreeRTOS_SAMD21 calls Arduino loop() through
 * its existing Idle Hook implementation.
 */
void loop(void)
{
    idleCount++;
}
