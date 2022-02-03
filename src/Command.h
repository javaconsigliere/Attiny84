#ifndef XLOGISTX_COMMAND_h
#define XLOGISTX_COMMAND_h

#include <DataTypes.h>
#include <Arduino.h>

#define COMMAND_BUFFER_SIZE 32



enum COMMAND
{
  C_CPU_SPEED,
  C_RESET,
  C_PING,
  C_MESSAGES,
  C_VERSION,
  C_UPTIME,
  C_IO,
  C_CALIBRATE,
  C_I2C_ADDRESS,
  C_ECHO
};

//extern EnumMap EMPing;
//extern EnumMap EMMessages;
// extern EnumMap EMAref;
// extern EnumMap EMReset;
// extern EnumMap EMCPUSpeed;
// extern EnumMap EMUptime;
//extern EnumMap EMVersion;
//extern EnumMap EMI2CAddress;
// extern EnumMap EMEcho;


class Command
{
  private:
    uint8_t data[COMMAND_BUFFER_SIZE];
	  int dataLen = 0;
    int size = COMMAND_BUFFER_SIZE;
	
  public:
    
 	  Command();
      uint8_t * getCommand();
      int length();
      int getSize();
      void setLength(int len);
      void reset();

};

class CommandProcessor : public Runnable
{
  
      
  protected:
    EnumMap *command;
     ACTION action;
  public:
    CommandProcessor(EnumMap *c);
    virtual boolean parseParameters(int offset, Command *cmd);
    // virtual int run() = 0;
    EnumMap *getCommand();
    void setAction(ACTION);
    ACTION getAction();
};


// class TokenMap
// {
//     private:
        
//         int map;
//         const char * token;
//         DataType dataType;

//     public:
//         TokenMap(int map, const char *token, const DataType dataType);
//         int getMap();
//         const char * getToken();
//         DataType getDataType();
// };


//typedef void (*PFUNC_PTR)(int, Command *,RunParameters *);




class CManager
{
  private:
    CommandProcessor** cProcessors;
  
    int size = 0;
    //RunParameters toRun;

    void reset();
  public:
   //CManager(int size);
   CManager(CommandProcessor **cp);
   void add(CommandProcessor *cp);

   CommandProcessor *parse(Command *command);
   
    
};


//extern CommandProcessor **cpList;
extern CManager CommandManager;

#endif