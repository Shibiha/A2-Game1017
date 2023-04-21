#pragma once
#ifndef __FRAME_H__
#define __FRAME_H__

#include <array>

struct Frame {

	Frame(int inX, int inY, int inW, int inH) {
		x = inX;
		y = inY;
		w = inW;
		h = inH;
	}
	Frame(std::array<int, 4> vec4) { 
		x = vec4[0];
		y = vec4[1];
		w = vec4[2];
		h = vec4[3];
	}
	const char* name{};
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
};

#endif //!__FRAME_H__