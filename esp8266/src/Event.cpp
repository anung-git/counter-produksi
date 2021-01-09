#include "Event.h"
bool Event::getEvent()
{
    if (run == false)
    {
        return false;
    }

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        return true;
    }
    else
        return false;
}
void Event::setEvent(int milli_second)
{
    this->interval = milli_second;
    this->previousMillis = millis();
}
void Event::start()
{
    if (run == false)
    {
        run = true;
        this->previousMillis = millis();
    }
}

long Event::getInterval()
{
    return interval;
}
void Event::stop()
{
    run = false;
}

Event::Event(int milli_second)
{
    this->interval = milli_second;
    this->previousMillis = millis();
}

Event::~Event()
{
}
