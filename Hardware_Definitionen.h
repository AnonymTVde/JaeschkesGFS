/*
 * Hardware_Definitionen.h
 *
 * Created: 21.01.2023 15:44:07
 *  Author: Thomas Heß
 */ 


#ifndef HARDWARE_HWDEFS_H_
#define HARDWARE_HWDEFS_H_



namespace ExtInterruptPins
{

    #define T_WECHSELN      PB_0
    #define T_PLUS          PB_4

}

namespace Timers 
{
    // #define für einzelne Konfigurationsbits

    // RCC_APB1ENR
    #define TIM6EN 4
    #define TIM7EN 5

    // TIMx_CR1, CEN... counter enable
    #define CEN 0

    // TIMx_DIER, UIE ... Update Interrupt Enable
    #define UIE 0

    // TIMx_SR, UIF ... Update Interrupt Flag
    #define UIF 0
}

#endif /* HARDWARE_HWDEFS_H_ */