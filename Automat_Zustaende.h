/*
 * Automat_Zustaende.h
 *
 * Created: 22.01.2023 10:32:23
 *  Author: Thomas Heß
 *  Implements all the states of the finite state machine
 *  ApplicationState States of FSM
 */ 


#ifndef CLASSAPPLICATIONSTATES_H_
#define CLASSAPPLICATIONSTATES_H_

#include "Zustand_Oberklasse.h"

#include <stdint.h> 

#pragma region StateInitialisierung

class StateInitialisierung : public State {
  public:
    void entryAction();
    void exitAction();
    void doAction();

    StateInitialisierung();
    ~StateInitialisierung();
    
};
#pragma endregion
#pragma region StateBetrieb

class StateBetrieb : public State {
 private:

  public:
    void entryAction();
    void exitAction();
    void doAction();
    

    StateBetrieb();
    ~StateBetrieb();
};
#pragma endregion
#pragma region SateStellenStunden

class SateStellenStunden : public State
{
  private:

  public:
    void entryAction();
    void exitAction();
    void doAction();

    

    SateStellenStunden();
    ~SateStellenStunden();
};
#pragma endregion
#pragma region SateStellenMinuten

class SateStellenMinuten : public State
{
  private:

  public:
    void entryAction();
    void exitAction();
    void doAction();

    

    SateStellenMinuten();
    ~SateStellenMinuten();
};
#pragma endregion
#pragma region SateStellenSekunden

class SateStellenSekunden : public State
{
  private:

  public:
    void entryAction();
    void exitAction();
    void doAction();

    

    SateStellenSekunden();
    ~SateStellenSekunden();
};
#pragma endregion

#endif /* CLASSAPPLICATIONSTATES_H_ */