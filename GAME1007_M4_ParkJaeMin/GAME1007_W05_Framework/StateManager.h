//#pragma once
//#ifndef __STATEMANAGER_H__
//#define __STATEMANAGER_H__
//
//#include "State.h"
//#include <vector>
//
//class StateManager //Finite state machine (FSM)
//{ // "static" class, not asingleton
//public:
//	static void Update();
//	static void Render();
//	//Specific StateManager methods:
//	static void PushState(State* pStates);
//	static void PopState(); //Going from Paused back to game
//	static void ChangeState(State* pStates);
//	static void Quit();
//	//accessor/getter for state
//	static std::vector<State*>& GetStates();
//
//private:
//	StateManager() = default;
//	static std::vector<State*> s_states;
//};
//
//typedef StateManager STMA;
//
//#endif
