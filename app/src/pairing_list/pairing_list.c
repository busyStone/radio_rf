#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "pairing_list.h"
#include "eeprom.h"
#include <stdio.h>
#include <string.h>


I2C_INSTANCE_STRU eeprom_i2c;

int32_t pairing_list_addr_write(uint8_t rx_num, uint8_t *addr)
{
    uint8_t eeprom_addr;

    if (rx_num >= PAIRING_LIST_MAX_ADDR_NUM)
    {
        printf("[%s, L%d] rx num(%d) excess max.\r\n", __FILE__, __LINE__, rx_num);
        return -1;
    }
   
    eeprom_addr = PAIRING_LIST_ADDR_LEN * rx_num;
    (void)eeprom_write_bytes(&eeprom_i2c, eeprom_addr, addr, PAIRING_LIST_ADDR_LEN);

    return 0;
}

int32_t pairing_list_addr_read(uint8_t rx_num, uint8_t *addr)
{
    uint8_t eeprom_addr;

    if (rx_num >= PAIRING_LIST_MAX_ADDR_NUM)
    {
        printf("[%s, L%d] rx num(%d) excess max.\r\n", __FILE__, __LINE__, rx_num);
        return -1;
    }
      
    eeprom_addr = PAIRING_LIST_ADDR_LEN * rx_num;
    (void)eeprom_read_bytes(&eeprom_i2c, eeprom_addr, addr, PAIRING_LIST_ADDR_LEN);

    return 0;    
}

int32_t pairing_list_init(void)
{
    int32_t ret;
    I2C_WIRE_STRU i2c_wire;

    i2c_wire.scl.port = GPIOB;
    i2c_wire.scl.port_num = GPIO_PIN_6;
    
    i2c_wire.sda.port = GPIOB;
    i2c_wire.sda.port_num = GPIO_PIN_7;
    ret = i2c_init(&eeprom_i2c, &i2c_wire);
    if (I2C_RET_OK != ret)
    {
        printf("i2c init failed return %d.\r\n", (int)ret);
        return ret;
    }
    else
    {
        printf("i2c init ok!\r\n");
        return 0;
    }
}

void pairing_list_entry_show(uint8_t idx)
{
    uint8_t addr[PAIRING_LIST_ADDR_LEN] = {0, 0, 0, 0};
    (void)pairing_list_addr_read(idx, addr);

    printf("pairing list idx %d: %02x%02x%02x%02x \r\n",
        idx, addr[0], addr[1], addr[2], addr[3]);
    return;
}

