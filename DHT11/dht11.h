#ifndef _H_DHT11
#define _H_DHT11

#include "stdint.h"

/**
 * Interface define
*/

#include "stm32f10x.h"
#include "BITBAND.h"
#include "SysTick.h"

#define DATA_WRITE(val) PBout(0) = (0x01U & (val))
#define DATA_READ() PBin(0)
#define DelayUs(us) SysTick_DelaySyncUs(us)

//---------------------

typedef struct
{
    uint8_t humidity;
    float temperature;
} DHT11_Data;

void DHT11_Init()
{
    DATA_WRITE(1);
}

uint8_t DHT11_Measure(DHT11_Data *dat)
{
    int8_t buf[5];
    int16_t i, j;

    DATA_WRITE(0);
    DelayUs(20000); // 开始信号 20 ms
    DATA_WRITE(1);
    DelayUs(50); // 40~120 us

    if (DATA_READ() == 0)
    {
        while (DATA_READ() == 0) //等待 DHT11 拉高
            ;

        while (DATA_READ() == 1)
            ;

        i = 0;
        while (i < 5)
        {
            j = 0;
            while (j < 8)
            {
                while (DATA_READ() == 0)
                    ;
                DelayUs(30);
                buf[i] <<= 1;
                buf[i] |= DATA_READ();
                while (DATA_READ() == 1)
                    ;
                j++;
            }
            i++;
        }

        DelayUs(60);

        if (buf[4] == buf[0] + buf[1] + buf[2] + buf[3])
        {
            dat->temperature = buf[2];
            dat->humidity = buf[0];
            return 1;
        }
    }
    else
    {
        DelayUs(1000);
    }

    return 0;
}

#endif
