#include<DataTypes.h>

EnumMap::EnumMap(int _map, const char *_name)
{
    iName = _name;
    iMap = _map;
}
const char *EnumMap::name()
{
    return iName;
}
int EnumMap::map()
{
    return iMap;
}

EnumMap *EnumMap::match(const char * token, ...)
{
    EnumMap *ret = NULL;
    va_list vl;
    va_start(vl, token);

    for(int i = 0; ret != NULL; i++)
    {
        ret = va_arg(vl, EnumMap *);
        // if(ret == NULL)
        //     break;
        if(strncmp(ret->name(), token, strlen(ret->name())) == 0)
        {
            break;
        }
    }
    va_end(vl);
    
    return ret;
}

int BytesToPrimitive::toInt(const uint8_t buffer[])
{
    int ret = (buffer[0] & 0xFF);
    ret = ret << 8 | (buffer[1] & 0xFF);
    return ret;
}
long BytesToPrimitive::toLong(const uint8_t buffer[])
{
    long ret = (buffer[0] & 0xFF);
    ret = ret << 8 | (buffer[1] & 0xFF);
    ret = ret << 8 | (buffer[2] & 0xFF);
    ret = ret << 8 | (buffer[3] & 0xFF);
    return ret;
}

// float BytesToPrimitive::toFloat(const uint8_t buffer[])
// {
//     long ret = (buffer[0] & 0xFF);
//     ret = ret << 8 | (buffer[1] & 0xFF);
//     ret = ret << 8 | (buffer[2] & 0xFF);
//     ret = ret << 8 | (buffer[3] & 0xFF);
//     return ret;
// }



EnumMap EMByte(BYTE, "B");
EnumMap EMInt(INT, "I");
EnumMap EMLong(LONG,"L");
EnumMap EMFloat(FLOAT,"F");
EnumMap EMDouble(DOUBLE,"D");
EnumMap EMString(STRING,"S");
EnumMap EMEnum(ENUM,"E");


EnumMap EMAnalog(IO_A, "A");
EnumMap EMDigital(IO_D, "D");
EnumMap EMPWM(IO_P, "P");
Runnable *PendingRunnable;