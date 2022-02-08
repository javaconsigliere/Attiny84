#include <Arduino.h>
#include <I2CUtil.h>
#include <Command.h>
#include <CommonI2CCommand.h>
// adding support for stepper motor




// #ifdef PINMAPPING_CCW
// #error "Sketch was written for clockwise pin mapping!"
// #endif


#if defined (__AVR_ATtiny84__)
#define DEVICE_MODEL "84-I2C"
#define RESET_PIN_CONTROLLER PB0
CReset Reset(RESET_PIN_CONTROLLER);
#elif defined (__AVR_ATtiny85__)   
#define  DEVICE_MODEL "85-I2C"  
#endif
#define ADC_PIN_AREF A2
#define ADC_PIN A3
#define AREV_VOLTAGE 4.092000

// Number of time the loop function is called
long loopCounter = 0;
// Command object to store incoming i2c request or reads
Command command = Command();

EnumMap EMAref(C_CALIBRATE, "AREF");

EnumMap EMEcho(C_ECHO, "ECHO");


// EnumMap EMUsablePins[] =
// {
//   EnumMap(A1, "A1"),
//   EnumMap(A3, "A3"),
//   EnumMap(A5, "A5"),
//   EnumMap(A7, "A7"),
//   EnumMap(PB0, "PB0"),
//   EnumMap(PB2, "PB2"),
// };




class CAref:public CommandProcessor
{
  private:
    int aRefValue = 0;
    uint8_t referencePin;
  public:
    CAref(uint8_t pin):CommandProcessor(&EMAref)
    {
      referencePin = pin;
    }
    int run()
    {
      I2CUtil::write(OK);
      I2CUtil::write((uint8_t)':');
      I2CUtil::write((long)getAnalogAref());
      return OK;
    }

    int getAnalogAref()
    {
      aRefValue = analogRead(referencePin);
      return aRefValue;
    }
    int getAref()
    {
      return aRefValue;
    }
};
//////////////////////////////////////////////////////////////////////////////






// Gets called when the ATTiny receives an i2c request MASTER--> READ FROM I2C SLAVE DEVICE
void requestEvent()
{

   if(command.length() > 0)
   {
     //int index = 0;
     
     CommandProcessor *rp = CommandManager.parse(&command);
     if(rp != NULL)
     {
       int status = rp->run();
       if (status != OK)
       {
          I2CUtil::write(status);
          I2CUtil::write((uint8_t)':');
          I2CUtil::write(-1L);
       }
     }
    else
    {
      I2CUtil::write(NOT_FOUND);
      I2CUtil::write((uint8_t)':');
      I2CUtil::write(-1L);
    }
   }
   else
      I2CUtil::write(I2CConfig.getAddress());

   command.reset();

  
}
// Gets called when the ATTiny receives an i2c command or MASTER --> WRITE TO I2C SLAVE DEVICE
void receiveEvent(int howMany)
{
    if (howMany < 1)
    {
        return;// Sanity-check
    }
    
    I2CMessageCounter.inc();
  
    int counter = 0;
    uint8_t *c = command.getCommand();
    while(Wire.available() && counter < command.getSize())
    {
      c[counter++] = Wire.read();
    }
    command.setLength(counter);
}



CAref ARef(ADC_PIN_AREF);
CVersion Version(DEVICE_MODEL,"1.05.51"); 
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
  // CommandManager.add(&Echo);
  
  
  // set the analog reference to external
  

#if defined (__AVR_ATtiny84__) 
    //analogReference(EXTERNAL);
    CommandManager.add(&Reset);
    CommandManager.add(&ARef);
#endif

  

  
    
  Wire.begin(I2CConfig.getAddress()); // join i2c network
  Wire.onReceive(receiveEvent); // incoming 
  Wire.onRequest(requestEvent);

  // Turn on LED when program starts
  //pinMode(A1, OUTPUT);
  //pinMode(PB2, OUTPUT);
  // digitalWrite(PB2, HIGH);
  //digitalWrite(A1, HIGH);

  //ARef.getAnalogAref(); // ARef value
    
    

    //TCCR1B = TCCR1B & B11111000 | B00000001; //this changes the PWM frequency to 32kHz
}
 
void loop()
{
    // This needs to be here
    //TinyWireS_stop_check();
    // if(servoPos != servoOldPos)
    // {
    //   servo.write(servoPos);
    //   servoOldPos = servoPos;
    //   delay(25);
    // }
    
  // adding delays create i2c issues

  if(PostRun != NULL || PendingRunnable != NULL)
  {
    if(PostRun != NULL)
    {
      PostRun->postRun();
      PostRun = NULL;
    }
    if(PendingRunnable != NULL)
    {
      PendingRunnable->run();
      PendingRunnable = NULL;
    }
  }
  else
  {
  #if defined (__AVR_ATtiny84__) 
    delay(1);
  #endif
  }
  loopCounter++;
  
}
 