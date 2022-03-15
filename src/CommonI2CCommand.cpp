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






int CPinIO::run()
{
    int val = 0;
    char iotchar;
   
    switch(getAction())
    {
        case A_S:
        case A_U:// Set or update
        
            switch(iot)
            {
                case IO_A:
                    iotchar = 'A';
                    val = setValue;
                    //pinMode(pin, OUTPUT);
                    break;
                case IO_D: 
                    iotchar = 'D';
                    val = setValue == 0 ? LOW : HIGH;
                    //pinMode(pin, OUTPUT);
                    break;
                
            }
            if (PendingRunnable == NULL)
            {
                
                PendingRunnable = &IOPostRun;
                I2CUtil::write(OK);
                I2CUtil::write((uint8_t)':');
                I2CUtil::write((uint8_t)iotchar);
                I2CUtil::write((uint8_t)':');
                I2CUtil::write((uint8_t)pin);
                I2CUtil::write((uint8_t)':');
                I2CUtil::write((long)val);
            }
            else
            {
                return CONFLICT;
            }
        
            break;
        case A_P://provision the pin
        
            pinMode(pin, setValue);
            iotchar = 'P';
            I2CUtil::write(OK);
            I2CUtil::write((uint8_t)':');
            I2CUtil::write((uint8_t)iotchar);
            I2CUtil::write((uint8_t)':');
            I2CUtil::write((uint8_t)pin);
            I2CUtil::write((uint8_t)':');
            I2CUtil::write((long)setValue);
            break;    
        case A_G:
        
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
boolean CPinIO::parseParameters(int offset, Command *cmd)
{
    // full command IO:[G,S,U,D,P]:[A,D]:pin:[value]
    // IO:P:pin:[I,O,P]
    setValue = -1;
    
    // parsing [A,D,P]
    if (getAction() == A_P)
    {
       if (offset < cmd->length())
        {
            pin = cmd->getCommand() [offset++];
            
        }
        
        if(offset < cmd->length() && cmd->getCommand()[offset] == ':')
        {
            offset++;
        }
        // value
        // value to be set
        if (offset < cmd->length())
        {

         
                // provisioning mode
            switch(cmd->getCommand()[offset])
            {
                case 'P':
                    setValue = INPUT_PULLUP;
                    break;
                case 'O':
                    setValue = OUTPUT;
                    break;
                case 'I':
                    setValue = INPUT;
                    break;
                default:
                    return false;
            }
            return true;
            
            
        }
        
    }
    else
    {
        if (offset < cmd->length())
        {
            switch(cmd->getCommand()[offset++])
            {
                case 'A':
                    iot = IO_A;
                    break;
                case 'D':
                    iot = IO_D;
                    break;
                default:
                    return false;
            }
        }

        if(offset < cmd->length() && cmd->getCommand()[offset] == ':')
        {
            offset++;
        }

        // pin value one byte
        if (offset < cmd->length())
        {
            pin = cmd->getCommand() [offset++];
            if(getAction() == A_G)
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

            // if (getAction() == A_P)
            // {
            //      // provisioning mode
            //     switch(cmd->getCommand()[offset])
            //     {
            //         case 'P':
            //             setValue = INPUT_PULLUP;
            //             break;
            //         case 'O':
            //             setValue = OUTPUT;
            //             break;
            //         case 'I':
            //             setValue = INPUT;
            //             break;
            //         default:
            //             return false;
            //     }
            //     return true;
            // }
            // else
            {
                setValue = BytesToPrimitive::toInt(cmd->getCommand() + offset);
                return true;
            }
        }
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
    if(getAction() == A_S && offset < cmd->length())
    {
        //i2cAddress = BytesToPrimitive::toInt((cmd->getCommand() + offset));
        i2cAddress = cmd->getCommand()[offset++];
        if(i2cAddress > 7 && i2cAddress < 128)
            return true;
    }
    else if (getAction() == A_G)
    {
        return true;
    }
    return false;
}
int CI2CAddress::run()
{
    uint8_t old = I2CConfig.getAddress();
    if(getAction() == A_S || getAction() == A_U)
    {
    I2CConfig.setAddress(i2cAddress);
    }

    I2CUtil::write(OK);
    I2CUtil::write((uint8_t)':');
    I2CUtil::write(old);
    if(getAction() == A_S || getAction() == A_U)
    {
    //I2CConfig.setAddress(i2cAddress);
    I2CUtil::write((uint8_t)':');
    I2CUtil::write(I2CConfig.getAddress());
    }
    return OK;
}

CI2CAddress I2CAddress;
// I2C-ADDRESS end

EnumMap EMAref(C_ADC_REF, "AREF");
CAref::CAref():CommandProcessor(&EMAref)
{

}

boolean CAref::parseParameters(int offset, Command *cmd)
{
    // full command AREF:[G,S]:[D,E,I]
  
  
    // parsing [A,D,P]
    switch (getAction())
    {
    
        case A_S:
            if (offset < cmd->length())
            {
                uint8_t refType = cmd->getCommand()[offset++];

                switch(refType)
                {
                    case 'D':
                        arefType = DEFAULT;
                        break;
                    case 'E':
                        arefType = EXTERNAL;
                        break;
                    case 'I':
                        arefType = INTERNAL;
                        break;
                    default:
                        return false;
                }
                return true;

            }
            else
            {
                return false;
            }
        case A_G:
            return true;
        default:
            return false;
    }
    
}

 int CAref::run()
 {
    switch(getAction())
    {
        case A_S:
            analogReference(getArefType());
            break;
        default:
            break;
    }

    I2CUtil::write(OK);
    I2CUtil::write((uint8_t)':');
    I2CUtil::write((long)getArefType());
    return OK;
}


int CAref::getArefType()
{
    return arefType;
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
CAref ARef;
