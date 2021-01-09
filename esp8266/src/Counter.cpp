#include "Counter.h"

uint16_t Counter::getAddres(uint8_t tanggal, uint8_t bulan)
{
    return (((bulan - 1) * 31) + (tanggal - 1)) * 4;
}

void Counter::saveToEprom(uint8_t tanggal, uint8_t bulan, uint16_t offsite)
{
    uint16_t addr = this->getAddres(tanggal, bulan) + offsite;
    myeprom->writeInt(0x50, addr, count);
    // Serial.print("save ");
    // Serial.print(offsite);
    // Serial.print(" = ");
    // Serial.println(count);
}
void Counter::getFromEprom(uint8_t tanggal, uint8_t bulan, uint16_t offsite)
{
    uint16_t addr = this->getAddres(tanggal, bulan) + offsite;
    count = myeprom->readInt(0x50,addr);


    // uint8_t msb = myeprom->readByte(0x50, addr);
    // uint8_t lsb = myeprom->readByte(0x50, addr + 1);
    // count = (msb << 8);
    // count |= lsb;

    // if (count > 9999 || count < 0)
    // {
    //     count = 0;
    //     this->saveToEprom(tanggal, bulan, offsite);
    // }
    // Serial.print("get ");
    // Serial.print(offsite);
    // Serial.print(" = ");
    // Serial.println(count);
}
void Counter::reset()
{
    count = minimum;
}
void Counter::incriment()
{
    count++;
    if (count > maximum)
    {
        count = minimum;
    }
}
void Counter::decriment()
{
    count--;
    if (count < minimum)
    {
        count = maximum;
    }
}
void Counter::setValue(unsigned int val)
{
    count = val;
}
unsigned int Counter::getValue()
{
    return count;
}

Counter::Counter(int min, int max)
{
    myeprom = new Eprom();
    minimum = min;
    maximum = max;
}

Counter::~Counter()
{
}
