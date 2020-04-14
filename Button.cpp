#include "Button.h"

Button::Button(uint8_t pin, uint32_t db){
  buttonPin = pin;
  debouncePeriod = db; 
}

void Button::Init(bool usePullup){
  if(usePullup){
    pinMode(buttonPin, INPUT_PULLUP);
  }
  else{
    pinMode(buttonPin, INPUT);
  }
}
//returns true when the button transitions from unpressed to pressed 
bool Button::CheckButtonPress(void){
  buttonPos = digitalRead(buttonPin);
  switch(state){
    case BUTTON_STABLE:
      if(buttonPos != lastButtonPos){
        lastBounceTime = millis();  
        state = BUTTON_UNSTABLE; 
      }
      return false;
      break;
    case BUTTON_UNSTABLE:                                            
      if((millis() - lastBounceTime) >= debouncePeriod){                
        if(buttonPos == HIGH && lastButtonPos == LOW){          
          lastButtonPos = buttonPos;                                
          state = BUTTON_STABLE;                                         
          return true;
        } 
        else{
          lastButtonPos = buttonPos;
          state = BUTTON_STABLE;
        }
        return false;
      }
      break;
  } 
}
