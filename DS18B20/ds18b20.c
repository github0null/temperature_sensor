#include "ds18b20.h"

DelayUsFunc _DelayUs;

#define WRITE_BUS(val) DB_Write = val
#define READ_BUS() DB_Read

void DS18B20_Init(DelayUsFunc delayUs)
{
    _DelayUs = delayUs;
    WRITE_BUS(1);
    DS18B20_Measure();
}

int8_t DS18B20_Reset()
{
    uint8_t _dp;
    WRITE_BUS(0);
    _DelayUs(600);
    WRITE_BUS(1);
    _DelayUs(30);
    _dp = READ_BUS();
    _DelayUs(500);
    return !_dp;
}

int8_t _Read()
{
    int8_t res = 0x00;
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        WRITE_BUS(0);
        _DelayUs(1);
        WRITE_BUS(1);

        _DelayUs(1);
        _DelayUs(1);

        res |= READ_BUS() << i;

        _DelayUs(60);

        WRITE_BUS(1);

        _DelayUs(1);
    }

    return res;
}

void _Write(int8_t _data)
{
    uint8_t i;
    int8_t _dat = _data;

    for (i = 0; i < 8; i++)
    {
        WRITE_BUS(0);
        _DelayUs(1);

        WRITE_BUS(_dat & 0x01);
        _DelayUs(60);

        WRITE_BUS(1);

        _dat = ((_dat >> 1) & 0x7F);
        _DelayUs(1);
    }
}

void DS18B20_SetTriggerTemp(int8_t t_high, int8_t t_low)
{
    if (DS18B20_Reset())
    {
        _Write(0xCC);
        _Write(0x4E);
        _Write(t_high);
        _Write(t_low);
        DS18B20_Reset();
    }
}

float DS18B20_Measure()
{
    int8_t valL, valH, val;
    float temp;

    if (DS18B20_Reset())
    {
        _Write(0xCC);
        _Write(0x44);

        //Delay(100);

        if (DS18B20_Reset())
        {
            _Write(0xCC);
            _Write(0xBE);

            valL = _Read();
            valH = _Read();

            valL &= 0x00FF;
            valH &= 0x00FF;

            val = valH << 8;
            val |= valL;

            if (val & 0x8000)
                val = -(~val + 1);

            temp = val * 0.0625;

            return temp;
        }
    }

    return DS18B20_INVALID_TEMPERATURE;
}

int DS18B20_IsTrigger()
{
    if (DS18B20_Reset())
    {
        _Write(0xCC);
        _Write(0xEC);
        _DelayUs(15);
        if (_Read())
        {
            return 1;
        }
        return 0;
    }
    return 0;
}
