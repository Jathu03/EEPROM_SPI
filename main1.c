#include "main.h"
#include "spi.h"
#include "eeprom.h"

SPI_HandleTypeDef hspi1;  // Define SPI handle

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();

    SPI_Init(&hspi1);
    EEPROM_Init();

    uint8_t write_data[EEPROM_PAGE_SIZE] = {0x01, 0x02, 0x03};  // Example data
    uint8_t read_data[EEPROM_PAGE_SIZE] = {0};

    if (EEPROM_WritePage(0, write_data) == HAL_OK) {
        if (EEPROM_ReadPage(0, read_data) == HAL_OK) {
            // Validate read_data
        }
    }

    while (1) {}
}


