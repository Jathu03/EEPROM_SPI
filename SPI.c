#include "spi.h"

extern SPI_HandleTypeDef hspi1;

void SPI_Init(SPI_HandleTypeDef *hspi) {
    hspi = &hspi1;
    HAL_SPI_Init(hspi);
}

HAL_StatusTypeDef SPI_Transmit(uint8_t *data, uint16_t size) {
    return HAL_SPI_Transmit(&hspi1, data, size, HAL_MAX_DELAY);
}

HAL_StatusTypeDef SPI_Receive(uint8_t *data, uint16_t size) {
    return HAL_SPI_Receive(&hspi1, data, size, HAL_MAX_DELAY);
}
