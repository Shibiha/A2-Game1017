#include "EndButton.h"

void CEndButton::OnEnter()
{
	CButton::s_currentTexture = s_buttonTextures[END];
	m_dst = { 0,0,0,0 };
}


void CEndButton::Execute()
{
	STMA::ChangeState(new TitleState());
	s_currentButton = s_buttons[START];
	s_currentTexture = s_buttonTextures[START];
	isEnabled = false;
}
