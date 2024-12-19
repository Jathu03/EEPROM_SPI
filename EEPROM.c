#include "eeprom.h"

void EEPROM_WriteEnable(void);
void EEPROM_Write(uint16_t addr, uint8_t *data, uint16_t length);
void EEPROM_Read(uint16_t addr, uint8_t *data, uint16_t length);
void EEPROM_CS_Select(void);
void EEPROM_CS_Deselect(void);
void EEPROM_WriteProtect_Disable(void);
void EEPROM_WriteProtect_Enable(void);


#define EEPROM_WP_PIN GPIO_PIN_9
#define EEPROM_WP_PORT GPIOA

void EEPROM_WriteProtect_Disable(void) {
    HAL_GPIO_WritePin(EEPROM_WP_PORT, EEPROM_WP_PIN, GPIO_PIN_SET);
}

void EEPROM_WriteProtect_Enable(void) {
    HAL_GPIO_WritePin(EEPROM_WP_PORT, EEPROM_WP_PIN, GPIO_PIN_RESET);
}

void EEPROM_WriteEnable(void) {
    uint8_t cmd = EEPROM_CMD_WREN;  // EEPROM_CMD_WREN = 0x06
    EEPROM_CS_Select();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    EEPROM_CS_Deselect();
}

void EEPROM_Write(uint16_t addr, uint8_t *data, uint16_t length) {
    uint16_t remaining = length;
    uint16_t offset = 0;
    uint8_t addr_high, addr_low;

    while (remaining > 0) {
        uint16_t to_write = (remaining > 256) ? 256 : remaining;

        // Disable write protect
        EEPROM_WriteProtect_Disable();

        // Enable write operations
        EEPROM_WriteEnable();

        // Calculate address
        addr_high = (addr >> 8) & 0xFF;
        addr_low = addr & 0xFF;

        uint8_t instruction[3] = {EEPROM_CMD_WRITE, addr_high, addr_low};  // EEPROM_CMD_WRITE = 0x02

        // Select EEPROM and send write command with address
        EEPROM_CS_Select();
        HAL_SPI_Transmit(&hspi1, instruction, 3, HAL_MAX_DELAY);

        // Transmit data
        HAL_SPI_Transmit(&hspi1, &data[offset], to_write, HAL_MAX_DELAY);
        EEPROM_CS_Deselect();

        // Wait for write to complete (polling WIP bit)
        uint8_t status;
        do {
            uint8_t cmd = EEPROM_CMD_RDSR;  // EEPROM_CMD_RDSR = 0x05
            EEPROM_CS_Select();
            HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
            HAL_SPI_Receive(&hspi1, &status, 1, HAL_MAX_DELAY);
            EEPROM_CS_Deselect();
        } while (status & 0x01);  // Check the Write-In-Progress (WIP) bit

        // Re-enable write protect
        EEPROM_WriteProtect_Enable();

        // Update remaining bytes and offset
        remaining -= to_write;
        offset += to_write;
        addr += to_write;
    }
}

void EEPROM_Read(uint16_t addr, uint8_t *data, uint16_t length) {
    uint8_t addr_high = (addr >> 8) & 0xFF;
    uint8_t addr_low = addr & 0xFF;

    uint8_t instruction[3] = {EEPROM_CMD_READ, addr_high, addr_low};  // EEPROM_CMD_READ = 0x03

    EEPROM_CS_Select();
    HAL_SPI_Transmit(&hspi1, instruction, 3, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, data, length, HAL_MAX_DELAY);
    EEPROM_CS_Deselect();
}
