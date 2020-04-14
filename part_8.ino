#include <Zumo32U4Motors.h>
#include <Zumo32U4Encoders.h>
#include "Button.h"

#include "params.h"
#include "serial_comm.h"
#include "eventTimer.h"

volatile uint8_t readyToPID = 0;   //a flag that is set when the PID timer overflows

Zumo32U4Motors motors;
Zumo32U4Encoders encoders;

volatile int16_t countsLeft = 0;
volatile int16_t countsRight = 0;

Button buttonA(14);
eventTimer timer;

struct segment{
  int iSeg;
  float distance;
};

//array of segments from part 5, includes motor effort and time in ms
const int segCount = 5;
segment segments [segCount] = { 
{0,0}, 
{1,100},
{2,16},
{1,50},
{0,0}
};
//create global variables
bool started = false;
int step= 0;
int startDistance = 0;
float actualDistance = 0;

void setup()
{
  Serial.begin(115200);
  //while(!Serial) {}  //IF YOU DON'T COMMENT THIS OUT, YOU MUST OPEN THE SERIAL MONITOR TO START
  Serial.println("Hi.");

  noInterrupts(); //disable interupts while we mess with the Timer4 registers
  
  //sets up timer 4
  TCCR4A = 0x00; //disable some functionality -- no need to worry about this
  TCCR4B = 0x0C; //sets the prescaler -- look in the handout for values
  TCCR4C = 0x04; //toggles pin 6 at one-half the timer frequency
  TCCR4D = 0x00; //normal mode

  OCR4C = 0x74;   //TOP goes in OCR4C
  TIMSK4 = 0x04; //enable overflow interrupt
  
  interrupts(); //re-enable interrupts
  
//pinMode(6, OUTPUT); //COMMENT THIS OUT TO SHUT UP THE PIEZO!!!

}

void CalcPID()
{ 
  
    //for tracking previous counts
    static int16_t prevLeft = 0;
    static int16_t prevRight = 0;

    //error sum
    static int16_t sumLeft = 0; 
    static int16_t sumRight = 0; 
    //the compounded the previous and curtrent errors togethor: sumLeft  

    noInterrupts();//disable interrupts
    int16_t speedLeft = countsLeft - prevLeft;
    int16_t speedRight = countsRight - prevRight;
    actualDistance = (countsLeft - startDistance)*(85/4900);
    prevLeft = countsLeft;
    prevRight = countsRight;
    interrupts(); //re enable interupts

    int16_t errorLeft = targetLeft - speedLeft;
    sumLeft += errorLeft;
       if(sumLeft > 200)//if left motor is greater than half max effors
    {
      sumLeft = 200;   //set = to half max effort
    }
    else if (sumLeft < -200)//same but make it negative (reverse)
    {
      sumLeft = -200;  
    }

    int16_t errorRight = targetRight - speedRight;
    sumRight += errorRight;
    
    float effortLeft = Kp * errorLeft + Ki * sumLeft;
    float effortRight = Kp * errorRight + Ki * sumRight;
   
    // effort left is the adjusted moter value with the given error
    motors.setLeftSpeed(effortLeft);
    motors.setRightSpeed(effortRight);
   //motors.setSpeeds(effortRight, 0);
   //Serial.print(millis());
   //Serial.print('\t');
    Serial.print(targetLeft);
    Serial.print('\t');
    Serial.print(speedLeft);
    Serial.print('\t');
    Serial.print(effortLeft/10.0);
    Serial.print('\t');
    Serial.print(targetRight);
    Serial.print('\t');
    Serial.print(speedRight);
    Serial.print('\t');
    Serial.print(effortRight/10.0);

    Serial.print('\n');
}
  

void loop() 
{    
  if(readyToPID) //timer flag set
  {
    if(buttonA.CheckButtonPress())
    {
        if(!started)//once button A is pressed
        {
            started = true;//start
            step = 0;
            //timer.Start(segments[step].duration);
        }
    } 
    if(actualDistance >= segments[step].distance)
    {
       if (started)
       {
          step++;
          if(step == segCount)
          {
            Serial.println("finsihed");
            started = false; 
          }
         // timer.Start(segments[step].duration);
          startDistance = countsLeft;
          Serial.println(step);
       }
  }
    if(started)
    {
     Drive(segments[step].iSeg);   
    }
}
    //clear the timer flag
    readyToPID = 0;
  
  if(CheckSerialInput()) {ParseSerialInput();}
}

void Drive(int iSeg) //pass it the number of the Segment you wish to execute
{
  
  switch(iSeg)
  {
    case 0:
      motors.setSpeeds(0,0);
      break;
    case 1:
      targetLeft = 25;
      targetRight = 25;
      CalcPID();
      break;
    case 2:
      targetLeft = 25;
      targetRight = 0;
      CalcPID();
      break;
    
  }
}

ISR(TIMER4_OVF_vect)
{
  //Capture a "snapshot" of the encoder counts for later processing
  countsLeft = encoders.getCountsLeft();
  countsRight = encoders.getCountsRight();

  readyToPID = 1;
}
