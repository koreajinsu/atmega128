#include "common/def.h"

#define LED_DDR     DDRB
#define LED_PORT    PORTB


//구조체
typedef struct 
{
    volatile uint8_t    *port;
    uint8_t              pin;
}LED;
void ledInit(); 
void ledOn();
void ledOff();
void ledTog();