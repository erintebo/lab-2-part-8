#include "Arduino.h"
#ifndef __BUTTON_H
#define __BUTTON_H

class Button{
  private:
    uint8_t buttonPin = 14; //button A on the Romi
    uint32_t debouncePeriod = 10; // in ms
    uint32_t lastBounceTime = 0;
     
    uint8_t buttonPos = HIGH; //most recent stable position
    uint8_t lastButtonPos = HIGH; //temporary position, bouncing not complete
    
    enum BUTTON_STATE {BUTTON_STABLE, BUTTON_UNSTABLE};
    BUTTON_STATE state = BUTTON_STABLE; 
    
  public:
    Button(uint8_t pin, uint32_t db = 10);
    void Init(bool usePullup = true);
    bool CheckButtonPress(void);  
};
#endif
