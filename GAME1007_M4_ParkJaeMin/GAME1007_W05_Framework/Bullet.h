#pragma once
#ifndef _BULLET_H_
#define _BULLET_H_
#include <SDL_rect.h>
#include <SDL_pixels.h>

class Bullet
{
public:
	Bullet(const SDL_FPoint spawn, const double speed, bool fromPlayer = true); // Constructor.
	void Update(const double dt); // Bullet will move itself.
	// void Render(); // What info would this need from the Engine?
	SDL_FRect* GetRect(); // Getter.
	bool IsPlayerBullet();
	bool m_deleteMe;
	SDL_Color m_col;
private:
	bool m_isPlayerBullet; // Is this bullet obj fired from player?
	SDL_FRect m_dst;
	// SDL_Rect m_src; // To use ALL pixels from image make src NULL.
	double m_speed, m_dx, m_dy;
};

#endif