#include "servo.h"
#include "common/globals.h"
#include "common/types.h"

#define DC_MIN      (1574U) // ~1.3 ms Backwards
#define DC_Z        (1816) // ~1.5 ms Stop
#define DC_MAX      (2058U) // ~1.7 ms Forwards
#define DC_DELTA    (DC_MAX - DC_Z)
#define DC_MAP(dc)  (((dc) * (DC_DELTA)) / 100U)

/* Private prototypes *********************************************************/
static void servo_configReg(ePTCKPS ps);
static void servo_setFreq(uint16_t freq, ePTCKPS ps);

/* Public functions ***********************************************************/
void servo_init(uint16_t freq, ePTCKPS ps)
{
    servo_configReg(ps);
    servo_setFreq(freq, ps);
}

void servo_setDC(eSERVO reg, uint16_t dutycycle)
{
    vuint16_t *dc[] = {&P1DC1, &P1DC2};
    *dc[reg] = dutycycle;
}

void servo_setDCPC(eSERVO reg, int16_t dutycyclePC)
{
    int16_t dutycycle = DC_Z + ((dutycyclePC * 241) / 100);
    servo_setDC(reg, dutycycle);
}

void servo_start(void)
{
    BIT_SET(P1TCON, 15);
}

void servo_stop(void)
{
    BIT_CLR(P1TCON, 15);
}


/* Private functions **********************************************************/
static void servo_configReg(ePTCKPS ps)
{
    P1TCON =
            (0U << 15)  |   /* PTEN = 0     PWM time base is off */
                                            // bit 14 unimplemented
            (0U << 13)  |   /* PTSDL = 0    PWM time base runs in CPU idle mode */
                                            // bit <12:8> uimplemented
            (0U << 4)   |   /* PTOPS<7:4> = 0   1:1 postscale */
            (ps << 2)   |   /* PTCKPS<3:2> = 0  1:1 prescale */
            (0U << 0);      /* PTMOD<1:0> = 0   PWM time base operates in Free-running mode */

    PWM1CON1 =
                                            // bit <15:11> unimplemented
            (0U << 10)  |   /* PMOD3 = 0    PWM I/O pin is in the Complementary Output mode */
            (1U << 9)   |   /* PMOD2 = 1    PWM I/O pin is in the Independent Output mode */
            (1U << 8)   |   /* PMOD1 = 1    PWM I/O pin is in the Independent Output mode */
                                            // bit 7 unimplemented
            (0U << 6)   |   /* PEN3H = 0    PWM3H pin disabled. I/O pin is general purpose */
            (0U << 5)   |   /* PMO2H = 0    PWM2H pin disabled. I/O pin is general purpose */
            (0U << 4)   |   /* PMO1H = 0    PWM1H pin disabled. I/O pin is general purpose */
                                            // bit 3 unimplemented
            (0U << 2)   |   /* PEN3L = 0    PWM3H pin disabled. I/O pin is general purpose */
            (1U << 1)   |   /* PMO2L = 1    PWM2L pin is enabled for PWM output */
            (1U << 0);      /* PMO1L = 1    PWM1L pin is enabled for PWM output */

    PWM1CON2 =
                                            // bit <15:12> uinimplemented
            (0U << 8)   |   /* SECPOS<11:8> = 0     1:1 postscale */
                                            // bit <7:3> unimplemented
            (1U << 2)   |   /* IUE = 1      Updates to the active PxDC registers are inmediate */
            (1U << 1)   |   /* OSYNC = 1    Output overrides via the RxOVDCON register are synchronized to the PWM time base */
            (0U << 0);      /* UDIS = 0     Updates from Duty Cycle and Period Buffer registers are enabled */

}
static void servo_setFreq(uint16_t freq, ePTCKPS ps)
{
    // PxTPER = (FCY/(freq * PTCKPS)) - 1
    // PTCKPS = 1
    uint8_t _ps[] = {1, 4, 16, 64};
    uint16_t ptper = (FCY/((uint32_t)freq * _ps[ps])) - 1;
    P1TPER = ptper;
}
