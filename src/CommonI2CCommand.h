#ifndef C_COMMON_I2C_COMMAND
#define C_COMMON_I2C_COMMAND
#include<Command.h>
#include<I2CUtil.h>

class CPinIO : public CommandProcessor
{
    public:
    CPinIO();
    int run();
    boolean parseParameters(int offset, Command *cmd);

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
   const char * version;
  public:
    

    CVersion(const char * v);
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
extern Counter I2CMessageCounter;
extern Counter PingCounter;

extern CI2CAddress I2CAddress;
extern CCPUSpeed CPUSpeed;
extern CPinIO PinIO;
#endif