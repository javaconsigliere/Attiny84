#include <I2CUtil.h>

#include <EEPROM.h>

#define CHECK_ARRAY_SIZE 5
#define I2C_DEFAULT_SLAVE_ADDRESS 0x4

I2C_Config::I2C_Config()
{
   
}


void I2C_Config::setAddress(uint8_t addr)
{
   uint8_t eepromAddress = CHECK_ARRAY_SIZE;
    if((addr > 0x02) && (addr < 0x78))
    { 
        for(int i = 0; i < eepromAddress; i++)
        {
            EEPROM.update(i, i);
        }
        EEPROM.update(eepromAddress, addr);
        i2cAddress = EEPROM.read(eepromAddress);
    }
}

uint8_t I2C_Config::getAddress()
{
  uint8_t eepromAddress = CHECK_ARRAY_SIZE;
  if (i2cAddress == 0)
  {
    
    uint8_t checkArray[CHECK_ARRAY_SIZE] = {0,0,0,0,0};
    // read 0 1 2 3 4 
    //
    for(int i = 0; i < eepromAddress; i++)
    {
      checkArray[i] = EEPROM.read(i);
      if(checkArray[i] != i)
      {
         i2cAddress = I2C_DEFAULT_SLAVE_ADDRESS; 
         setAddress(i2cAddress);
         break;
      }
    }
    i2cAddress = EEPROM.read(eepromAddress);
  }


  return i2cAddress;
}




void I2CUtil::write(uint8_t b)
{
   Wire.write(b);
}

void I2CUtil::write(const char * str)
{
  int len = strlen(str);
  if (len > 15)
    len = 15;
  I2CUtil::write((uint8_t)len);
 
  for(int i = 0 ; i < len ; i++)
  {
    Wire.write(str[i]);
  }
  
}

void I2CUtil::writeReverse(uint8_t *b, int len)
{
  for(int i = len -1 ; i > -1 ; i--)
  {
    I2CUtil::write(b[i]);
  }
}


void I2CUtil::write(float val)
{
  I2CUtil::writeReverse((uint8_t*) &val, 4);
}


void I2CUtil::write(long val)
{
   writeReverse((uint8_t*) &val, 4);
  // ((byte) ((val >> 24) & 0xff));
  // ((byte) ((val >> 16) & 0xff));
  // ((byte) ((val >> 8) & 0xff));
  // ((byte) ((val >> 0) & 0xff));
}


void I2CUtil::write(int val)
{
   I2CUtil::writeReverse((uint8_t*) &val, 2);
}

I2C_Config I2CConfig = I2C_Config();

// void wireWrite(byte b)
// {
// #ifdef TinyWireS_h
//     TinyWireS.send(b);
// #endif

//     Wire.write(b);

// }


// void wireWrite(byte *b, int len)
// {
//   for(int i = len; i > 0 ; i--)
//   {
//     wireWrite(b[i-1]);
//   }
// }

// void wireWrite(float val)
// {
//   wireWrite((byte*) &val, 4);
// }


// void wireWrite(long val)
// {
//    wireWrite((byte*) &val, 4);
//   // ((byte) ((val >> 24) & 0xff));
//   // ((byte) ((val >> 16) & 0xff));
//   // ((byte) ((val >> 8) & 0xff));
//   // ((byte) ((val >> 0) & 0xff));
// }

// void wireWrite(int val)
// {
//    wireWrite((byte*) &val, 2);
// }