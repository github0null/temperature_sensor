#include "dht11.h"

void DHT11_Init()
{
    DHT11_PIN_HIGH();
}

DHT11_ExitCode DHT11_Measure(DHT11_Data *dat)
{
    uint8_t buf[5];
    uint8_t i, j, k;

    DHT11_PIN_LOW(); // 开始信号 25 ms
    DHT11_DelayMs(25);

    DHT11_PIN_HIGH(); // 拉高 30 us
    DHT11_Delay10Us();
    DHT11_Delay10Us();
    DHT11_Delay10Us();

    DHT11_Delay10Us(); // 等待 40 us
    DHT11_Delay10Us();
    DHT11_Delay10Us();
    DHT11_Delay10Us();

    if (DHT11_PIN_READ() == 0)
    {
        while (DHT11_PIN_READ() == 0) //等待 DHT11 拉高
            ;

        while (DHT11_PIN_READ() == 1)
            ;

        for (i = 0; i < 5; i++)
        {
            for (j = 0; j < 8; j++)
            {
                while (DHT11_PIN_READ() == 0)
                    ;

                k = 0;
                while (DHT11_PIN_READ() == 1)
                {
                    DHT11_Delay10Us();
                    k++;
                }

                buf[i] <<= 1;
                buf[i] |= (k > 4); // 高电平大于 40 us
            }
        }

        DHT11_Delay10Us();
        DHT11_Delay10Us();
        DHT11_Delay10Us();
        DHT11_Delay10Us();
        DHT11_Delay10Us(); // 50 us

        if (buf[4] == buf[0] + buf[1] + buf[2] + buf[3])
        {
            dat->temperature = buf[2];
            dat->humidity = buf[0];
            return DHT11_DONE;
        }

        return DHT11_VERIFY_ERR;
    }

    return DHT11_CONNECT_ERR;
}
