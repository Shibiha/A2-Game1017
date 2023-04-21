#include "PauseButton.h"

void CPauseButton::OnEnter()
{
	CButton::s_currentTexture = s_buttonTextures[PAUSE];
	m_dst = { 0,0,0,0 };
}

void CPauseButton::Execute()
{
	STMA::PopState();
	isEnabled = false;
}
