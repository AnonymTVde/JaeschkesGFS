/*
 * Automat_Transitionen.cpp
 *
 * Created: 12.02.2023 23:15:36
 *  Author: Thomas Heß
 */ 

#include <stdint.h>

#include "Automat_Transitionen.h"
#include "Hardware_Methoden.h"  // Für die Transitionsaktionen
#include "Variable_Global.h"    // Für Zugriff auf Methoden des Automaten

#pragma region Init->Betrieb
// default constructor
Trans_Initialisierung_Betrieb::Trans_Initialisierung_Betrieb()
{
    ereignis = NoEvent; // unbedingter Übergang, also muss am Ende der entry() für Initialieierung
                        // sendEvent(NoEvent) kommen um diesen Übergang auszulösen.
}

bool Trans_Initialisierung_Betrieb::guard()
{
    return true;
}
bool Trans_Initialisierung_Betrieb::action()
{
    return true;
}
Events Trans_Initialisierung_Betrieb::getEvent()
{
    return ereignis;
}
bool Trans_Initialisierung_Betrieb::decision()
{
    return true;
}

// default destructor
Trans_Initialisierung_Betrieb::~Trans_Initialisierung_Betrieb()
{

}
#pragma endregion
#pragma region Betrieb -> Betrieb

// default constructor
Trans_Betrieb_Betrieb::Trans_Betrieb_Betrieb()
{
    ereignis = Tick;
}

bool Trans_Betrieb_Betrieb::guard()
{
    return true;
}
bool Trans_Betrieb_Betrieb::action()
{
    derAutomat.set_zeit(derAutomat.get_zeit() + 1);
    return true;
}

Events Trans_Betrieb_Betrieb::getEvent()
{
    return ereignis;
}

bool Trans_Betrieb_Betrieb::decision()
{
    return true;
}

// default destructor
Trans_Betrieb_Betrieb::~Trans_Betrieb_Betrieb()
{

}
#pragma endregion
#pragma region Betrieb -> StellenStunden

// default constructor
Trans_Betrieb_StellenStunden::Trans_Betrieb_StellenStunden()
{
    ereignis = TasterWechseln;
}

bool Trans_Betrieb_StellenStunden::guard()
{
    return true;
}
bool Trans_Betrieb_StellenStunden::action()
{
    Display::cls();
    return true;
}

Events Trans_Betrieb_StellenStunden::getEvent()
{
    return ereignis;
}

bool Trans_Betrieb_StellenStunden::decision()
{
    return true;
}

// default destructor
Trans_Betrieb_StellenStunden::~Trans_Betrieb_StellenStunden()
{

}
#pragma endregion
#pragma region StellenStunden (Reflexiv)

// default constructor
Trans_StellenStunden_StellenStunden::Trans_StellenStunden_StellenStunden()
{
    ereignis = TasterPlus;
}

bool Trans_StellenStunden_StellenStunden::guard()
{
    return true;
}
bool Trans_StellenStunden_StellenStunden::action()
{
    derAutomat.set_zeit(derAutomat.get_zeit() + 3600);
    return true;
}

Events Trans_StellenStunden_StellenStunden::getEvent()
{
    return ereignis;
}

bool Trans_StellenStunden_StellenStunden::decision()
{
    return true;
}

// default destructor
Trans_StellenStunden_StellenStunden::~Trans_StellenStunden_StellenStunden()
{

}
#pragma endregion
#pragma region StellenStunden -> StellenMinuten

// default constructor
Trans_StellenStunden_StellenMinuten::Trans_StellenStunden_StellenMinuten()
{
    ereignis = TasterWechseln;
}

bool Trans_StellenStunden_StellenMinuten::guard()
{
    return true;
}
bool Trans_StellenStunden_StellenMinuten::action()
{
    Display::cls();
    return true;
}

Events Trans_StellenStunden_StellenMinuten::getEvent()
{
    return ereignis;
}

bool Trans_StellenStunden_StellenMinuten::decision()
{
    return true;
}

// default destructor
Trans_StellenStunden_StellenMinuten::~Trans_StellenStunden_StellenMinuten()
{

}
#pragma endregion
#pragma region StellenMinuten (Reflexiv)

// default constructor
Trans_StellenMinuten_StellenMinuten::Trans_StellenMinuten_StellenMinuten()
{
    ereignis = TasterPlus;
}

bool Trans_StellenMinuten_StellenMinuten::guard()
{
    return true;
}
bool Trans_StellenMinuten_StellenMinuten::action()
{
    derAutomat.set_zeit(derAutomat.get_zeit() + 60);
    return true;
}

Events Trans_StellenMinuten_StellenMinuten::getEvent()
{
    return ereignis;
}

bool Trans_StellenMinuten_StellenMinuten::decision()
{
    return true;
}

// default destructor
Trans_StellenMinuten_StellenMinuten::~Trans_StellenMinuten_StellenMinuten()
{

}
#pragma endregion
#pragma region StellenMinuten -> StellenSekunden

// default constructor
Trans_StellenMinuten_StellenSekunden::Trans_StellenMinuten_StellenSekunden()
{
    ereignis = TasterWechseln;
}

bool Trans_StellenMinuten_StellenSekunden::guard()
{
    return true;
}
bool Trans_StellenMinuten_StellenSekunden::action()
{
    Display::cls();
    return true;
}

Events Trans_StellenMinuten_StellenSekunden::getEvent()
{
    return ereignis;
}

bool Trans_StellenMinuten_StellenSekunden::decision()
{
    return true;
}

// default destructor
Trans_StellenMinuten_StellenSekunden::~Trans_StellenMinuten_StellenSekunden()
{

}
#pragma endregion
#pragma region StellenSekunden (Reflexiv)

// default constructor
Trans_StellenSekunden_StellenSekunden::Trans_StellenSekunden_StellenSekunden()
{
    ereignis = TasterPlus;
}

bool Trans_StellenSekunden_StellenSekunden::guard()
{
    return true;
}
bool Trans_StellenSekunden_StellenSekunden::action()
{
    derAutomat.set_zeit(derAutomat.get_zeit() + 1);
    return true;
}

Events Trans_StellenSekunden_StellenSekunden::getEvent()
{
    return ereignis;
}

bool Trans_StellenSekunden_StellenSekunden::decision()
{
    return true;
}

// default destructor
Trans_StellenSekunden_StellenSekunden::~Trans_StellenSekunden_StellenSekunden()
{

}
#pragma endregion
#pragma region StellenSekunden -> Betrieb

// default constructor
Trans_StellenSekunden_Betrieb::Trans_StellenSekunden_Betrieb()
{
    ereignis = TasterWechseln;
}

bool Trans_StellenSekunden_Betrieb::guard()
{
    return true;
}
bool Trans_StellenSekunden_Betrieb::action()
{
    Display::cls();
    return true;
}

Events Trans_StellenSekunden_Betrieb::getEvent()
{
    return ereignis;
}

bool Trans_StellenSekunden_Betrieb::decision()
{
    return true;
}

// default destructor
Trans_StellenSekunden_Betrieb::~Trans_StellenSekunden_Betrieb()
{

}
#pragma endregion