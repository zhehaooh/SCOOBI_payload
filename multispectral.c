/*
 * multispectral.c
 *
 *  Created on: 4 Mar 2020
 *      Author: Zhe Hao
 */

#include <multispectral.h>

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

//Writes data to VEML6075 register. Only writable register is configuration register, hence no register address required. Takes data to be written. Used to write configuration settings.
p_status VEML6075_Write(uint16_t register_value)
{
    uint8_t *regptr = (uint8_t *)(&register_value);

    if(HAL_OK == HAL_I2C_Mem_Write(&hi2c3, VEML6075_I2C_ADDRESS<<1, VEML6075_CONFIG, I2C_MEMADD_SIZE_8BIT, regptr, 2, 10)) {
    	return INIT_SUCCESS;
    }
    else return INIT_ERROR;
}

//Reads data from VEML6075 register. Takes address of register, and address to store read data.
p_status VEML6075_Read(uint8_t register_pointer, uint8_t *dataptr)
{
    if(HAL_OK == HAL_I2C_Mem_Read(&hi2c3, VEML6075_I2C_ADDRESS<<1, register_pointer, I2C_MEMADD_SIZE_8BIT, dataptr, 2, 10)){
    	return DATA_SUCCESS;
    }
    else{
    	*dataptr = VEML6075_READ_ERROR;
    	return DATA_ERROR;
    }
}

//Writes data to AS726X virtual registers. Takes virtual register address, and data to be written.
p_status AS726X_Write(uint8_t virtual_reg, uint8_t d)
{
    int counter1, counter2;
    volatile uint8_t status;
    uint8_t *statptr = (uint8_t *)(&status);
    uint8_t virtual_regW = virtual_reg|0x80;
    uint8_t *virtual_regWptr = (uint8_t *)(&virtual_regW);
    uint8_t *dataptr = (uint8_t *)(&d);
    counter1 = 0;
    counter2 = 0;

    // Infinite loop to catch errors; Runs three times if not, exit function completely
    while(1)
    {
        if(HAL_OK == HAL_I2C_Mem_Read(&hi2c3, AS726X_I2C_ADDR<<1, AS72XX_OPERATION_STATUS_REG, I2C_MEMADD_SIZE_8BIT, statptr, 1, 10)){
        	if((status & AS72XX_OPERATION_TX_VALID) == 0){
				break;
        	}
        }
        counter1++;
        if(counter1 > AS726x_I2C_RW_RETRIES)
        {
            return DATA_ERROR;
        }
    }

    if(HAL_OK != HAL_I2C_Mem_Write(&hi2c3, AS726X_I2C_ADDR<<1, AS72XX_OPERATION_WRITE_REG, I2C_MEMADD_SIZE_8BIT, virtual_regWptr, 1, 10 )){
    	return DATA_ERROR;
    }

    // Second check on write register; is mounted write register ready to write data?
    while(1)
    {
        if(HAL_OK == HAL_I2C_Mem_Read(&hi2c3, AS726X_I2C_ADDR<<1, AS72XX_OPERATION_STATUS_REG, I2C_MEMADD_SIZE_8BIT, statptr, 1, 10))
        	if((status & AS72XX_OPERATION_TX_VALID) == 0){
				break;
			}
        counter2++;
        if(counter2 > AS726x_I2C_RW_RETRIES)
        {
            return DATA_ERROR;
        }
    }

    if(HAL_OK == HAL_I2C_Mem_Write(&hi2c3, AS726X_I2C_ADDR<<1, AS72XX_OPERATION_WRITE_REG, I2C_MEMADD_SIZE_8BIT, dataptr, 1, 10 )){
    	return DATA_SUCCESS;
    }
    return DATA_ERROR;
}

