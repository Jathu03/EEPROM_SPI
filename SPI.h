#ifndef SPI_H
#define SPI_H

#include "stm32f105xx_hal.h"

void SPI_Init(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef SPI_Transmit(uint8_t *data, uint16_t size);
HAL_StatusTypeDef SPI_Receive(uint8_t *data, uint16_t size);

#endif // SPI_H
