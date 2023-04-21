#pragma once
#ifndef __PAUSEBUTTON_H__
#define __PAUSEBUTTON_H__

#include "Button.h"

class CPauseButton : public CButton
{
public:
private:
	void OnEnter() override;
	void Execute() override;
};

#endif // !__PAUSEBUTTON_H__