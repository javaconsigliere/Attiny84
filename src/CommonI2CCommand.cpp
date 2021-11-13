#include "CommonI2CCommand.h"
// IO
EnumMap EMIO(C_IO, "IO");
CPinIO::CPinIO():CommandProcessor(&EMIO)
{

}
int CPinIO::run()
{

    return OK;
}
boolean CPinIO::parseParameters(int offset, Command *cmd)
{

    return true;
}
CPinIO PinIO;
// IO end


// CPU-SPEED
EnumMap EMCPUSpeed(C_CPU_SPEED, "CPU-SPEED");
CCPUSpeed::CCPUSpeed():CommandProcessor(&EMCPUSpeed)
{
}

int CCPUSpeed::run()
{
    I2CUtil::write(OK);
    I2CUtil::write((uint8_t)':');
    I2CUtil::write(F_CPU);
    return OK;
}
CCPUSpeed CPUSpeed;
// CPU-SPEED end
EnumMap EMVersion(C_VERSION, "VERSION");
CVersion::CVersion(const char * v):CommandProcessor(&EMVersion)
{
    version = v;
}
int CVersion::run()
{
    I2CUtil::write(version);
    return OK;
}


EnumMap EMI2CAddress(C_I2C_ADDRESS, "I2C-ADDRESS");
CI2CAddress::CI2CAddress():CommandProcessor(&EMI2CAddress)
{
}
boolean CI2CAddress::parseParameters(int offset, Command *cmd)
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
int CI2CAddress::run()
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

CI2CAddress I2CAddress;






Counter::Counter(EnumMap *c, boolean cType) : CommandProcessor(c)
{
    incOnRun = cType;
}
int  Counter::run()
{
    long val = incOnRun ? inc() : get();
    I2CUtil::write(OK);
    I2CUtil::write((uint8_t)':');
    I2CUtil::write(val);
    
    return OK;
}
long Counter::inc(){return ++counter;}
long Counter::get(){return counter;}

EnumMap EMPing(C_PING, "PING");
EnumMap EMMessages(C_MESSAGES, "MESSAGES");
Counter I2CMessageCounter(&EMMessages, false);
Counter PingCounter(&EMPing, true);
