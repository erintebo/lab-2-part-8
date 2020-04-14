#include "button.h"

Button::Button (unsigned int pin, unsigned int db = 10) //default to 10 ms debounce
{
    this->buttonPin = pin;
    this->debouncePeriod = db; 
}

void Button::Init(bool usePullup = true)
{
    if (usePullup) pinMode(buttonPin, INPUT_PULLUP);
    else pinMode(buttonPin, INPUT);
}
bool Button::CheckButtonPress(void)
{
      if (tempButtonPos != digitalRead(buttonPin))
      {
         tempButtonPos = digitalRead(buttonPin);
         state = BUTTON_UNSTABLE; 

      }
      switch (state)
      {
          case BUTTON_UNSTABLE:
              if (millis() > debouncePeriod + lastBounceTime)
              {
                  state = BUTTON_STABLE;
                  return false; 
              }
              else
              {
                  if (tempButtonPos != digitalRead(buttonPin))
                  {
                     tempButtonPos = digitalRead(buttonPin);
                     lastBounceTime = millis();
                     state = BUTTON_UNSTABLE;  
                  }
                  return false;
              }
              break;
          case BUTTON_STABLE:
              if (buttonPosition != tempButtonPos && tempButtonPos == LOW)
              {
                  buttonPosition = tempButtonPos;
                  return true;
              }
              else
              {
                  buttonPosition = tempButtonPos;
                  return false;  
              }
              break;

 

         

    }

}
