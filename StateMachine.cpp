/* 
* StateMachine.cpp
*
* Created: 01.05.2023 10:56:21
* Author: Thomas Heß
*/


#include <cstdint>
#include <stddef.h> // für NULL
#include "StateMachine.h"

// default constructor
StateMachine::StateMachine()
{
} //ClassStateMachine

// default destructor
StateMachine::~StateMachine()
{
} //~ClassStateMachine

void StateMachine::init(State2State *UebTabelle, State *StartZustand, volatile Events *EreignisSpeicher, volatile int GroesseSpeicher, uint8_t AnzahlZeilen_UebTabelle)
{
	StateTransTable = UebTabelle;  // Pointer auf Zustandsübergangstabelle, daher (*(StateTransTable + i)).transition
	InitialState = StartZustand;
	num_of_entries = AnzahlZeilen_UebTabelle;  // Da nur Pointer übergeben werden funktioniert sizeof(StateTransTable nicht)!
	
	evQueue = EreignisSpeicher;  // Pointer auf Ereignisspeicher, daher e = (*(evQueue + evQueueFront))
	evQueueSize = GroesseSpeicher; 
	evQueueFront = 0;
	evQueueRear = 0;
	
	currentState = InitialState;
	currentState->entryAction();

     timeInState = 0;  
	
	
	
}

void StateMachine::sendEvent(Events e)
{
		if (!evQueueIsFull())
		{
			// evQueue[evQueueRear] = e;
			*((evQueue+evQueueRear)) = e;  // evQueue ist ein Pointer auf das 0. Element des Felds, daher die Dereferenzierungsschreibweise!
			
			// evQueueRear um 1 erhöhen um auf den nächsten freien Speicherplatz zu kommen
			// evQueueRear zeigt immer auf einen freien Speicherplatz
			evQueueRear = (evQueueRear+1) % evQueueSize; // 1 := 1, 2 := 2, 3 := 3, 4 := 0;
		}
}

bool StateMachine::evQueueIsEmpty()
{
		if(evQueueFront == evQueueRear) return true;
		else return false;
}

bool StateMachine::evQueueIsFull()
{
		if(((evQueueRear==evQueueSize-1) && (evQueueFront == 0)) || (evQueueRear == evQueueFront-1)) return true;
		else return false;
}

uint16_t StateMachine::get_timeInState()
{
    return timeInState;
    
}

void StateMachine::set_timeInState(uint16_t increment)
{
    timeInState += increment;
}

bool StateMachine::StateChange()
{
	
	StateChanged = false;
	StateOnEntry = currentState;
	
	if (evQueueIsEmpty())  // Also sind in diesem Durchlauf nur do-Aktionen
	{
        StateOnEntry->doAction();
		StateOnExit = StateOnEntry;  // Zustand bleibt unverändert
		StateChanged = false; // und es hat auch kein (nicht mal ein reflexiver aka Selbsttransition) Zustandsübergang stattgefunden
	}
	else
	{
		
		
		// Es liegt also ein Ereignis vor, das abgearbeitet werden muss
		
		// Events e = evQueue[evQueueFront]; // Vom Anfang der Schlange lesen
		
		Events e = *(evQueue + evQueueFront); // Vom Anfang der Schlange lesen
		
		evQueueFront = (evQueueFront + 1) % evQueueSize; // Auf den nächsten in der Schlange setzen

		
		for ( uint8_t i=0; i<num_of_entries; i++ ) // Zustandsfolgetabelle wird durchlaufen
		{
			// (Falls (für den aktuellen Zustand currentState) UND (das aktuelle Ereignis e aus der Schlange) ein Eintrag in der Tabelle existiert)
			if (   ( (currentState == (*(StateTransTable + i)).currentState) && (e == (*(StateTransTable + i)).transition->getEvent() ) )    )
			
            {
				
					
                    // timeInState wird in der TIM7 Timer_isr um 1 erhöht. Dadurch gibt es auch bei nicht
                    // Tick-Ereignissen eine Verweildauer im Zustand, die z.B. als Guard() ausgewertet werden kann
					
							if( (*(StateTransTable + i)).transition->guard() == true) // Wächterbedingung für den Übergang ist erfüllt?
							{
								// Zustandsübergang erfolgt nun
								StateOnEntry->exitAction(); // Die exitAction() muss aufgerufen werden BEVOR die Transitions-Action() aufgerufen wird!
								(*(StateTransTable + i)).transition->action(); // Transitionsaktion (aka "Verhalten") ausführen
								
								if( (*(StateTransTable + i)).altState == NULL) // Falls der alternative Zustand NULL ist, also kein Entscheidungsknoten
								{
									currentState = (*(StateTransTable + i)).nextState;
								}
								else // Es ist also ein Entscheidungsknoten, folglich Decision() auswerten!
								{
									if( (*(StateTransTable + i)).transition->decision() == true) // Falls Bedingung für den Entscheidungsknoten erfüllt
									{
										currentState = (*(StateTransTable + i)).nextState;
									}
									else // Bedingung für den Entscheidungsknoten ist nicht erfüllt
									{
										currentState = (*(StateTransTable + i)).altState;
									}
								}
								
								
								StateOnExit = currentState; // Zustand beim Austritt festlegen
								
								timeInState = 0;
								StateChanged = true;
                                // Entry-Methode des (neuen) Folgezustands aufrufen
                                // Die Exit-Methode des Ausgangszustands wurde bereits oben ausgeführt
                                StateOnExit->entryAction();
							}
							else // Wächterbedingung nicht erfüllt, kein Zustandsübergang!
							{
								StateChanged = false;
                                // Do-Methode des Ausgangszustands aufrufen
                                StateOnEntry->doAction();
							}
						
				
				
				break; // for-Schleife wird verlassen!  DAMIT DAS EREIGNIS e FÜR DIESEN ÜBERGANG AUFGEBRAUCHT IST!
				// Wenn sich currentState verändert hat kann es sein, dass nachfolgende Zeilen der FSM-Tabelle
				// noch einen Eintrag enthalten, der dann doch wieder zu einer Aktion führt obwohl die StateMachine für diesen
				// Fall gar nicht durchlaufen werden dürfte. Problem: "Altes Ereignis" <-> neuer Zustand
				// Also: Für den EntryState wird das Ereignis abgearbeitet und ggf. der Zustand gewechselt
				// Dann muss aber die Schleife verlassen werden, damit nach der Aktualisierung der Ausgänge
				// die Tabelle wieder von neuem mit dem dann aktuellen EntryState samt nächstem Ereignis durchlaufen wird.
				
			}
		} // of for()
	} // of if (evQueueIsEmpty())
	

	return true;
}

