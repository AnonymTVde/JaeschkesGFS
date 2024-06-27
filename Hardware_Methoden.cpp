/*
 * Hardware_Methoden.cpp
 *
 * Created: 21.01.2023 17:29:12
 *  Author: Thomas Heß
 */ 
#undef __ARM_FP
#include "Hardware_Methoden.h"
#include "Hardware_Definitionen.h"
#include "mbed.h"
#include "TextLCD_I2C.h"
#include "Variable_Global.h" // für globalen Zugriff auf den Automaten derAutomat, speziell für isr

#include <cstdio>
#include <stdarg.h>


// Für Sturm-Board, um das LC-Display per I2C ansteuern zu können obwohl es an PA_11, PA_12
// angeschlossen ist. Es müssen auf dem Morpho-Header folgende Verbindungen gesteckt werden:
// SCL: PA_11 <-> PB_8
// SDA: PA_12 <-> PB_9

DigitalIn scl(PA_11,PullUp);  
DigitalIn sda(PA_12,PullUp);

InterruptIn Wechseln(T_WECHSELN);
InterruptIn Plus(T_PLUS);


I2C i2c_lcd(I2C_SDA, I2C_SCL); // PB_9, PB_8 
// I2C i2c_lcd(PB_9, PB_8);
// I2C-Adresse 0x27 - 16 Spalten und 2 Zeilen 
// LCD20x4 → 20 Spalten und 4 Zeilen 
TextLCD_I2C LC_Display(&i2c_lcd, 0x27<<1, TextLCD_I2C::LCD16x2);


// BufferedSerial blue(BT_TX, BT_RX); // TX,RX
// DigitalOut enable(BT_EN, 0); // Enable auf 0, also kein Konfigurationsmodus


#pragma region LCD
void Display::init()
{
    LC_Display.setBackLight(true);
    LC_Display.cls(); 
    LC_Display.locate(0,0); // (column,row) 
    
}

void Display::locate(int column, int row)
{
    LC_Display.locate(column, row);
}

// Wrapper for printf(), according to https://stackoverflow.com/questions/20639632/how-to-wrap-printf-into-a-function-or-macro

void Display::print(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    LC_Display.vprintf(format, args);


    va_end(args);
}

void Display::cls()
{
    LC_Display.cls();
}
#pragma endregion
#pragma region Serial

void Serial::print(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);

}
#pragma endregion

#pragma region Externe InterruptPins
    #pragma region Wechseln

    void ExtInterruptPins::init_extint_Wechseln(void)
    {
        Wechseln.mode(PullDown);
        Wechseln.rise(&isr_Wechseln);
        Wechseln.enable_irq();
    }

    void ExtInterruptPins::isr_Wechseln(void)
    {
        derAutomat.sendEvent(TasterWechseln);
        ExtInterruptPins::extint_Wechseln_disable();
    }

    void ExtInterruptPins::extint_Wechseln_enable(void)
    {
        Wechseln.rise(&isr_Wechseln);
        Wechseln.enable_irq();
    }

    void ExtInterruptPins::extint_Wechseln_disable(void)
    {
        Wechseln.rise(NULL);
    }
    #pragma endregion
    #pragma region Plus

    void ExtInterruptPins::init_extint_Plus()
    {
        Plus.mode(PullDown);
        Plus.rise(&isr_Plus);
        Plus.enable_irq();
    }

    void ExtInterruptPins::isr_Plus(void)
    {
        derAutomat.sendEvent(TasterPlus);
        ExtInterruptPins::extint_Plus_disable();
    }

    void ExtInterruptPins::extint_Plus_enable(void)
    {
        Plus.rise(&isr_Plus);
        Plus.enable_irq();
    }

    void ExtInterruptPins::extint_Plus_disable(void)
    {
        Plus.rise(NULL);
    }
    #pragma endregion
#pragma endregion

#pragma region Timer

void Timers::init_TIM6_interrupt(void)
{
    // TIM6 mit Takt versorgen, Bit 5 in RCC_APB1ENR
    RCC->APB1ENR |= (1 << TIM6EN); // RCC_APB1ENR |= 0b 100000: RCC_APB1ENR.TIM6EN=1
    
    // TIM6 stoppen
    TIM6->CR1 &= ~(1 << CEN); // TIM6_CR1.CEN = 0; Bit 0
    
    // Prescaler auf 31999 setzen -A Taktsignal wird durch 32000 geteilt
    // Bei fCLK = 32MHz ergibt dies 1ms fCNT
    TIM6->PSC = 31999; // (1ms * 32MHz)-1 = 31999
    
    // TIM6 UIF löschen, ist Bit 0 im Register TIM6_SR
    TIM6->SR &= ~(1 << UIF); // FS TIM6->SR = 0, geht, da restl. 15 Bit reserved sind, trotzdem ungute Lösung
   
    // TIM6 neu laden
    TIM6->CNT = 0;
    
    // TIM6 Maximalwert auf 250 setzen, also Interrupt nach 250ms --> ¼s -->2Hz
    TIM6->ARR = 250;
    
    // TIM6 ISR in die Vektortabelle des NVIC eintragen
    NVIC_SetVector(TIM6_IRQn,(uint32_t) &isr_TIM6);

    // TIM6-Interrupt im NVIC freigeben
    HAL_NVIC_EnableIRQ(TIM6_IRQn);

    // TIM6 Interrupt freigeben TIM6_DIER.UIE=1: Bit 0 setzen
    TIM6->DIER |= (1 << UIE);
}

