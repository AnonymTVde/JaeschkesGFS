/*
 * Automat_Zustaende.cpp
 *
 * Created: 22.01.2023 10:41:44
 *  Author: Thomas Heß
 * Implementierung der Methoden entry(), exit() und do() für die einzelnen Zustandsklassen (die Objekte werden erst in ClassAutomat.cpp erzeugt)
 */ 

#include "Automat_Zustaende.h"
#include "Hardware_Methoden.h"
#include "Variable_Global.h"

#include <cstdint>
#include <stdint.h>

#pragma region StateInitialisierung

void StateInitialisierung::entryAction()
{

    Display::init();
    ExtInterruptPins::init_extint_Wechseln();
    ExtInterruptPins::init_extint_Plus();

    ExtInterruptPins::extint_Wechseln_enable();
    ExtInterruptPins::extint_Plus_enable();

    Timers::init_TIM6_interrupt();
    Timers::init_TIM7_interrupt();
    Timers::TIM7_start();

    derAutomat.sendEvent(NoEvent); // Hiermit wird die Transition nach Bereit ausgelöst

}

void StateInitialisierung::exitAction()
{

}

void StateInitialisierung::doAction()
{

}

// default constructor
StateInitialisierung::StateInitialisierung()
{

}
// default destructor
StateInitialisierung::~StateInitialisierung()
{

}
#pragma endregion
#pragma region StateBetrieb
void StateBetrieb::entryAction() 
{
    //LCD "Uhrzeit" anzeigen
    Display::locate(0, 0);
    Display::print("Uhrzeit");
}

//-----------------------------------------------------------------------------
void StateBetrieb::exitAction() 
{
    derAutomat.set_BlinkZustand(true);
}

//-----------------------------------------------------------------------------

void StateBetrieb::doAction()
{
    //Zeile 2
    Display::locate(0, 1);
    //Aktuelle Uhrteit ausgeben
    //Display::print("%02u", derAutomat.get_zeit() / (60 * 60));
    //Display::print(":");
    //Display::print("%02u", derAutomat.get_zeit() / 60 % 60);
    //Display::print(":");
    //Display::print("%02u", derAutomat.get_zeit() % 60);

    Display::print("%02u%1s%02u%1s%02u", derAutomat.get_zeit() / (60 * 60),":", derAutomat.get_zeit() / 60 % 60,":", derAutomat.get_zeit() % 60);
    //derAutomat.set_zeit(derAutomat.get_zeit() + 1);
    // --> https://en.wikipedia.org/wiki/Printf#:~:text=The%20type%20field%20can%20be%20any%20of%3A
}	

// default constructor
StateBetrieb::StateBetrieb()
{

}
// default destructor
StateBetrieb::~StateBetrieb()
{

}
#pragma endregion
#pragma region SateStellenStunden
void SateStellenStunden::entryAction() 
{
	Timers::TIM6_start();
	Display::locate(0, 0);
	Display::print("Uhr Stellen: Std");
}

//-----------------------------------------------------------------------------
void SateStellenStunden::exitAction() 
{
    Timers::TIM6_stop();
	derAutomat.set_BlinkZustand(true);
}

//-----------------------------------------------------------------------------

void SateStellenStunden::doAction() 
{
    Display::locate(0, 1);
    //Display::print("%02u%1s%02u%1s%02u", derAutomat.get_zeit() / (60 * 60),":", derAutomat.get_zeit() / 60 % 60,":", derAutomat.get_zeit() % 60);
    if (derAutomat.get_BlinkZustand()) 
        Display::print("%02u", derAutomat.get_zeit() / (60 * 60));
    else
        Display::print("  ");
    Display::print(":");
    Display::print("%02u", derAutomat.get_zeit() / 60 % 60);
    Display::print(":");
    Display::print("%02u", derAutomat.get_zeit() % 60);
}	

// default constructor
SateStellenStunden::SateStellenStunden()
{
    
}
// default destructor
SateStellenStunden::~SateStellenStunden()
{

}
#pragma endregion
#pragma region SateStellenMinuten

void SateStellenMinuten::entryAction() 
{
	Timers::TIM6_start();
	Display::locate(0, 0);
	Display::print("Uhr Stellen: Min");
}

//-----------------------------------------------------------------------------
void SateStellenMinuten::exitAction() 
{
    Timers::TIM6_stop();
	derAutomat.set_BlinkZustand(true);
}

//-----------------------------------------------------------------------------

void SateStellenMinuten::doAction() 
{
    Display::locate(0, 1);
    //Display::print("%02u%1s%02u%1s%02u", derAutomat.get_zeit() / (60 * 60),":", derAutomat.get_zeit() / 60 % 60,":", derAutomat.get_zeit() % 60);
    Display::print("%02u", derAutomat.get_zeit() / (60 * 60));
    Display::print(":");
    if (derAutomat.get_BlinkZustand()) 
        Display::print("%02u", derAutomat.get_zeit() / 60 % 60);
    else
        Display::print("  ");
    Display::print(":");
    Display::print("%02u", derAutomat.get_zeit() % 60);
}	

// default constructor
SateStellenMinuten::SateStellenMinuten()
{
    
}
// default destructor
SateStellenMinuten::~SateStellenMinuten()
{

}
#pragma endregion
#pragma region SateStellenSekunden

void SateStellenSekunden::entryAction() 
{
	Timers::TIM6_start();
	Display::locate(0, 0);
	Display::print("Uhr Stellen: Sek");
}

//-----------------------------------------------------------------------------
void SateStellenSekunden::exitAction() 
{
    Timers::TIM6_stop();
	derAutomat.set_BlinkZustand(true);
}

//-----------------------------------------------------------------------------

void SateStellenSekunden::doAction() 
{
    Display::locate(0, 1);
    //Display::print("%02u%1s%02u%1s%02u", derAutomat.get_zeit() / (60 * 60),":", derAutomat.get_zeit() / 60 % 60,":", derAutomat.get_zeit() % 60);
    Display::print("%02u", derAutomat.get_zeit() / (60 * 60));
    Display::print(":");
    Display::print("%02u", derAutomat.get_zeit() / 60 % 60);
    Display::print(":");
	if (derAutomat.get_BlinkZustand()) 
    	Display::print("%02u", derAutomat.get_zeit() % 60);
	else
	 	Display::print("  ");
}	

// default constructor
SateStellenSekunden::SateStellenSekunden()
{
    
}
// default destructor
SateStellenSekunden::~SateStellenSekunden()
{

}
#pragma endregion