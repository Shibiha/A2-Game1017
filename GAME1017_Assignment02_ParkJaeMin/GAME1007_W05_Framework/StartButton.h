#pragma once
#ifndef __STARTBUTTON_H__
#define __STARTBUTTON_H__

#include "Button.h"

class CStartButton : public CButton
{
public:
	CStartButton() = default;
private:
	void OnEnter() override;
	void Execute() override;
};

#endif //__STARTBUTTON_H__