#include "StartButton.h"

#include "Engine.h"


void CStartButton::OnEnter()
{
	CButton::s_currentTexture = s_buttonTextures[START];
	m_dst = { 0, 0, 0, 0 }; // If we don't init it throws a nice nullptr error.
}

// When button is pressed
void CStartButton::Execute()
{
	STMA::ChangeState(new GameState()); // Going into the game!
	CButton::s_currentButton = s_buttons[PAUSE];
	CButton::s_currentTexture = s_buttonTextures[PAUSE];
}
