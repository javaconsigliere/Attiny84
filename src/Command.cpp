#include <Command.h>



// TokenMap::TokenMap(int map_, const char * token_, DataType dt_)
// {
//     map = map_;
//     token = token_;
//     dataType = dt_;
// }

// int TokenMap::getMap()
// {
//     return map;
// }


// const char * TokenMap::getToken()
// {
//     return token;
// }

// DataType TokenMap::getDataType()
// {
//     return dataType;
// }











Command::Command()
{

}

uint8_t * Command::getCommand()
{
    return data;

}
int Command::length()
{
    return dataLen;
}
int Command::getSize()
{
    return size;
}
void Command::setLength(int len)
{
    dataLen = len;
}

void Command::reset()
{
    setLength(0);
}






EnumMap *CommandProcessor::getCommand()
{
    return command;
}

void CommandProcessor::setAction(ACTION act)
{
    action = act;
}

ACTION CommandProcessor::getAction()
{
    return action;
}
CommandProcessor::CommandProcessor(EnumMap *em)
{
    command = em;
}


boolean CommandProcessor::parseParameters(int offset, Command *command)
{
    return true;
}



/*CManager::CManager(int size)
{
    cProcessors = &CommandProcessor[size];
}*/

CManager::CManager(CommandProcessor **cp)
{
    cProcessors = cp;
}
void CManager::add(CommandProcessor *ct)
{
    if(ct != NULL)
    {
        cProcessors[size++] = ct; 
    }
}







CommandProcessor * CManager::parse(Command *command)
{
    for(int i = 0 ; i < size; i++)
    {
         const char *commandToken = cProcessors[i]->getCommand()->name();
        int ctLength = strlen(commandToken);
        int m = 0;
        for(; m < ctLength && m < command->length(); m++)
        {
            if(commandToken[m] != command->getCommand()[m])
            {
                m = -1;
                break;
            }
        }
        if (m == ctLength)
        {
            // we have a match
            cProcessors[i]->setAction(G);
            if(m < command->length())
            {
                // we need to parse action
                if(command->getCommand()[m] == ':')
                {
                    m++;
                }
                if(m < command->length())
                {
                    
                    switch(command->getCommand()[m])
                    {
                        case 'S': 
                        cProcessors[i]->setAction(S);
                        m++;
                        break;
                        case 'U':
                        cProcessors[i]->setAction(U);
                        m++;
                        break;
                        case 'D':
                        cProcessors[i]->setAction(D);
                        m++;
                        break;
                        case 'G':
                        cProcessors[i]->setAction(G);
                        m++;
                        break;
                        default:
                            return NULL;
                    }

                    if(m < command->length())
                        if(command->getCommand()[m] == ':')
                            m++;
                }
            }
            // parse the rest of parameters
            if(cProcessors[i]->parseParameters(m, command))
                return cProcessors[i];
        }
    }


    return NULL;
}






// TokenMap TMSet = TokenMap(S, "S", ENUM);
// TokenMap TMGet = TokenMap(G, "G", ENUM);
// TokenMap TMUpdate = TokenMap(U, "U", ENUM);
// TokenMap TMDelete = TokenMap(D, "D", ENUM);

// EnumMap EMPing(C_PING, "PING");
// EnumMap EMMessages(C_MESSAGES, "MESSAGES");
// EnumMap EMAref(C_CALIBRATE, "AREF");
// EnumMap EMReset(C_RESET, "RESET");

// // EnumMap EMUptime(C_UPTIME, "UPTIME");
// //EnumMap EMVersion(C_VERSION, "VERSION");
// //EnumMap EMI2CAddress(C_I2C_ADDRESS, "I2C-ADDRESS");
// EnumMap EMEcho(C_ECHO, "ECHO");


CommandProcessor *cpList[15];

CManager CommandManager(cpList);