/*
 * QWIIC_MUX_Lib.c
 *
 *  Created on: 17 Mar 2019
 *      Author: Marcus Yeo Rui Han
 *      Modified by: Oh Zhe Hao
 */

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "main.h"
#include "MCU_Init.h"
#include <stdio.h>
// Include the relevant sensor libraries below
#include "QWIIC_MUX.h"

// Function Definitions
HAL_StatusTypeDef enableMUXport(uint8_t channel);
HAL_StatusTypeDef disableMUXport(void);
uint8_t whichport(void);


//Sets multiplexer channel, takes in channel code defined in QWIIC_MUX.h
HAL_StatusTypeDef enableMUXport(uint8_t channel)
{
	// select channel based on user choice
	uint8_t *channelptr = (uint8_t *)(&channel);
	if(HAL_OK == HAL_I2C_Mem_Write(&hi2c3, MASTER_ADDRESS<<1, DEFAULTCONTROL, I2C_MEMADD_SIZE_8BIT, channelptr, 1, 10))
		return HAL_OK;
	else
		return HAL_ERROR;
}

//Disables all multiplexer channels
HAL_StatusTypeDef disableMUXport(void)
{
    uint8_t reset = DEFAULTCONTROL;
    uint8_t *resetptr = (uint8_t *)(&reset);
    // Reset control register to 0x00 ie. all channels disabled
    if(HAL_OK == HAL_I2C_Mem_Write(&hi2c3, MASTER_ADDRESS<<1, DEFAULTCONTROL, I2C_MEMADD_SIZE_8BIT, resetptr, 1, 10))
        return HAL_OK;
    else
        return HAL_ERROR;
}

//Returns port currently used. Not used.
uint8_t whichport(void){
    uint8_t data;
    uint8_t *dataptr = (uint8_t *)(&data);
    HAL_I2C_Master_Receive(&hi2c3, MASTER_ADDRESS<<1, dataptr, 1, 10);
    return data;
}