//Reads data from AS726X virtual registers. Takes address of virtual register, and address to store read data.
p_status AS726X_Read(uint8_t virtual_reg, uint8_t *dataptr)
{
    int counter1 =0;
    int counter2 =0;
    *dataptr = AS726x_READ_ERROR;
    volatile uint8_t Rstatus;
    uint8_t *Rstatptr = (uint8_t *)(&Rstatus);
    uint8_t *virtual_reg_ptr = (uint8_t *)(&virtual_reg);

    while(1)
    {
        if(HAL_OK == HAL_I2C_Mem_Read(&hi2c3, AS726X_I2C_ADDR<<1, AS72XX_OPERATION_STATUS_REG, I2C_MEMADD_SIZE_8BIT, Rstatptr, 1, 10)){
			if((Rstatus & AS72XX_OPERATION_TX_VALID) == 0){
			   break;
			}
        }
		counter1++;
		if(counter1 > AS726x_I2C_RW_RETRIES){
			return DATA_ERROR;
		}
    }

    // Check for leftover data
	if((Rstatus & AS72XX_OPERATION_RX_VALID) != 0){
		if(HAL_OK != HAL_I2C_Mem_Read(&hi2c3, AS726X_I2C_ADDR<<1, AS72XX_OPERATION_READ_REG, I2C_MEMADD_SIZE_8BIT, dataptr, 1, 10)){
			return DATA_ERROR;
		}
	}

    if(HAL_OK != HAL_I2C_Mem_Write(&hi2c3, AS726X_I2C_ADDR<<1, AS72XX_OPERATION_WRITE_REG, I2C_MEMADD_SIZE_8BIT, virtual_reg_ptr, 1, 10 )){
    	return DATA_ERROR;
    }

    while(1)
    {
        if(HAL_OK == HAL_I2C_Mem_Read(&hi2c3, AS726X_I2C_ADDR<<1, AS72XX_OPERATION_STATUS_REG, I2C_MEMADD_SIZE_8BIT, Rstatptr, 1, 10)){
			if((Rstatus & AS72XX_OPERATION_RX_VALID) != 0){
			   break;       // Data is ready to be read
			}
        }
        counter2++;
		if(counter2 > AS726x_I2C_RW_RETRIES){
		   return DATA_ERROR;
		}
    }
    if(HAL_OK == HAL_I2C_Mem_Read(&hi2c3, AS726X_I2C_ADDR<<1, AS72XX_OPERATION_READ_REG, I2C_MEMADD_SIZE_8BIT, dataptr, 1, 10)){
        return DATA_SUCCESS;
    }
    *dataptr = AS726x_READ_ERROR;
    return DATA_ERROR;
}

//Reads single channel data from AS726X. Takes combined device/register address defined in multispectral.h and address to store read data. Not used.
p_status AS726X_Readout_Raw_Single(uint16_t device_virtual_reg, uint16_t *dataptr)
{
	uint8_t return_status = DATA_ERROR;

	uint8_t readout_raw_h = AS726x_READ_ERROR;
	uint8_t readout_raw_l = AS726x_READ_ERROR;

	uint8_t device_sel = (uint8_t) (device_virtual_reg>>8);
	uint8_t reg_sel = (uint8_t) (device_virtual_reg & 0x00FF);

	if (INIT_SUCCESS == AS726X_Write(AS726x_CONFIG_DEV_SEL, device_sel)) {
		if (DATA_SUCCESS == AS726X_Read(reg_sel,&readout_raw_h) && DATA_SUCCESS == AS726X_Read(reg_sel+1,&readout_raw_l)){
			return_status = DATA_SUCCESS;
		}
		else{
			readout_raw_h = AS726x_READ_ERROR;
			readout_raw_l = AS726x_READ_ERROR;
		}
	}
	*dataptr = (uint16_t) readout_raw_h << 8 | (uint16_t) readout_raw_l;

	return return_status;
}

//Reads temperature register. Takes address to store read data. Not used.
p_status AS726X_Readout_Temp(uint8_t *dataptr)
{
    int fail_count = 0;

    if(AS726X_Read(AS726x_DEVICE_TEMP, dataptr) == DATA_ERROR){
    	fail_count++;
    }

    if(fail_count >=1){
        return DATA_ERROR;
    }
    else{
        return DATA_SUCCESS;
    }
}

