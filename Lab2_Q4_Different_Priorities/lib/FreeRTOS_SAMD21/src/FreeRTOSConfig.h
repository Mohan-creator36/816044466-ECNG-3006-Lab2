/*
 * Lab 2 fix: the FreeRTOS kernel in this library includes "FreeRTOSConfig.h"
 * from its own folder first. This file forwards that include to the
 * application's configuration file, so the kernel is built with
 * <project>/include/FreeRTOSConfig.h and the settings there take effect.
 */
#include "../../../include/FreeRTOSConfig.h"
