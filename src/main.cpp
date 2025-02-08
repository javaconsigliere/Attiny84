#include <Arduino.h>
#include <I2CUtil.h>
#include <Command.h>
#include <CommonI2CCommand.h>
#include <ActionMonitor.h>


CActionMonitor *ActionMonitor = NULL;
/*
  The main program for attiny family slave i2c controller
*/


// #ifdef PINMAPPING_CCW
// #error "Sketch was written for clockwise pin mapping!"
// #endif


#if defined (__AVR_ATtiny84__)
#define DEVICE_MODEL "84-I2C"
#define RESET_PIN_CONTROLLER PB1
// ATTINY84 specififc
CReset Reset(RESET_PIN_CONTROLLER);

#elif defined (__AVR_ATtiny85__)   
#define  DEVICE_MODEL "85-I2C"
CActionMonitor oActionMonitor(500, PB1, PB3, PB4);
#endif






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


CVersion Version(DEVICE_MODEL,"1.06.04"); 
CUptime Uptime;
void setup()
{   


  // register the I2C commands
  // Count I2C messages
  CommandManager.add(&I2CMessageCounter);
  // Simple ping with a counter
  CommandManager.add(&PingCounter);
  // Get the code version of the controller
  CommandManager.add(&Version);
  // Return the uptime not really working
  // inaccuracy maybe caused by the I2C library
  CommandManager.add(&Uptime);
  // Return the current CPU speed in Hz
  CommandManager.add(&CPUSpeed);
  // Command the update the I2C address of the controller from 10 to 127
  // the new I2C address will take place after the restart the controller 
  CommandManager.add(&I2CAddress);
  // Setting and reading pin  digital or analog value, also provision the pin type
  // as analog or digital
  CommandManager.add(&PinIO);
  // Set the analog reference value internel or external and levels
  CommandManager.add(&ARef);

#if defined (__AVR_ATtiny84__) 
    // ATyiney84 support reset
    // other controller like ATyiney85 does not
    CommandManager.add(&Reset);
#endif

  // set the i2c protocol 
  I2CUtil::setup();  
  ARef.setArefType(DEFAULT);

#if defined (__AVR_ATtiny85__)   
  ActionMonitor = &oActionMonitor;
#endif

    //TCCR1B = TCCR1B & B11111000 | B00000001; //this changes the PWM frequency to 32kHz
}
 
void loop()
{
   
    
  // adding delays due to i2c issues
  #if defined (__AVR_ATtiny84__) 
    delay(1);
  #endif



  if (PendingRunnable == NULL && ActionMonitor != NULL && ActionMonitor->monitor(100))
      PendingRunnable = ActionMonitor;
  


  if(PendingRunnable != NULL)
  {
    PendingRunnable->run();
    PendingRunnable = NULL;
  }
  
  loopCounter++;
  
}
 