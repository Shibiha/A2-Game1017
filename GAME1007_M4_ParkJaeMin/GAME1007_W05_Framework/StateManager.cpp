//#include "StateManager.h"
//
//void StateManager::Update()
//{
//	if (!s_states.empty()) //if state vector is not empty
//		s_states.back()->Update(); // Invoke Update of the "current" state
//}
//
//void StateManager::Render()
//{
//	if (!s_states.empty()) //if state vector is not empty
//		s_states.back()->Update(); // Invoke Update of the "current" state
//}
//
//void StateManager::PushState(State* pState)
//{
//
//}
//
//void StateManager::PopState()
//{
//
//}
//
//void StateManager::ChangeState(State* pState)
//{
//
//}
//
//void StateManager::Quit()
//{
//
//}
//
//std::vector<State*>& StateManager::GetStates()
//{
//	return s_states;
//}
//
//std::vector<State*> StateManager::s_states; // NOW this static object will be allocated