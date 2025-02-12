/** @brief Constants, structures, function prototypes for hts221
 *  @file hts221.h
 *  @since 2024-03-01
 */
 
#ifndef HTS221_H
#define HTS221_H

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
#define HTS221_DEV_ID 0x5F
#ifndef WHO_AM_I
#define WHO_AM_I 0x0F
#endif // WHO_AM_I

#ifndef CTRL_REG1
#define CTRL_REG1 0x20
#endif // CTRL_REG1
#ifndef CTRL_REG2
#define CTRL_REG2 0x21
#endif // CTRL_REG2

#define T0_OUT_L 0x3C
#define T0_OUT_H 0x3D
#define T1_OUT_L 0x3E
#define T1_OUT_H 0x3F
#define T0_degC_x8 0x32
#define T1_degC_x8 0x33
#define T1_T0_MSB 0x35

#define TMP_OUT_L 0x2A
#define TMP_OUT_H 0x2B

#define H0_T0_OUT_L 0x36
#define H0_T0_OUT_H 0x37
#define H1_T0_OUT_L 0x3A
#define H1_T0_OUT_H 0x3B
#define H0_rH_x2 0x30
#define H1_rH_x2 0x31

#define H_T_OUT_L 0x28
#define H_T_OUT_H 0x29

// Enumerated Types

// Structures

// Function Prototypes
/// @cond INTERNAL
double ShGetTemperatureAlt(); //From humidity sensor
double ShGetHumidity();
/// @endcond

#endif // HTS221_H