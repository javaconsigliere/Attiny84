#include <CommonI2CCommand.h>


class CIOPostRun:public Runnable
{
    public:
    int run()
    {
        switch(PinIO.getIOT())
        {
        case IO_A:
            pinMode(PinIO.getPin(), OUTPUT);
            analogWrite(PinIO.getPin(), PinIO.getValue());
            break;
        case IO_D: 
            pinMode(PinIO.getPin(), OUTPUT);
            digitalWrite(PinIO.getPin(),  PinIO.getValue() == 0 ? LOW : HIGH);
            

            // if(setValue == -1)
            //     pinMode(pin, OUTPUT);
            // else
            // {
            //     if (setValue == 0)
            //         PORTA &= ~_BV(pin);// low
            //     else
            //         PORTA |= _BV(pin);//high
            // }
            break;
        case IO_P:
        
            break;
        }

        return OK;
    }
};

CIOPostRun IOPostRun;

// IO
EnumMap EMIO(C_IO, "IO");
CPinIO::CPinIO():CommandProcessor(&EMIO)
{

}


int CPinIO::run()
{
    int val = 0;
    char iotchar;
   
    switch(getAction())
    {
        // case G:
        //     switch(iot)
        //     {
        //         case IO_A:
        //             val = analogRead(pin);
        //             break;
        //         case IO_D:
        //             val = digitalRead(pin);
        //             break;
        //         case IO_P:
        //             break;
        //     }
        //     break;
        case S:
        case U:
            switch(iot)
            {
                case IO_A:
                    //analogWrite(pin, setValue);
                    //val = analogRead(pin);
                    iotchar = 'A';
                    val = setValue;
                    break;
                case IO_D: 
                    //pinMode(pin, OUTPUT);
                    val = setValue == 0 ? LOW : HIGH;
                    //digitalWrite(pin,  val);
                    
                    iotchar = 'D';

                    // if(setValue == -1)
                    //     pinMode(pin, OUTPUT);
                    // else
                    // {
                        
                    //     if (setValue == 0)
                    //         PORTA &= ~_BV(pin);// low
                    //     else
                    //         PORTA |= _BV(pin);//high
                    // }

                    //val = digitalRead(pin);
                    break;
                case IO_P:
                    break;
            }
            if (PendingRunnable == NULL)
            {
                // PostRunnable.iot = iot;
                // PostRunnable.pin = pin;
                // PostRunnable.setValue = val;
                // PendingRunnable = &PostRunnable;
                PendingRunnable = &IOPostRun;
                I2CUtil::write(OK);
                I2CUtil::write((uint8_t)':');
                I2CUtil::write((uint8_t)iotchar);
                I2CUtil::write((uint8_t)':');
                I2CUtil::write((uint8_t)pin);
                I2CUtil::write((uint8_t)':');
                I2CUtil::write((long)val);
                
                return OK;
            }
            else
            {
                return CONFLICT;
            }
            break;
            
        case G:
            switch(iot)
            {
                case IO_A:
                    val = analogRead(pin);
                    iotchar = 'A';
                    break;
                case IO_D:
                    val = digitalRead(pin);
                    iotchar = 'D';
                    break;
                case IO_P:
                    break;
            }
            I2CUtil::write(OK);
            I2CUtil::write((uint8_t)':');
            I2CUtil::write((uint8_t)iotchar);
            I2CUtil::write((uint8_t)':');
            I2CUtil::write((uint8_t)pin);
            I2CUtil::write((uint8_t)':');
            I2CUtil::write((long)val);
        break;
        default:
            break;
    }
    

    return OK;
}

IO_TYPE CPinIO::getIOT()
{
    return iot;
}
uint8_t CPinIO::getPin()
{
    return pin;
}
int CPinIO::getValue()
{
    return setValue;
}







boolean CPinIO::parseParameters(int offset, Command *cmd)
{
    // full command IO:[G,S,U,D]:[A,D,P]:pin:value
    setValue = -1;
    // parsing [A,D,P]
    if (offset < cmd->length())
    {
        // char tok[2];
        // tok[1] = 0;
        // tok[0] = cmd->getCommand()[offset++];
        // EnumMap *ioType = EnumMap::match(tok, &EMAnalog, &EMDigital, &EMPWM, NULL);
        // if (ioType == NULL)
        //     return false;
        // iot = (IO_TYPE)ioType->map();

        char ioType = cmd->getCommand()[offset++];

        switch(ioType)
        {
            case 'A':
                iot = IO_A;
                break;
            case 'D':
                iot = IO_D;
                break;
            // case 'P':
            //     iot = IO_P;
            //     break;
            default:
                return false;
        }

    }

    if(offset  < cmd->length() && cmd->getCommand()[offset] == ':')
    {
        offset++;
    }

    // pin value one byte
    if (offset < cmd->length())
    {
        pin = cmd->getCommand() [offset++];
        //offset+=2;
        
        if(getAction() == G)
        {
            return true;
        }
        
    }
    
    if(offset < cmd->length() && cmd->getCommand()[offset] == ':')
    {
        offset++;
    }
    // value
    // value to be set
    if (offset < cmd->length())
    {
        setValue = BytesToPrimitive::toInt(cmd->getCommand() + offset);
        return true;
    }
    else if (iot == IO_D) // just set the pin to digital output
    {
        return true;
    }

    

    return false;
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


// UPTIME
EnumMap EMCUptime(C_UPTIME, "UPTIME");
CUptime::CUptime():CommandProcessor(&EMCUptime)
{
}

int CUptime::run()
{

    I2CUtil::write(OK);
    I2CUtil::write((uint8_t)':');
    I2CUtil::write((long)millis());
    return OK;
}
// UPTIME end

// VERSION
EnumMap EMVersion(C_VERSION, "VERSION");
CVersion::CVersion(const char *m, const char * v):CommandProcessor(&EMVersion)
{
    model = m;
    version = v;
    length = strlen(model) + strlen(version) + 1;
}
int CVersion::run()
{
    I2CUtil::write((uint8_t)length);
    I2CUtil::write(false, model);
    I2CUtil::write((uint8_t)':');
    I2CUtil::write(false, version);
    return OK;
}
// VERSION END


// RESET
EnumMap EMReset(C_RESET, "RESET");
CReset::CReset(uint8_t pin):CommandProcessor(&EMReset)
{
    resetPin = pin;
}
int CReset::run()
{
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, LOW);
    return OK;
}
// RESET end

// I2C-ADDRESS
EnumMap EMI2CAddress(C_I2C_ADDRESS, "I2C-ADDRESS");
CI2CAddress::CI2CAddress():CommandProcessor(&EMI2CAddress)
{
}
boolean CI2CAddress::parseParameters(int offset, Command *cmd)
{
    // I2C:S:address
    if(getAction() == S && offset < cmd->length())
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
// I2C-ADDRESS end

EnumMap EMAref(C_CALIBRATE, "AREF");
CAref::CAref(uint8_t pin):CommandProcessor(&EMAref)
{
      referencePin = pin;
}



 int CAref::run()
 {
      I2CUtil::write(OK);
      I2CUtil::write((uint8_t)':');
      I2CUtil::write((long)getAnalogAref());
      return OK;
}

int CAref::getAnalogAref()
{
    aRefValue = analogRead(referencePin);
    return aRefValue;
}
int CAref::getAref()
{
    return aRefValue;
}


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
