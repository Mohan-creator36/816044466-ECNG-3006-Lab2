
/*
 * Name: Mohan Latchman
 * Student ID: 816044466
 * Lab 2 - Question 3
 * Technique: Run to Completion (Option A)
 */

#include <Arduino.h>
#include <FreeRTOS_SAMD21.h>

#include "q2.h"

/*
 * Q3 CHANGE:
 * Use the built-in LED instead of an external LED on pin 2.
 */
static const int LED_PIN = LED_BUILTIN;

extern "C" void board_led_write(int on)
{
    digitalWrite(LED_PIN, on ? HIGH : LOW);
}

extern "C" uint32_t board_time_us(void)
{
    return micros();
}

/*
 * Print task messages with microsecond timestamps
 * to help compare the two scheduling configurations.
 */
extern "C" void board_log(const char *message)
{
    SerialUSB.print(micros());
    SerialUSB.print(" us | ");
    SerialUSB.println(message);
}

extern "C" void board_status_print(void)
{
    /* Send the status message using the hardware UART. */
    Serial1.println("Task 3: system running");

    /*
     * Also display Task 3 with a timestamp
     * in the USB Serial Monitor.
     */
    board_log("Task 3: system running");
}

void setup(void)
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial1.begin(115200);
    SerialUSB.begin(115200);

    /* Allow time for the USB serial connection. */
    unsigned long start = millis();

    while (!SerialUSB && (millis() - start < 5000UL))
    {
    }

    vSetErrorSerial(&SerialUSB);

    /* Updated the startup message. */
    SerialUSB.println("Starting Q3 FreeRTOS application");

    if (!q2_start())
    {
        SerialUSB.println(
            "ERROR: Task creation or scheduler startup failed"
        );

        while (true)
        {
        }
    }
}

void loop(void)
{
    /* The three tasks are managed by FreeRTOS. */
}
