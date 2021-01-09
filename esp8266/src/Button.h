// #ifndef Button_H_
// #define Button_H_

// #endif

#if !defined(Button_H_)
#define Button_H_

#include "Arduino.h"

class Button
{
private:
    /* data */
    uint8_t pin;
    uint8_t state;
    unsigned long previousMillis;
    const unsigned long bouncing = 30;

public:
    Button(uint8_t pin);
    uint8_t getState();
    bool falling();
    bool rising();
    ~Button();
};

#endif // Button_H_
