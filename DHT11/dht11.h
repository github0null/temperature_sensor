#ifndef _H_DHT11
#define _H_DHT11

#ifdef __cplusplus
extern "C" {
#endif

/* User must implement DHT11 interface in "dht11_conf.h" */
#include "dht11_conf.h"

/**
 * DHT11 interface
*/
#ifndef DHT11_PIN_HIGH
#error "DHT11_PIN_HIGH() must be implemented"
#endif

#ifndef DHT11_PIN_LOW
#error "DHT11_PIN_LOW() must be implemented"
#endif

#ifndef DHT11_PIN_READ
#error "DHT11_PIN_READ() must be implemented"
#endif

#ifndef DHT11_Delay10Us
#error "DHT11_Delay10Us() must be implemented"
#endif

#ifndef DHT11_DelayMs
#error "DelayMs(uint16 ms) must be implemented"
#endif

//---------------------

typedef struct
{
    uint8_t humidity;
    float temperature;
} DHT11_Data;

typedef enum {
    DHT11_DONE,
    DHT11_CONNECT_ERR,
    DHT11_VERIFY_ERR
} DHT11_ExitCode;

void DHT11_Init(void);

DHT11_ExitCode DHT11_Measure(DHT11_Data *dat);

#ifdef __cplusplus
}
#endif

#endif
