/* 
* StateMachine.h
*
* Created: 01.05.2023 10:56:21
* Author: Thomas Heß
*/


#ifndef __CLASSSTATEMACHINE_H__
#define __CLASSSTATEMACHINE_H__
#include "Zustand_Oberklasse.h"
#include "Ereignisse_Transition_Oberklasse.h" // Definition of all possible Events
#include "Automat_Transitionen.h"
#include "Automat_Zustaende.h"

typedef struct  {
	State*      currentState;
	Transition* transition;
	
	State*      nextState;
	State*      altState; // Für Entscheidungsknoten, wenn die Entscheidung negativ ausfällt
} State2State;
#include <cstdint>

class StateMachine
{
//variables
public:
protected:
private:
		State2State *StateTransTable;  // Pointer auf Zustandsübergangstabelle
		
		State *InitialState;  // Startzustand
		volatile Events *evQueue;  // Pointer auf den Ereignisspeicher (Ringspeicher, Schlange)
		
		uint8_t num_of_entries; // Anzahl Zeilen der Zustandsfolgetabelle
		
		uint16_t timeInState = 0;
		State* currentState;

		State* StateOnEntry;  // Zustand beim Eintritt in StateChange()
		State* StateOnExit;   // Zustand beim Austritt aus StateChange()
		bool StateChanged;  // true, wenn ein Zustandswechsel erfolgt ist, ansonsten false

		volatile int evQueueSize; // Größe des Ereignisspeichers
		volatile int evQueueFront;  // index begin data .. von hier werden Daten ausgelesen, Anfang der Schlange
		volatile int evQueueRear;  // index stop data ... Hier werden Daten eingefügt, Ende der Schlange

//functions
public:
	StateMachine();
	~StateMachine();
	void init(State2State*, State*, volatile Events*, volatile int, uint8_t);
	void sendEvent(Events);
	bool StateChange();
	
    uint16_t get_timeInState();
    void set_timeInState(uint16_t);

private:
	bool evQueueIsEmpty();
	bool evQueueIsFull();

}; //StateMachine

#endif //__CLASSSTATEMACHINE_H__