//Initialises single sensor board, consisting of AS7265x & VEML6075. Configuration settings stored in multispectral.h.
p_status Multispectral_Init(void)
{
    int fail_count = 0;
    int counter1 = 0;
    uint8_t *tempptr = 0;

    if(HAL_OK != HAL_I2C_IsDeviceReady(&hi2c3, AS726X_I2C_ADDR<<1, 5, 10) || HAL_OK != HAL_I2C_IsDeviceReady(&hi2c3, VEML6075_I2C_ADDRESS<<1, 5, 10)){
    	return INIT_ERROR;
    }

    // Sets AS726x_CONFIG_OPMODE
    if(AS726X_Write(AS726x_CONFIG_OPMODE, AS726x_CONFIG_OPMODE_INIT) == DATA_ERROR){
    	fail_count++;
    }
    // Sets AS726x_CONFIG_INT_TIME
    if(AS726X_Write(AS726x_CONFIG_INT_TIME, AS726x_CONFIG_INT_TIME_INIT) == DATA_ERROR){
    	fail_count++;
    }
    // Sets VEML6075_CONFIG
    if(VEML6075_Write(VEML6075_CONFIG_DEFAULT) == INIT_ERROR){
    	fail_count++;
    }

    if(fail_count >=1){
        return INIT_ERROR;
    }
    else{
        return INIT_SUCCESS;
    }
}

//Readouts raw values from single sensor board, consisting of AS7265x & VEML6075.
p_status Multispectral_Readout_Raw_All(uint16_t buf[22][5], uint8_t sensor_id)
{
	int fail_count = 0;
	uint8_t readout_raw_h = AS726x_READ_ERROR;
	uint8_t readout_raw_l = AS726x_READ_ERROR;
	uint8_t veml_read_addr[4] = {VEML6075_READ_UVA, VEML6075_READ_UVB, VEML6075_READ_UVCOMP1, VEML6075_READ_UVCOMP2};
	uint8_t readout_counter = 0;

	for (uint8_t device_sel = AS726x_CONFIG_DEV_SEL_START; device_sel <= AS726x_CONFIG_DEV_SEL_END; device_sel++) {
		if (DATA_SUCCESS == AS726X_Write(AS726x_CONFIG_DEV_SEL, device_sel)) {
			for (uint8_t reg_sel = AS726x_READ_RAW_START; reg_sel <= AS726x_READ_RAW_END; reg_sel = reg_sel + 2) {
				if (DATA_ERROR == AS726X_Read(reg_sel,&readout_raw_h) || DATA_ERROR == AS726X_Read(reg_sel+1,&readout_raw_l)){
					fail_count++;
				}
				buf[readout_counter][sensor_id] = (uint16_t) readout_raw_h << 8 | (uint16_t) readout_raw_l;
				readout_counter++;
			}
		}
		else {
			readout_raw_h = AS726x_READ_ERROR;
			readout_raw_l = AS726x_READ_ERROR;
			for (uint8_t reg_sel = AS726x_READ_RAW_START; reg_sel <= AS726x_READ_RAW_END; reg_sel = reg_sel + 2) {
				buf[readout_counter][sensor_id] = (uint16_t) readout_raw_h << 8 | (uint16_t) readout_raw_l;
				readout_counter++;
			}
			fail_count++;
		}
	}

	for (uint8_t veml_reg_sel = 0; veml_reg_sel < sizeof(veml_read_addr) / sizeof(uint8_t); veml_reg_sel++){
		if(DATA_ERROR == VEML6075_Read(veml_read_addr[veml_reg_sel],(uint8_t *)(&buf[readout_counter][sensor_id]))){
			fail_count++;
		}
		readout_counter++;
	}

	if(fail_count >=1){
		return DATA_ERROR;
	}
	else{
		return DATA_SUCCESS;
	}

}


