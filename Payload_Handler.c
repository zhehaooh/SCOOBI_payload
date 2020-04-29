/*
 * Payload.c
 *
 *  Created on: 4 Mar 2020
 *      Author: Zhe Hao
 */
#include <Payload_Handler.h>

p_status Payload_Multispectral_Init_All (void);
p_status Payload_Multispectral_Read_All (uint16_t buf[MULTISPECTRAL_DATA_SIZE_ROWS][MULTISPECTRAL_DATA_SIZE_COLS]);
p_status Payload_Multispectral_Config_AS726X_Op_Mode (uint8_t buf[MULTISPECTRAL_DATA_SIZE_COLS]);
p_status Payload_Multispectral_Config_AS726X_Int_Time (uint8_t buf[MULTISPECTRAL_DATA_SIZE_COLS]);
p_status Payload_Multispectral_Config_VEML6075 (uint16_t buf[MULTISPECTRAL_DATA_SIZE_COLS]);

p_status Payload_Thermocouple_Init_All (void);
p_status Payload_Thermocouple_Read_All (uint16_t buf[THERMOCOUPLE_DATA_SIZE_ROWS][THERMOCOUPLE_DATA_SIZE_COLS]);

//Initialise all sensor settings
p_status Payload_Multispectral_Init_All (void)
{
	int fail_count = 0;

	if(disableMUXport() != HAL_OK) {
		fail_count++;
	}

	for(uint8_t sensor_id = 0; sensor_id<=MULTISPECTRAL_DATA_SIZE_COLS-1; sensor_id++){
		if(enableMUXport(CHANNEL0<<sensor_id) == HAL_OK) {
			if(INIT_ERROR == Multispectral_Init()) {
				fail_count++;
			}
		}
		else{
			fail_count++;
		}
	}
	if(disableMUXport() != HAL_OK) {
		fail_count++;
	}

	if(fail_count > 0) {
		return INIT_ERROR;
	}
	else {
		return INIT_SUCCESS;
	}
}

//Readout 5 sensors. Provide uint16_t buf[22][5]. MULTISPECTRAL_DATA_SIZE_ROWS & MULTISPECTRAL_DATA_SIZE_COLS defined in Payload_Handler.h
p_status Payload_Multispectral_Read_All (uint16_t buf[MULTISPECTRAL_DATA_SIZE_ROWS][MULTISPECTRAL_DATA_SIZE_COLS])
{
	int fail_count = 0;

	for(uint8_t sensor_id = 0; sensor_id<=MULTISPECTRAL_DATA_SIZE_COLS-1; sensor_id++){
		if(enableMUXport(CHANNEL0<<sensor_id) == HAL_OK) {
			if(DATA_ERROR == Multispectral_Readout_Raw_All(buf,sensor_id)) {
				fail_count++;
			}
		}
		else{
			fail_count++;
		}
	}

	if(disableMUXport() != HAL_OK) {
		fail_count++;
	}

	if(fail_count > 0) {
		return DATA_ERROR;
	}
	else {
		return DATA_SUCCESS;
	}
}

//Configure operation mode register for AS7265x on 5 sensors. Provide 5 element array containing settings for each sensor. Refer to datasheet for valid settings.
p_status Payload_Multispectral_Config_AS726X_Op_Mode (uint8_t buf[MULTISPECTRAL_DATA_SIZE_COLS])
{
	int fail_count = 0;

	for(uint8_t sensor_id = 0; sensor_id<=MULTISPECTRAL_DATA_SIZE_COLS-1; sensor_id++){
		if(enableMUXport(CHANNEL0<<sensor_id) == HAL_OK) {
			if(DATA_ERROR == AS726X_Write(AS726x_CONFIG_OPMODE,buf[sensor_id])) {
				fail_count++;
			}
		}
		else{
			fail_count++;
		}
	}

	if(disableMUXport() != HAL_OK) {
		fail_count++;
	}

	if(fail_count > 0) {
		return INIT_ERROR;
	}
	else {
		return INIT_SUCCESS;
	}
}

//Configure integration time register for AS7265x on 5 sensors. Provide 5 element array containing settings for each sensor. Refer to datasheet for valid settings.
p_status Payload_Multispectral_Config_AS726X_Int_Time (uint8_t buf[MULTISPECTRAL_DATA_SIZE_COLS])
{
	int fail_count = 0;

	for(uint8_t sensor_id = 0; sensor_id<=MULTISPECTRAL_DATA_SIZE_COLS-1; sensor_id++){
		if(enableMUXport(CHANNEL0<<sensor_id) == HAL_OK) {
			if(DATA_ERROR == AS726X_Write(AS726x_CONFIG_INT_TIME,buf[sensor_id])) {
				fail_count++;
			}
		}
		else{
			fail_count++;
		}
	}

	if(disableMUXport() != HAL_OK) {
		fail_count++;
	}

	if(fail_count > 0) {
		return INIT_ERROR;
	}
	else {
		return INIT_SUCCESS;
	}
}

//Configure operation mode register for VEML6075 on 5 sensors. Provide 5 element array containing settings for each sensor. Refer to datasheet for valid settings.
p_status Payload_Multispectral_Config_VEML6075 (uint16_t buf[MULTISPECTRAL_DATA_SIZE_COLS])
{
	int fail_count = 0;

	for(uint8_t sensor_id = 0; sensor_id<=MULTISPECTRAL_DATA_SIZE_COLS-1; sensor_id++){
		if(enableMUXport(CHANNEL0<<sensor_id) == HAL_OK) {
			if(INIT_ERROR == VEML6075_Write(buf[sensor_id])) {
				fail_count++;
			}
		}
		else{
			fail_count++;
		}
	}

	if(disableMUXport() != HAL_OK) {
		fail_count++;
	}

	if(fail_count > 0) {
		return INIT_ERROR;
	}
	else {
		return INIT_SUCCESS;
	}
}

