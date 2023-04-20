#pragma once
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <vector>
#include <string>
#include "Frame.h"

struct Animation {
	std::string name;
	std::vector<Frame> m_frames;
	int current_frame = 0;
};

#endif //!__ANIMATION_H__