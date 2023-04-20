#include "Bullet.h"

Bullet::Bullet(const SDL_FPoint spawn, const double speed, bool fromPlayer)
{
	m_isPlayerBullet = fromPlayer;
	if (m_isPlayerBullet)
		m_dst = { spawn.x - 16.0f, spawn.y - 16.0f, 32.0f, 32.0f };
	else
		m_dst = { spawn.x - 8.0f, spawn.y - 8.0f, 16.0f, 16.0f };
	
	// m_src = {x,y,w,h};
	this->m_speed = speed;
	// this-> is a pointer to the object that invokes the function.
	// this-> is optional and rarely used. But could be.
	m_dx = 1.0;
	m_dy = 0.0; // No movement on Y axis.
}

void Bullet::Update(const double dt)
{
	if (m_deleteMe == true)
		return;
	m_dst.x += m_dx * m_speed * dt;
	// m_dst.y += m_dy * m_speed * dt;
	// Check if bullet goes off-screen. If so, set m_deleteMe to true.
}

SDL_FRect* Bullet::GetRect()
{
	return &m_dst;
}

bool Bullet::IsPlayerBullet()
{
	return m_isPlayerBullet;
}
