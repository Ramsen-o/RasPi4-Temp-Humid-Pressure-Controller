/** @brief Constants, structures, function prototypes for lps25h
 *  @file lps25h.h
 *  @since 2024-03-01
 */
 
#ifndef LPS25H_H
#define LPS25H_H

// Includes
#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Constants
#ifndef DEV_PATH
#define DEV_PATH "/dev/i2c-1"
#endif // DEV_PATH
#define LPS25H_DEV_ID 0x5c
#ifndef WHO_AM_I
#define WHO_AM_I 0x0F
#endif // WHO_AM_I

#ifndef CTRL_REG1
#define CTRL_REG1 0x20
#endif // CTRL_REG1
#ifndef CTRL_REG2
#define CTRL_REG2 0x21
#endif // CTRL_REG2

#define PRESS_OUT_XL 0x28
#define PRESS_OUT_L 0x29
#define PRESS_OUT_H 0x2A
#define TEMP_OUT_L 0x2B
#define TEMP_OUT_H 0x2C

// Enumerated Types

// Structures

// Function Prototypes
/// @cond INTERNAL
double ShGetTemperature(); //From pressure sensor
double ShGetPressure();
/// @endcond

#endif // LPS25H_H