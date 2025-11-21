#ifndef I2CUtil_h
#define I2CUtil_h

#include <Wire.h>
#include <Arduino.h>
#include<Command.h>
#include<CommonI2CCommand.h>


class I2CUtil
{

  private:
  

  public:
    static void write(uint8_t b);
    static void write(int i);
    static void write(long l);
    static void write(float f);
    static void write(const char * str);
    static void write(bool writeLength, const char * str);
    static void flush();
    
    static void writeReverse(uint8_t *b, int len);
    static void request(int);
    static void response();
    static void setup();
};


class I2C_Config
{
  private:
	  uint8_t i2cAddress = 0;
	
  public:
	I2C_Config();
	void setAddress(uint8_t I2CSlaveAddres);
    uint8_t getAddress();
};


extern I2C_Config I2CConfig;


#endif