#ifndef SERVO_H
#define SERVO_H

#include <xc.h>
#include <stdint.h>

typedef enum 
{
    SERVO_1,
    SERVO_2
} eSERVO;

typedef enum
{
    PTCKPS_1,
    PTCKPS_4,
    PTCKPS_16,
    PTCKPS_64
} ePTCKPS;

void servo_init(uint16_t freq, ePTCKPS ps);
void servo_start(void);
void servo_stop(void);
void servo_setDC(eSERVO reg, uint16_t dutycycle);
void servo_setDCPC(eSERVO reg, int16_t dutycycle);

#endif	/* SERVO_H */

