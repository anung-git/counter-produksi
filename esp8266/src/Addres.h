#if !defined(Addres_H_)
#define Addres_H_

#include <Arduino.h>
class Addres
{
private:
    uint16_t start;
    /* data */
public:
    Addres(uint16_t addr);
    ~Addres();
    uint16_t mulai();
    uint16_t istirahat1();
    uint16_t istirahat2();
    uint16_t istirahat3();
    uint16_t masuk1();
    uint16_t masuk2();
    uint16_t masuk3();
    uint16_t selesai();
    uint16_t plan();

};

#endif // Addres_H_
