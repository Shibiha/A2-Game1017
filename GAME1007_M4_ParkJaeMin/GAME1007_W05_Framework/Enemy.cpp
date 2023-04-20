#include "Enemy.h"
#include "Engine.h" // For static vector access.
#include <iostream>

Enemy::Enemy(const SDL_FPoint spawn)
{
	m_src = { 64, 0, 64, 64 };
	m_dst = { spawn.x - 32.0f, spawn.y - 32.0f, 64.0f, 64.0f };
	m_bulletCtr = 0;
	m_bulletMax = 60 + rand() % 121;
}

void Enemy::Update()
{
	// Update timer and spawn Bullet.
	if (m_bulletCtr++ % m_bulletMax == 0) // Spawn a new enemy.
	{
		m_enemylaser = Mix_LoadWAV("../Assets/aud/laserEnemy.wav");
		Mix_PlayChannel(-1, m_enemylaser, 0);
		Bullet* temp = new Bullet(
			{ m_dst.x + m_dst.w / 2, m_dst.y + m_dst.h / 2 },
			-400.0, false); 
		temp->m_col = { (Uint8)(rand() % 255), (Uint8)(rand() % 255), (Uint8)(rand() % 255), 255 };
		Engine::m_enemyBulletVec.push_back( temp );
	}
}

SDL_Rect* Enemy::GetSrc()
{
	return &m_src;
}

SDL_FRect* Enemy::GetRect()
{
	return &m_dst;
}