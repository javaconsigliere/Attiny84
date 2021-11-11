#include <Arduino.h>
#include <I2CUtil.h>
#include <Command.h>




#define RESET_PIN_CONTROLLER PB1

#define ADC_PIN_AREF A2
#define ADC_PIN A3
#define AREV_VOLTAGE 4.092000


Command command = Command();


EnumMap EMUsablePins[] =
{
  EnumMap(A1, "A1"),
  EnumMap(A3, "A3"),
  EnumMap(A5, "A5"),
  EnumMap(A7, "A7"),
  EnumMap(PB0, "PB0"),
  EnumMap(PB2, "PB2"),
 
};










//////////////////////////////////////////////////////////////////////////////
class CVersion:public CommandProcessor
{
  private:
   const char * version;
  public:
    

    CVersion(const char * v):CommandProcessor(&EMVersion)
    {
      version = v;
    }
    int run()
    {
      I2CUtil::write(version);
      return OK;
    }
};
CVersion Version("84-I2C-1.04.23");
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
class CUptime:public CommandProcessor
{
  public:
    CUptime():CommandProcessor(&EMUptime)
    {
    }
    int run()
    {
      I2CUtil::write(OK);
      I2CUtil::write((uint8_t)':');
      I2CUtil::write((long)millis());
      return OK;
    }
};
CUptime Uptime;
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
class CPI2CAddress:public CommandProcessor
{
  private:
    uint16_t i2cAddress = 0;
  public:
    CPI2CAddress():CommandProcessor(&EMI2CAddress)
    {
    }
    boolean parseParameters(int offset, Command *cmd)
    {
      // I2C:S:address
      if(getAction() == S && offset + 1 < cmd->getSize())
      {
        //i2cAddress = BytesToPrimitive::toInt((cmd->getCommand() + offset));
        i2cAddress = cmd->getCommand()[offset++];
        if(i2cAddress > 7 && i2cAddress < 128)
          return true;
      }
      else if (getAction() == G)
      {
        return true;
      }
      return false;
    }
    int run()
    {
      uint8_t old = I2CConfig.getAddress();
      if(getAction() == S || getAction() == U)
      {
        I2CConfig.setAddress(i2cAddress);
      }

      I2CUtil::write(OK);
      I2CUtil::write((uint8_t)':');
      I2CUtil::write(old);
      if(getAction() == S || getAction() == U)
      {
        //I2CConfig.setAddress(i2cAddress);
        I2CUtil::write((uint8_t)':');
        I2CUtil::write(I2CConfig.getAddress());
      }
      return OK;
    }
};
CPI2CAddress I2CAddress;
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
class CPCPUSpeed:public CommandProcessor
{
  public:
    CPCPUSpeed():CommandProcessor(&EMCPUSpeed)
    {
    }
    int run()
    {
      I2CUtil::write(OK);
      I2CUtil::write((uint8_t)':');
      I2CUtil::write(F_CPU);
      return OK;
    }
};
CPCPUSpeed CPUSpeed;
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// echo the received value int, long or byte
class CPEcho:public CommandProcessor
{
  private:
    long value;
    int length;
    EnumMap *current;
  public:
    CPEcho():CommandProcessor(&EMEcho)
    {

    }
    boolean parseParameters(int offset, Command *cmd)
    {
      //ECHO:S:[I,L]:value
      value = 0;
      length = 0;
      current = NULL;
      if(getAction() == S && offset + 1 < cmd->getSize())
      {
        
        char * tok = (char *)cmd->getCommand();
        current = EnumMap::match(tok + offset, &EMInt, &EMLong, NULL);
        if(current != NULL)
        {
          offset+=2;
          switch(current->map())
          {
            case INT:
              value = BytesToPrimitive::toInt(cmd->getCommand() + offset);
              length = 2;
              break;
            case LONG:
              value = BytesToPrimitive::toLong(cmd->getCommand() + offset);
              length = 4;
              break;
          }
        }
      }
      return (length!=0);
    }

    int run()
    {
      // format OK:[I,L]:value
      I2CUtil::write(OK);
      I2CUtil::write((uint8_t)':');
      
      I2CUtil::write((uint8_t) current->name()[0]);
      I2CUtil::write((uint8_t)':');
      
      switch(current->map())
      { 
        case INT:
          I2CUtil::write((int) value);
          break;
        case LONG:
          I2CUtil::write(value);
          break;
      }
      return OK;
    }
};
CPEcho Echo;
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
class CPReset:public CommandProcessor
{
  public:
    CPReset():CommandProcessor(&EMReset)
    {
    }
    int run()
    {
      pinMode(RESET_PIN_CONTROLLER, OUTPUT);
      digitalWrite(RESET_PIN_CONTROLLER, LOW);
      return OK;
    }
};
CPReset Reset;
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

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
CAref ARef(ADC_PIN_AREF);
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
class Counter : public CommandProcessor
{
  private:
    long counter = 0;
    boolean incOnRun = false;
  public:
    Counter(EnumMap *c, boolean cType) : CommandProcessor(c)
    {
      incOnRun = cType;
    }
    int run()
    {
      long val = incOnRun ? inc() : get();
      I2CUtil::write(OK);
      I2CUtil::write((uint8_t)':');
      I2CUtil::write(val);
      
      return OK;
    }
    long inc(){return ++counter;}
    long get(){return counter;}
};
Counter I2CMessageCounter(&EMMessages, false);
Counter PingCounter(&EMPing, true);
//////////////////////////////////////////////////////////////////////////////





// Gets called when the ATTiny receives an i2c request
void requestEvent()
{
   if(command.length() > 0)
   {
     //int index = 0;
     
     CommandProcessor *rp = CommandManager.parse(&command);
     if(rp != NULL)
     {
      rp->run();
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

 
void setup()
{   
  // register the I2C commands
  CommandManager.add(&I2CMessageCounter);
  CommandManager.add(&PingCounter);
  CommandManager.add(&Version);
  CommandManager.add(&Uptime);
  CommandManager.add(&ARef);
  CommandManager.add(&CPUSpeed);
  CommandManager.add(&Reset);
  CommandManager.add(&I2CAddress);
  CommandManager.add(&Echo);
  
  // set the analog reference to external
  analogReference(EXTERNAL);

  

  
    
  Wire.begin(I2CConfig.getAddress()); // join i2c network
  Wire.onReceive(receiveEvent); // incoming 
  Wire.onRequest(requestEvent);

  // Turn on LED when program starts
  pinMode(A1, OUTPUT);
  digitalWrite(A1, HIGH);
  ARef.getAnalogAref(); // ARef value
    
    

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
  
}
 