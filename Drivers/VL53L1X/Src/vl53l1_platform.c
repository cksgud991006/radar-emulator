/**
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "vl53l1_platform.h"
#include <string.h>
#include <time.h>
#include <math.h>
#include "stm32f4xx_hal.h"

static const uint32_t timeout = 10;

extern I2C_HandleTypeDef hi2c1;

int8_t VL53L1_WriteMulti( uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
	uint8_t status = 255;
	
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
	
	status = HAL_I2C_Mem_Write(&hi2c1, dev, index, I2C_MEMADD_SIZE_16BIT, pdata, count, timeout);

	return status;
}

int8_t VL53L1_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count){
	uint8_t status = 255;
	
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
	
	status = HAL_I2C_Mem_Read(&hi2c1, dev, index, I2C_MEMADD_SIZE_16BIT, pdata, count, timeout);

	return status;
}

int8_t VL53L1_WrByte(uint16_t dev, uint16_t index, uint8_t data) {
	uint8_t status = 255;
	
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
	
	status = VL53L1_WriteMulti(dev, index, &data, 1);

	return status;
}

int8_t VL53L1_WrWord(uint16_t dev, uint16_t index, uint16_t data) {
	uint8_t status = 255;
	
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
	
	uint8_t buffer[2];
	buffer[0] = (uint8_t) (data >> 8);
	buffer[1] = (uint8_t) (data & 0x00ff);

	status = VL53L1_WriteMulti(dev, index, buffer, 2);

	return status;
}

int8_t VL53L1_WrDWord(uint16_t dev, uint16_t index, uint32_t data) {
	uint8_t status = 255;
	
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
	
	uint8_t buffer[4];
	buffer[0] = (uint8_t) (data >> 24);
	buffer[1] = (uint8_t) ((data >> 16) & 0x00ff);
	buffer[2] = (uint8_t) ((data >> 8) & 0x00ff);
	buffer[3] = (uint8_t) (data & 0x00ff);

	status = VL53L1_WriteMulti(dev, index, buffer, 4);

	return status;
}

int8_t VL53L1_RdByte(uint16_t dev, uint16_t index, uint8_t *data) {
	uint8_t status = 255;
	
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
	
	status = VL53L1_ReadMulti(dev, index, data, 1);

	return status;
}

int8_t VL53L1_RdWord(uint16_t dev, uint16_t index, uint16_t *data) {
	uint8_t status = 255;
	
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
	
	uint8_t buffer[2];

	status = VL53L1_ReadMulti(dev, index, buffer, 2);

	if (status == HAL_OK) {
		*data = (uint16_t) ((uint16_t) buffer[0] << 8 | (uint16_t) buffer[1]);
	}

	return status;
}

int8_t VL53L1_RdDWord(uint16_t dev, uint16_t index, uint32_t *data) {
	uint8_t status = 255;
	
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
	
	uint8_t buffer[4];

	status = VL53L1_ReadMulti(dev, index, buffer, 4);

	if (status == HAL_OK) {
		*data = (uint32_t) ((uint32_t) buffer[0] << 24 |
							(uint32_t) buffer[1] << 16 |
							(uint32_t) buffer[2] << 8  |
							(uint32_t) buffer[3]);
	}

	return status;
}

int8_t VL53L1_WaitMs(uint16_t dev, int32_t wait_ms){
	uint8_t status = 0;
	
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
	
	HAL_Delay(wait_ms);

	return status;
}
