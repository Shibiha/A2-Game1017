#pragma once
#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "GameObject.h"

class Background : public GameObject
{
public:
	Background(const SDL_Rect src, const SDL_FRect dst, float speed, std::string path, std::string key);
	void Update();
	void Render();

private:
	SDL_Rect m_src;
	float m_scrollSpeed;
	std::string key;
	SDL_FPoint m_start;
	SDL_FPoint m_end;
};

#endif //!__BACKGROUND_H__