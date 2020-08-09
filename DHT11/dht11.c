#include "dht11.h"

#define PIN_LEVEL_LOW 0
#define PIN_LEVEL_HIGH 1

#define WAIT_TIMEOUT 10

DHT11_ExitCode DHT11_WaitPinLevel(uint8_t pinLevel)
{
    uint8_t i;

    for (i = 0; (i < WAIT_TIMEOUT) && (DHT11_PIN_READ() != pinLevel); i++)
    {
        DHT11_Delay10Us();
    }

    return i < WAIT_TIMEOUT ? DHT11_DONE : DHT11_TIMEOUT;
}

DHT11_ExitCode DHT11_Measure(DHT11_Data *dat)
{
    uint8_t buf[5];
    uint8_t i, j, k;

    DHT11_PIN_HIGH();
    DHT11_Delay10Us();
    DHT11_PIN_LOW(); // 拉低至少 18 ms
    DHT11_DelayMs(20);

    DHT11_PIN_HIGH(); // 拉高 30 us
    DHT11_Delay10Us();
    DHT11_Delay10Us();
    DHT11_Delay10Us();

    if (DHT11_WaitPinLevel(PIN_LEVEL_LOW) != DHT11_DONE) // 等待 DHT11 拉低完成响应
        return DHT11_CONNECT_ERR;

    if (DHT11_WaitPinLevel(PIN_LEVEL_HIGH) != DHT11_DONE) // 等待 DHT11 拉高
        return DHT11_CONNECT_ERR;

    if (DHT11_WaitPinLevel(PIN_LEVEL_LOW) != DHT11_DONE) // 等待 DHT11 拉低
        return DHT11_CONNECT_ERR;
    
    // 开始接收数据

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (DHT11_WaitPinLevel(PIN_LEVEL_HIGH) != DHT11_DONE) // 等待 DHT11 拉高
                return DHT11_TIMEOUT;

            // 获得高电平时间
            for (k = 0; DHT11_PIN_READ() == 1; k++)
            {
                DHT11_Delay10Us();
            }

            buf[i] <<= 1;
            buf[i] |= (k > 4); // 高电平大于 40 us
        }
    }

    DHT11_WaitPinLevel(PIN_LEVEL_HIGH); // 等待 DHT11 释放总线

    if (buf[4] == buf[0] + buf[1] + buf[2] + buf[3])
    {
        dat->temperature = buf[2];
        dat->humidity = buf[0];
        return DHT11_DONE;
    }

    return DHT11_VERIFY_ERR;
}
