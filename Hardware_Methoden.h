/*
 * Hardware_Methoden.h
 *
 * Created: 21.01.2023 15:51:44
 *  Author: Thomas Heß
 */ 


#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <stddef.h>  // für size_t
#include <stdint.h> // für uint8_t
#include <stdbool.h> // damit bool als (abgeleiteter) Datentyp existiert






namespace Display
{
    void init();
    void locate(int, int);
    void print(const char*, ...);
    void cls();
}

namespace Serial
{
    void print(const char*, ...);
}

namespace ExtInterruptPins
{
    void init_extint_Wechseln(void);
    void isr_Wechseln(void);
    void extint_Wechseln_enable(void);
    void extint_Wechseln_disable(void);

    void init_extint_Plus(void);
    void isr_Plus(void);
    void extint_Plus_enable(void);
    void extint_Plus_disable(void);  

}


namespace Timers
{
    void init_TIM6_interrupt(void);
    void isr_TIM6(void);

    void TIM6_start(void);
    void TIM6_stop(void);

    void init_TIM7_interrupt(void);
    void isr_TIM7(void);

    void TIM7_start(void);
    void TIM7_stop(void);
}
/*
namespace Bluetooth
{

    void init_bt(void);
    bool bt_readable(void);
    void bt_read(void*,size_t);

}
*/
#endif /* HARDWARE_H_ */