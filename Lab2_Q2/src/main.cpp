#include <Arduino.h>
#include <FreeRTOS_SAMD21.h>

#include "q2.h"

static const int LED_PIN = 2;

extern "C" void board_led_write(int on)
{
    digitalWrite(LED_PIN, on ? HIGH : LOW);
}

extern "C" uint32_t board_time_us(void)
{
    return micros();
}

extern "C" void board_status_print(void)
{
    /* Send through the hardware UART. */
    Serial1.println("Task 3: system running");

    /* Mirror the message to USB for the laptop's Serial Monitor. */
    SerialUSB.println("Task 3: system running");
}

void setup(void)
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial1.begin(115200);
    SerialUSB.begin(115200);

    /* Allow time for the USB connection without waiting forever. */
    unsigned long start = millis();

    while (!SerialUSB && (millis() - start < 5000UL))
    {
    }

    vSetErrorSerial(&SerialUSB);

    SerialUSB.println("Starting Q2 FreeRTOS application");

    if (!q2_start())
    {
        SerialUSB.println("ERROR: task creation or scheduler startup failed");

        while (true)
        {
        }
    }
}

void loop(void)
{
    /* No application work is needed here. */
}