#include "Button.h"

bool Button::falling()
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= bouncing)
    {
        previousMillis = currentMillis;
        uint8_t value = digitalRead(pin);
        if (state == HIGH and value == LOW)
        {
            state = value;
            return true;
        }
        else
        {
            state = value;
            return false;
        }
    }
    else
    {
        return false;
    }
}
bool Button::rising()
{

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= bouncing)
    {
        uint8_t value = digitalRead(pin);
        if (state == LOW and value == HIGH)
        {
            state = value;
            return true;
        }
        else
        {
            state = value;
            return false;
        }
    }
    else
    {
        return false;
    }
}

uint8_t Button::getState()
{
    state = digitalRead(pin);
    return state;
}
Button::Button(uint8_t pin)
{
    pinMode(pin, INPUT_PULLUP);
    Button::pin = pin;
    state = digitalRead(pin);
}

Button::~Button()
{
}
