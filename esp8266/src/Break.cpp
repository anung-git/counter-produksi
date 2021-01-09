#include"Break.h"

void Break::setStart(unsigned int time)
{
    this->start = time;
}
void Break::setStop(unsigned int time)
{
    this->stop = time;
}
void Break::setTime(unsigned char jam, unsigned char menit)
{
    this->time = (jam * 60) + menit;
}
bool Break::getState(void)
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

Break::Break(/* args */)
{
}

Break::~Break()
{
}
