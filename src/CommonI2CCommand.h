#ifndef C_COMMON_I2C_COMMAND
#define C_COMMON_I2C_COMMAND
#include<Command.h>
#include<I2CUtil.h>

class CPinIO : public CommandProcessor
{
  private:
    IO_TYPE iot;
    uint8_t pin;
    
    int setValue;

  public:
    CPinIO();
    int run();
    boolean parseParameters(int offset, Command *cmd);
    IO_TYPE getIOT();
    uint8_t getPin();
    int getValue();

};


class CCPUSpeed:public CommandProcessor
{
  public:
    CCPUSpeed();
    int run();
    
};



class CVersion:public CommandProcessor
{
  private:
   const char *model;
   const char * version;
   int length;
  public:
    

    CVersion(const char *m, const char * v);
    int run();
};

/**
 * @brief Set the I2C address of the device
 * The device must restart after setting
 * 
 */
class CI2CAddress : public CommandProcessor
{
    private:
        uint16_t i2cAddress = 0;
    public:
        CI2CAddress();
        int run();
        boolean parseParameters(int offset, Command *cmd);

};


class CWriteRegister : public CommandProcessor
{
    private:
    public:
      CWriteRegister();
      int run();
      boolean parseParameters(int offset, Command *cmd);
};

class Counter : public CommandProcessor
{
  private:
    long counter = 0;
    boolean incOnRun = false;
  public:
    Counter(EnumMap *c, boolean cType);
    int run();
    
    long inc();
    long get();
};

class CUptime : public CommandProcessor
{
  public:
    CUptime();
    int run();
};

class CReset : public CommandProcessor
{
  private:
    uint8_t resetPin;

  public:
    CReset(uint8_t pin);
    int run();
};

class CAref:public CommandProcessor
{
  private:
    int arefType = DEFAULT;
  public:
    CAref();
   
    int run();
    boolean parseParameters(int offset, Command *cmd);

    int getArefType();
    void setArefType(int arefType);
    
};




extern Counter I2CMessageCounter;
extern Counter PingCounter;

extern CI2CAddress I2CAddress;
extern CCPUSpeed CPUSpeed;
extern CPinIO PinIO;
extern CAref ARef;

// This is used to execute a command inside the main loop function
// if PostRun is not NULL
#endif