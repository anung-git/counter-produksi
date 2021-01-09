#include "Addres.h"
uint16_t Addres::mulai()
{
    return (start);
}
uint16_t Addres::istirahat1()
{
    return (start + 2);
}
uint16_t Addres::istirahat2()
{
    return (start + 4);
}
uint16_t Addres::istirahat3()
{
    return (start + 6);
}
uint16_t Addres::masuk1()
{
    return (start + 8);
}
uint16_t Addres::masuk2()
{
    return (start + 10);
}
uint16_t Addres::masuk3()
{
    return (start + 12);
}
uint16_t Addres::selesai()
{
    return (start + 14);
}
uint16_t Addres::plan()
{
    return (start + 16);
}

Addres::Addres(uint16_t addr)
{
    start = addr;
}

Addres::~Addres()
{
}
