/*
 * QWIIC_MUX.h
 *
 *  Created on: 25 Mar 2019
 *      Author: Marcus Yeo Rui Han
 *      Modified by: Oh Zhe Hao
 */

#ifndef QWIIC_MUX_H_
#define QWIIC_MUX_H_

#include <string.h>
#include "component.h"
#include "sensor.h"
#include "main.h"
#include <stdio.h>
// Main Addresses to use
#define MASTER_ADDRESS 0x70
#define MASTER_READ 0xE1
#define MASTER_WRITE 0xE0

// Slave channels
#define DEFAULTCONTROL 0x00
#define CHANNEL0 0x01
#define CHANNEL1 0x02
#define CHANNEL2 0x04
#define CHANNEL3 0x08
#define CHANNEL4 0x10
#define CHANNEL5 0x20
#define CHANNEL6 0x40
#define CHANNEL7 0x80


HAL_StatusTypeDef enableMUXport(uint8_t channel);
HAL_StatusTypeDef disableMUXport(void);
uint8_t whichport(void);
#endif /* QWIIC_MUX_H_ */
