#pragma once
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include <SDL_rect.h>
#include <SDL_mixer.h>

class Enemy
{
public:
	Enemy(const SDL_FPoint spawn);
	void Update();
	SDL_Rect* GetSrc();
	SDL_FRect* GetRect();
private:
	SDL_Rect m_src;
	SDL_FRect m_dst;
	int m_bulletCtr, m_bulletMax;
	Mix_Chunk* m_enemylaser;
};

#endif