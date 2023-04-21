#pragma once
#ifndef __ENDBUTTON_H__
#define __ENDBUTTON_H__

#include "Button.h"

class CEndButton : public CButton
{
public:
private:
	void OnEnter() override;
	void Execute() override;
};

#endif // !__ENDBUTTON_H__