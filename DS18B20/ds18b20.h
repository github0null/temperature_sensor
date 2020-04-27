#ifndef _H_DS18B20
#define _H_DS18B20

//----------------define pin-------------------
#define DB_Read PBin(n)
#define DB_Write PBout(n)
//--------------------------------------

#include "stdint.h"

#define DS18B20_INVALID_TEMPERATURE 500.0f

typedef void (*DelayUsFunc)(unsigned int us);

void DS18B20_Init(DelayUsFunc _DelayUs);
void DS18B20_SetTriggerTemp(int8_t t_high, int8_t t_low);
float DS18B20_Measure(void);
int DS18B20_IsTrigger(void);

#endif
