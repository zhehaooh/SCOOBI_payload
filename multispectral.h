/*
 * multispectral.h
 *
 *  Created on: 4 Mar 2020
 *      Author: Zhe Hao
 */

#ifndef MULTISPECTRAL_H_
#define MULTISPECTRAL_H_

// Neccessary library header file includes
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "MCU_Init.h"
#include <string.h>
#include "component.h"
#include "sensor.h"
#include "main.h"
#include <stdio.h>

// START AS7265X DEFINE //
// I2C Slave address
#define AS726X_I2C_ADDR 0x49

// Register addresses

// Device info registers
#define AS726x_DEVICE_TYPE 0x00
#define AS726x_DEVICE_HW_VERSION 0x01
#define AS726x_DEVICE_TEMP 0x06

// Configuration registers
#define AS726x_CONFIG_OPMODE 0x04
#define AS726x_CONFIG_INT_TIME 0x05
#define AS726x_CONFIG_LED 0x07
#define AS726x_CONFIG_DEV_SEL 0x4F

// Virtual register R/W conditions
#define AS72XX_OPERATION_STATUS_REG 0x00
#define AS72XX_OPERATION_WRITE_REG 0x01
#define AS72XX_OPERATION_READ_REG 0x02
#define AS72XX_OPERATION_TX_VALID 0x02
#define AS72XX_OPERATION_RX_VALID 0x01

// Batch read start end register addresses
#define AS726x_READ_RAW_START 0x08
#define AS726x_READ_RAW_END 0x12
#define AS726x_CONFIG_DEV_SEL_START 0x00
#define AS726x_CONFIG_DEV_SEL_END 0x02

// Init data to write
#define AS726x_CONFIG_OPMODE_RESET 0x98
#define AS726x_CONFIG_OPMODE_INIT 0x08
#define AS726x_CONFIG_INT_TIME_INIT 0x3F

// Single read address code: 0x(DEV_SEL)(REG_ADD)
#define AS726x_READ_RAW_R 0x0008
#define AS726x_READ_RAW_G 0x0108
#define AS726x_READ_RAW_A 0x0208

#define AS726x_READ_RAW_S 0x000A
#define AS726x_READ_RAW_H 0x010A
#define AS726x_READ_RAW_B 0x020A

#define AS726x_READ_RAW_T 0x000C
#define AS726x_READ_RAW_I 0x010C
#define AS726x_READ_RAW_C 0x020C

#define AS726x_READ_RAW_U 0x000E
#define AS726x_READ_RAW_J 0x010E
#define AS726x_READ_RAW_D 0x020E

#define AS726x_READ_RAW_V 0x0010
#define AS726x_READ_RAW_K 0x0110
#define AS726x_READ_RAW_E 0x0210

#define AS726x_READ_RAW_W 0x0012
#define AS726x_READ_RAW_L 0x0112
#define AS726x_READ_RAW_F 0x0212


// Read failure code
#define AS726x_READ_ERROR 0xFF

// Number of channels
#define AS726x_ATTR_NUM_CHANNELS 18

// R/W procedure parameters
#define AS726x_I2C_RW_DELAY 5
#define AS726x_I2C_RESET_DELAY 1000
#define AS726x_I2C_RW_RETRIES 1000
#define AS726x_I2C_RESET_RETRIES 5

// END AS7265X DEFINE //

// START VEML6075 DEFINE //
// I2C Slave address
#define VEML6075_I2C_ADDRESS 0x10

// Register addresses
#define VEML6075_CONFIG 0x00
#define VEML6075_READ_UVA 0x07
#define VEML6075_READ_UVB 0x09
#define VEML6075_READ_UVCOMP1 0x0A
#define VEML6075_READ_UVCOMP2 0x0B

// VEML6075_CONFIG register setting values
#define VEML6075_CONFIG_SD 0x01
#define VEML6075_CONFIG_UV_AF_AUTO 0x00
#define VEML6075_CONFIG_UV_AF_FORCE 0x02
#define VEML6075_CONFIG_UV_TRIG_NO 0x00
#define VEML6075_CONFIG_UV_TRIG_ONCE 0x04
#define VEML6075_CONFIG_HD 0x08
#define VEML6075_CONFIG_UV_IT_MASK 0x70
#define VEML6075_CONFIG_UV_IT_50MS 0x00
#define VEML6075_CONFIG_UV_IT_100MS 0x10
#define VEML6075_CONFIG_UV_IT_200MS 0x20
#define VEML6075_CONFIG_UV_IT_400MS 0x30
#define VEML6075_CONFIG_UV_IT_800MS 0x40
#define VEML6075_CONFIG_DEFAULT 0x0008
//#define VEML6075_CONFIG_DEFAULT (VEML6075_CONFIG_UV_AF_AUTO | ; VEML6075_CONFIG_UV_TRIG_NO | ; VEML6075_CONFIG_UV_IT_100MS)

// Parameters for UVI calculation //
#define UVA_A_COEF  (2.22)
#define UVA_B_COEF  (1.33)
#define UVB_C_COEF  (2.95)
#define UVB_D_COEF  (1.74)
#define UVA_sensitivity (0.93)
#define UVA_CIE_sensitivity (0.093)
#define UVB_sensitivity (2.1)
#define UVB_CIE_sensitivity (0.21)
#define UVA_RESPONSIVITY (0.001461)
#define UVB_RESPONSIVITY (0.002591)

// Read failure code
#define VEML6075_READ_ERROR 0xFFFF

// END VEML6075 DEFINE //

// Function definitions
p_status AS726X_Write(uint8_t virtual_reg, uint8_t d);
p_status AS726X_Read(uint8_t virtual_reg, uint8_t *dataptr);
p_status AS726X_Readout_Raw_Single(uint16_t device_virtual_reg, uint16_t *dataptr);
p_status AS726X_Readout_Temp(uint8_t *dataptr);
p_status AS726X_Set_Config_Op_Mode(uint8_t opmode);
p_status AS726X_Set_Config_Int_Time(uint8_t int_time);

p_status VEML6075_Write(uint16_t register_value);
p_status VEML6075_Read(uint8_t register_pointer, uint8_t *dataptr);

p_status Multispectral_Init(void);
p_status Multispectral_Readout_Raw_All(uint16_t buf[22][5], uint8_t sensor_id);
#endif /* MULTISPECTRAL_H_ */
