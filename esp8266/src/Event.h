#ifndef Event_H
#define Event_H

#include <Arduino.h>
class Event
{
private:
    unsigned long previousMillis, interval;
    bool run;

public:
    Event(int milli_second);
    bool getEvent();
    void setEvent(int milli_second);
    void start();
    void stop();
    long getInterval();
    ~Event();
};
// Existing code goes here

#endif