/* 
* Ereignisse_Transition_Oberklasse.h
*
* Created: 12.02.2023 22:37:38
* Author: Thomas Heß
* Alle Transitionen erben aus dieser Basisklasse
* Die Methoden werden in Automat_Transitionen implementiert
*/


#ifndef __CLASSTRANSITION_H__
#define __CLASSTRANSITION_H__

#include <stdint.h>

/* Define all the possible Events here   */
enum Events {
	// System Events
	NoEvent,
    Tick, // Timer7 abgelaufen --> 1s
	

	// User Events
	TasterStart,
    TasterWechseln,
    TasterPlus
 
    


	// Hardware Events
    

};


class Transition
{
	
// attributes
protected: Events ereignis;

//functions
public:

	virtual bool guard()=0; // Wächterbedingung
	virtual bool action()=0; // Aktion beim Übergang
	virtual Events getEvent()=0; // Für diesen Übergang konfiguriertes Ereignis auslesen

// für Entscheidungsknoten
	virtual bool decision()=0; // wertet irgendeine Bedingung aus ,z.B. aAnzOffen > 0 (s. FS) und StateChange() reagiert 
								// entsprechend mit einem der beiden Zielzustände, 
								// wird nur aufgerufen, wenn der altState in der fsmApp-Tabelle nicht NULL ist
								// UND Guard erfüllt usw. ...
	

}; //Ereignisse_Transition_Oberklasse

#endif //__CLASSTRANSITION_H__
