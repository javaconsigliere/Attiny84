#ifndef XLOGISTX_DATA_TYPES_h
#define XLOGISTX_DATA_TYPES_h
#include<Arduino.h>


// this voltable value is used when voltage source chip is on the board
#define AREF_VOLTAGE 4.092000


class EnumMap
{
  private:
    const char *iName;
    int iMap;
  public:
    EnumMap(int map, const char *name);
    const char *name();
    int map();

    static EnumMap * match(const char *token, ...);
};

enum DATA_TYPE
{
  BYTE,   // one byte, 8 bits
  INT,    // integer, 16 bits
  LONG,   // long, 32 bits
  FLOAT,  // float, 32 bits
  DOUBLE, // double, 64 bits
  STRING,
  ENUM
};


enum ACTION
{
 A_G, // get
 A_S, // set
 A_U, // update
 A_D,
 A_P
};

enum IO_TYPE
{
  IO_A, // analog
  IO_D, // digital
};



enum HTTP_STATUS
{
  OK=200,
  BAD_REQUEST=400,
  UNAUTORIZED=401,
  FORBIDDEN=403,
  NOT_FOUND=404,
  CONFLICT=409,
  PRECONDITION_FAILED=412,

};

class Runnable
{
  public:
  virtual int run() = 0;
};






class BytesToPrimitive
{
  public:
  int static toInt(const uint8_t *);
  long static toLong(const uint8_t *);
  // float static toFloat(const uint8_t *);
};

extern EnumMap EMaps[];
extern EnumMap EMByte;
extern EnumMap EMInt;
extern EnumMap EMLong;
extern EnumMap EMFloat;
extern EnumMap EMDouble;
extern EnumMap EMString;
extern EnumMap EMEnum;


extern EnumMap EMAnalog;
extern EnumMap EMDigital;
extern EnumMap EMPWM;

extern Runnable *PendingRunnable;

#endif