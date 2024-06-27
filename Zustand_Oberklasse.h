/*
 * Zustand_Oberklasse.h
 *
 * Created: 22.01.2023 10:24:22
 *  Author: Thomas Heß
 * This is the base class from which all the states of the fsm should be inherited
 */ 


#ifndef __CLASSSTATE_H__
#define __CLASSSTATE_H__

#include <stdint.h>

class State {
	public:
	virtual void entryAction() = 0;
	virtual void exitAction() = 0;
    virtual void doAction() = 0;

};



#endif /* __CLASSSTATE_H__ */