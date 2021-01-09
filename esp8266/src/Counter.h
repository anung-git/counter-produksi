#ifndef Counter_H_
#define Counter_H_
#include "Eprom.h"
class Counter
{
private:
    int count, minimum, maximum;
    Eprom *myeprom; // = Eprom();
    uint16_t getAddres(uint8_t tanggal, uint8_t bulan);

public:
    Counter(int min, int max);
    ~Counter();
    void incriment();
    void decriment();
    void setValue(unsigned int val);
    void reset();
    void saveToEprom( uint8_t tanggal, uint8_t bulan, uint16_t offsite);
    void getFromEprom(uint8_t tanggal, uint8_t bulan, uint16_t offsite);
    unsigned int getValue();
};

#endif
