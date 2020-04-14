#ifndef __BUTTON_H
#define __BUTTON_H
#include <Arduino.h>

 

class Button
{
private:
    enum BUTTON_STATE {BUTTON_STABLE, BUTTON_UNSTABLE};
    BUTTON_STATE state = BUTTON_STABLE;

    unsigned int buttonPin = -1;

   

    //assume active LOW
    unsigned int buttonPosition = HIGH; //most recent stable position
    unsigned int tempButtonPos = HIGH; //temporary position, bouncing not complete

    unsigned int lastBounceTime = 0;
    unsigned int debouncePeriod = 10; // in ms

 

public:

    Button(unsigned int pin, unsigned int db = 10); //default to 10 ms debounce
    void Init(boolean usePullup = true);
    bool CheckButtonPress(void);
};
#endif
