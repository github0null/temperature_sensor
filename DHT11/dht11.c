#include "dht11.h"

void DHT11_Init()
{
    DHT11_PIN_HIGH();
}

DHT11_ExitCode DHT11_Measure(DHT11_Data *dat)
{
    int8_t buf[5];
    int16_t i, j;

    DHT11_PIN_LOW();
    DHT11_DelayMs(20); // 开始信号 20 ms
    DHT11_PIN_HIGH();

    DHT11_Delay10Us();
    DHT11_Delay10Us();
    DHT11_Delay10Us();
    DHT11_Delay10Us();
    DHT11_Delay10Us();
    DHT11_Delay10Us(); // 40 ~ 120 us

    if (DHT11_PIN_READ() == 0)
    {
        while (DHT11_PIN_READ() == 0) //等待 DHT11 拉高
            ;

        while (DHT11_PIN_READ() == 1)
            ;

        i = 0;
        while (i < 5)
        {
            j = 0;
            while (j < 8)
            {
                while (DHT11_PIN_READ() == 0)
                    ;

                DHT11_Delay10Us();
                DHT11_Delay10Us();
                DHT11_Delay10Us(); // 30us

                buf[i] <<= 1;
                buf[i] |= DHT11_PIN_READ();

                while (DHT11_PIN_READ() == 1)
                    ;
                j++;
            }
            i++;
        }

        DHT11_Delay10Us();
        DHT11_Delay10Us();
        DHT11_Delay10Us();
        DHT11_Delay10Us();
        DHT11_Delay10Us();
        DHT11_Delay10Us(); // 60 us

        if (buf[4] == buf[0] + buf[1] + buf[2] + buf[3])
        {
            dat->temperature = buf[2];
            dat->humidity = buf[0];
            return DHT11_DONE;
        }

        return DHT11_VERIFY_ERR;
    }
    else
    {
        DHT11_DelayMs(1);
    }

    return DHT11_CONNECT_ERR;
}
