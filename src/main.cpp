#include <Arduino.h>
#include <I2CUtil.h>
#include <Command.h>
#include <CommonI2CCommand.h>





// #ifdef PINMAPPING_CCW
// #error "Sketch was written for clockwise pin mapping!"
// #endif


#if defined (__AVR_ATtiny84__)
#define DEVICE_MODEL "84-I2C"
#define RESET_PIN_CONTROLLER PB0
// ATTINY84 specififc
#define ADC_PIN_AREF A2
CAref ARef(ADC_PIN_AREF);
CReset Reset(RESET_PIN_CONTROLLER);

#elif defined (__AVR_ATtiny85__)   
#define  DEVICE_MODEL "85-I2C"  
#endif



#define ADC_PIN A3
#define AREV_VOLTAGE 4.092000

// Number of time the loop function is called
long loopCounter = 0;
// Command object to store incoming i2c request or reads






// EnumMap EMUsablePins[] =
// {
//   EnumMap(A1, "A1"),
//   EnumMap(A3, "A3"),
//   EnumMap(A5, "A5"),
//   EnumMap(A7, "A7"),
//   EnumMap(PB0, "PB0"),
//   EnumMap(PB2, "PB2"),
// };


CVersion Version(DEVICE_MODEL,"1.05.64"); 
CUptime Uptime;
void setup()
{   
  // register the I2C commands
  CommandManager.add(&I2CMessageCounter);
  CommandManager.add(&PingCounter);
  CommandManager.add(&Version);
  CommandManager.add(&Uptime);
  CommandManager.add(&CPUSpeed);
  CommandManager.add(&I2CAddress);
  CommandManager.add(&PinIO);  

#if defined (__AVR_ATtiny84__) 
    // set the analog reference to external
    //analogReference(EXTERNAL);
    CommandManager.add(&Reset);
    CommandManager.add(&ARef);
#endif


    
  // set the i2c protocol 
  I2CUtil::setup();

  //ARef.getAnalogAref(); // ARef value
    
    

    //TCCR1B = TCCR1B & B11111000 | B00000001; //this changes the PWM frequency to 32kHz
}
 
void loop()
{
   
    
  // adding delays create i2c issues
  #if defined (__AVR_ATtiny84__) 
    delay(1);
  #endif
  if(PendingRunnable != NULL)
  {
    PendingRunnable->run();
    PendingRunnable = NULL;
  }
  
  loopCounter++;
  
}
 