void Timers::isr_TIM6(void)
{
    // TIM6 Interrupt stoppen TIM6_DIER.UIE=1: Bit 0 löschen
    TIM6->DIER &= ~(1 << UIE);

    // TIM6 stoppen
    TIM6->CR1 &= ~(1 << CEN); // TIM6_CR1.CEN = 0; Bit 0

    //Taster isr freigeben
    ExtInterruptPins::extint_Wechseln_enable();
    ExtInterruptPins::extint_Plus_enable();

    //Blinkzustand invertieren
    derAutomat.set_BlinkZustand(!derAutomat.get_BlinkZustand());

    // TIM6 UIF löschen, ist Bit 0 im Register TIM6_SR
    TIM6->SR &= ~(1 << UIF); // FS TIM6->SR = 0, geht, da restl. 15 Bit reserved sind, trotzdem ungute Lösung
   
    // TIM6 neu laden
    TIM6->CNT = 0;

    // TIM6 Interrupt freigeben TIM6_DIER.UIE=1: Bit 0 setzen
    TIM6->DIER |= (1 << UIE);    

    // TIM6 starten
    TIM6->CR1 |= (1 << CEN); // TIM6_CR1.CEN = 1; Bit 0
}


void Timers::TIM6_start(void)
{   
    TIM6->CNT=0;
    TIM6->CR1 |= (1 << CEN); // TIM6_CR1.CEN = 1; Bit 0
}

void Timers::TIM6_stop(void)
{
    TIM6->CR1 &= ~(1 << CEN); // TIM6_CR1.CEN = 0; Bit 0
}

void Timers::init_TIM7_interrupt(void)
{
        // TIM7 mit Takt versorgen, Bit 5 in RCC_APB1ENR
    RCC->APB1ENR |= (1 << TIM7EN); // RCC_APB1ENR |= 0b 100000: RCC_APB1ENR.TIM7EN=1
    
    // TIM7 stoppen
    TIM7->CR1 &= ~(1 << CEN); // TIM7_CR1.CEN = 0; Bit 0
    
    // Prescaler auf 31999 setzen -A Taktsignal wird durch 32000 geteilt
    // Bei fCLK = 32MHz ergibt dies 1ms fCNT
    TIM7->PSC = 31999; // (1ms * 32MHz)-1 = 31999
    
    // TIM7 UIF löschen, ist Bit 0 im Register TIM6_SR
    TIM7->SR &= ~(1 << UIF); // FS TIM7->SR = 0, geht, da restl. 15 Bit reserved sind, trotzdem ungute Lösung
   
    // TIM7 neu laden
    TIM7->CNT = 0;
    
    // TIM7 Maximalwert auf 1000 setzen, also Interrupt nach 1s
    TIM7->ARR = 1000;
    
    // TIM7 ISR in die Vektortabelle des NVIC eintragen
    NVIC_SetVector(TIM7_IRQn,(uint32_t) &isr_TIM7);

    // TIM7-Interrupt im NVIC freigeben
    HAL_NVIC_EnableIRQ(TIM7_IRQn);

    // TIM7 Interrupt freigeben TIM7_DIER.UIE=1: Bit 0 setzen
    TIM7->DIER |= (1 << UIE);

    // TIM7 starten
    // TIM7->CR1 |= (1 << CEN); // TIM7_CR1.CEN = 1; Bit 0

}

void Timers::isr_TIM7(void)
{
     // TIM7 Interrupt stoppen TIM7_DIER.UIE=1: Bit 0 löschen
    TIM7->DIER &= ~(1 << UIE);

    // TIM7 stoppen
    TIM7->CR1 &= ~(1 << CEN); // TIM7_CR1.CEN = 0; Bit 0

    // Tick-Ereignis senden
    derAutomat.sendEvent(Tick);


    derAutomat.fsm_set_timeInState(1); // timeInState der fsm um 1 erhöhen

    // TIM7 UIF löschen, ist Bit 0 im Register TIM7_SR
    TIM7->SR &= ~(1 << UIF); // FS TIM7->SR = 0, geht, da restl. 15 Bit reserved sind, trotzdem ungute Lösung
   
    // TIM7 neu laden
    TIM7->CNT = 0;

    // TIM7 Interrupt freigeben TIM7_DIER.UIE=1: Bit 0 setzen
    TIM7->DIER |= (1 << UIE);

    //TIM7 starten
    TIM7->CR1 |= (1 << CEN); // TIM7_CR1.CEN = 1; Bit 0

    
}


void Timers::TIM7_start(void)
{   
    TIM7->CNT=0;
    TIM7->CR1 |= (1 << CEN); // TIM7_CR1.CEN = 1; Bit 0
}

void Timers::TIM7_stop(void)
{
    TIM7->CR1 &= ~(1 << CEN); // TIM7_CR1.CEN = 0; Bit 0
}

#pragma endregion
//=============================================================================
// Bluetooth
//=============================================================================
/*
void Bluetooth::init_bt()
{
    // Set desired properties (38400-8-N-1). HC10 kann nur 9600Baud 
    blue.set_baud(9600);
    blue.set_format(8,                    // bits
                  BufferedSerial::None, // parity
                  1                     // stop bits
    );
}

bool Bluetooth::bt_readable()
{
    return(blue.readable());
}

void Bluetooth::bt_read(void *buf, size_t laenge)
{
    blue.read(buf, laenge);
}
*/