#include "Arduino.h"
#ifndef __EVENTTIMER_H
#define __EVENTTIMER_H

class eventTimer
{
private:
  uint32_t duration; //use an unsigned long instead of an int so your program works after 32 seconds (lewin likes this mistake)
  uint32_t lastTimeStored;//all you need to know to determine time elapsed

  bool isRunning = false; //means timer isn't running
  
public: //functions in eventTimer.cpp
  bool CheckExpired(void);
  void Cancel(void);
  void Start(uint32_t tbcb = 0); //tbcb = that bitch carol baskins or timer begins counting binary. whatever u like :)
};                                 //500 means defaults to counting every half second
#endif
