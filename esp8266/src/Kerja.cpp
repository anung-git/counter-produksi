#include"Kerja.h"

void Kerja::setStart(unsigned char jam, unsigned char menit)
{
    this->start = (jam * 60) + menit;
}
void Kerja::setStop(unsigned char jam, unsigned char menit)
{
    this->stop = (jam * 60) + menit;
}
void Kerja::setTime(unsigned char jam, unsigned char menit)
{
    this->time = (jam * 60) + menit;
}
bool Kerja::getState(void)
{
    if (time >= start && time < stop)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Kerja::Kerja(/* args */)
{
}

Kerja::~Kerja()
{
}
