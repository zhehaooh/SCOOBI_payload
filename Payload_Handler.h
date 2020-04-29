/*
 * Payload.h
 *
 *  Created on: 4 Mar 2020
 *      Author: Zhe Hao
 */

#ifndef PAYLOAD_H_
#define PAYLOAD_H_

#include "multispectral.h"
#include "QWIIC_MUX.h"

#define MULTISPECTRAL_DATA_SIZE_ROWS 22
#define MULTISPECTRAL_DATA_SIZE_COLS 5
#define THERMOCOUPLE_DATA_SIZE_ROWS 4
#define THERMOCOUPLE_DATA_SIZE_COLS 16

#endif /* PAYLOAD_H_ */

// Function definitions
p_status Payload_Multispectral_Init_All (void);
p_status Payload_Multispectral_Read_All (uint16_t buf[MULTISPECTRAL_DATA_SIZE_ROWS][MULTISPECTRAL_DATA_SIZE_COLS]);
p_status Payload_Multispectral_Config_AS726X_Op_Mode (uint8_t buf[MULTISPECTRAL_DATA_SIZE_COLS]);
p_status Payload_Multispectral_Config_AS726X_Int_Time (uint8_t buf[MULTISPECTRAL_DATA_SIZE_COLS]);
p_status Payload_Multispectral_Config_VEML6075 (uint16_t buf[MULTISPECTRAL_DATA_SIZE_COLS]);

p_status Payload_Thermocouple_Init_All (void);
p_status Payload_Thermocouple_Read_All (uint16_t buf[THERMOCOUPLE_DATA_SIZE_ROWS][THERMOCOUPLE_DATA_SIZE_COLS]);
