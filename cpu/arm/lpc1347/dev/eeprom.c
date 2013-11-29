#include "dev/eeprom.h"
#include "eeprom/eeprom.h"

void
eeprom_write(eeprom_addr_t addr, unsigned char *buf, int size)
{
    if (addr + size > EEPROM_SIZE)
    {
        return;
    }

    writeEEPROM(addr, buf, size);
}

void
eeprom_read(eeprom_addr_t addr, unsigned char *buf, int size)
{
    if (addr + size > EEPROM_SIZE)
    {
        return;
    }

    readEEPROM(addr, buf, size);
}

void
eeprom_init(void)
{

}
