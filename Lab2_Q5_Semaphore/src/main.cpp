
/*
 * Name: Mohan Latchman
 * Student ID: 816044466
 * Lab 2 - Question 5
 */

#include <Arduino.h>
#include <FreeRTOS_SAMD21.h>
#include <stdio.h>

#include "q2.h"

/* Use the built-in LED on the MKR Zero. */
static const int LED_PIN = LED_BUILTIN;

extern "C" void board_led_write(int on)
{
    digitalWrite(LED_PIN, on ? HIGH : LOW);
}

extern "C" uint32_t board_time_us(void)
{
    return micros();
}

extern "C" void board_log(const char *message)
{
    SerialUSB.println(message);
}

/*
 * Display the time Task 2 waited
 * for access to the LED.
 */
extern "C" void board_log_wait(uint32_t wait_us)
{
    char message[90];

    snprintf(message, sizeof(message),
             "Task 2: LED OFF | Wait = %lu us",
             (unsigned long)wait_us);

    SerialUSB.println(message);
}

/* Send Task 3 status through UART and USB. */
extern "C" void board_status_print(void)
{
    Serial1.println("Task 3: system running");

    SerialUSB.println("Task 3: system running");
}

void setup(void)
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial1.begin(115200);
    SerialUSB.begin(115200);

    /* Allow the USB Serial Monitor to connect. */
    unsigned long start = millis();

    while (!SerialUSB &&
           (millis() - start < 5000UL))
    {
    }

    vSetErrorSerial(&SerialUSB);

    SerialUSB.println(
        "Starting Lab 2 Question 5"
    );

    if (!q2_start())
    {
        SerialUSB.println(
            "ERROR: FreeRTOS startup failed"
        );

        while (true)
        {
        }
    }
}

void loop(void)
{
    /* FreeRTOS manages the application tasks. */
}
