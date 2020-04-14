#include "eventTimer.h"

  bool eventTimer::CheckExpired(void){
    if((millis()-lastTimeStored) >= duration && isRunning){
      lastTimeStored = millis();
      return true;  
    }
    else return false;
  }
  void eventTimer::Cancel(void){
    isRunning = false; //if timer isnt running
  }
  
  void eventTimer::Start(uint32_t tbcb){
    tbcb = duration;
    isRunning = true; //if timer is running
  }
