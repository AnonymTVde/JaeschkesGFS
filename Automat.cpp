/* 
* Automat.cpp
*
* Created: 14.05.2023 19:51:05
* Author: Thomas Heß
*/

 
#include <cstdint>
#include <stddef.h>  // für NULL

#include "Automat.h"



// default constructor
Automat::Automat()
{
    init();
} // Automat

// default destructor
Automat::~Automat()
{
} // ~Automat


void Automat::init()
{

	eineFSM.init(fsmApp,initState,evQueue,EventQueueSize,sizeof(fsmApp)/sizeof(State2State)); // sizeof(fsmApp)/sizeof(State2State) ... uint8_t num_of_entries

    zeit = 0;
    aBlinkZustand = true;
}

void Automat::sendEvent(Events e)
{
	eineFSM.sendEvent(e);
}

bool Automat::process_StateChange()
{
	eineFSM.StateChange();
    return true;
}



uint16_t Automat::fsm_get_timeInState()
{
    return eineFSM.get_timeInState();
}

void Automat::fsm_set_timeInState(uint16_t time_incr)
{
    eineFSM.set_timeInState(time_incr);
}

void Automat::set_zeit(unsigned int pZeit)
{
    zeit = pZeit;
    zeit %= (24*60*60); //damit die Zeit nicht größer als 23:59:59 wird
}

unsigned int Automat::get_zeit()
{
    return zeit;
}

void Automat::set_BlinkZustand(bool pBlinkZustand)
{
    aBlinkZustand = pBlinkZustand;
}

bool Automat::get_BlinkZustand()
{
    return aBlinkZustand;
}