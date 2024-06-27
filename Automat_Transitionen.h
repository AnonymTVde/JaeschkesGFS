/*
 * Automat_Transitionen.h
 *
 * Created: 04.11.2023 22:54:01
 *  Author: Thomas Heß
 */ 


#ifndef APPLICATIONTRANSITIONS_H_
#define APPLICATIONTRANSITIONS_H_

#include "Ereignisse_Transition_Oberklasse.h"

#include <stdint.h>

#pragma region Init->Betrieb

class Trans_Initialisierung_Betrieb : public Transition
{
		// properties
		private:
	
		public:
			bool guard();
			bool action();
			Events getEvent();
			bool decision();

            Trans_Initialisierung_Betrieb();
            ~Trans_Initialisierung_Betrieb();

};
#pragma endregion
#pragma region Betrieb (Reflexiv)

class Trans_Betrieb_Betrieb : public Transition
{
		// properties
		private:
	
		public:
			bool guard();
			bool action();
			Events getEvent();
			bool decision();

            Trans_Betrieb_Betrieb();
            ~Trans_Betrieb_Betrieb();

};
#pragma endregion
#pragma region Betrieb -> StellenStunden

class Trans_Betrieb_StellenStunden : public Transition
{
		// properties
		private:
	
		public:
			bool guard();
			bool action();
			Events getEvent();
			bool decision();

            Trans_Betrieb_StellenStunden();
            ~Trans_Betrieb_StellenStunden();

};
#pragma endregion
#pragma region StellenStunden (Reflexiv)

class Trans_StellenStunden_StellenStunden : public Transition
{
		// properties
		private:
	
		public:
			bool guard();
			bool action();
			Events getEvent();
			bool decision();

            Trans_StellenStunden_StellenStunden();
            ~Trans_StellenStunden_StellenStunden();

};
#pragma endregion
#pragma region StellenStunden -> StellenMinuten

class Trans_StellenStunden_StellenMinuten : public Transition
{
		// properties
		private:
	
		public:
			bool guard();
			bool action();
			Events getEvent();
			bool decision();

            Trans_StellenStunden_StellenMinuten();
            ~Trans_StellenStunden_StellenMinuten();

};
#pragma endregion
#pragma region StellenMinuten (Reflexiv)

class Trans_StellenMinuten_StellenMinuten : public Transition
{
		// properties
		private:
	
		public:
			bool guard();
			bool action();
			Events getEvent();
			bool decision();

            Trans_StellenMinuten_StellenMinuten();
            ~Trans_StellenMinuten_StellenMinuten();

};
#pragma endregion
#pragma region StellenMinuten -> StellenSekunden

class Trans_StellenMinuten_StellenSekunden : public Transition
{
		// properties
		private:
	
		public:
			bool guard();
			bool action();
			Events getEvent();
			bool decision();

            Trans_StellenMinuten_StellenSekunden();
            ~Trans_StellenMinuten_StellenSekunden();

};
#pragma endregion
#pragma region StellenSekunden (Reflexiv)

class Trans_StellenSekunden_StellenSekunden : public Transition
{
		// properties
		private:
	
		public:
			bool guard();
			bool action();
			Events getEvent();
			bool decision();

            Trans_StellenSekunden_StellenSekunden();
            ~Trans_StellenSekunden_StellenSekunden();

};
#pragma endregion
#pragma region StellenSekunden -> Betrieb

class Trans_StellenSekunden_Betrieb : public Transition
{
		// properties
		private:
	
		public:
			bool guard();
			bool action();
			Events getEvent();
			bool decision();

            Trans_StellenSekunden_Betrieb();
            ~Trans_StellenSekunden_Betrieb();

};
#pragma endregion

#endif /* APPLICATIONTRANSITIONS_H_ */