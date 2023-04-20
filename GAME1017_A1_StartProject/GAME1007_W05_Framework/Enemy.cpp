#include "Enemy.h"

Enemy::Enemy(const SDL_Rect src, const SDL_FRect dst, float speed)
	:Sprite(src, dst), m_speed(speed) {}

void Enemy::Update(const double dt)
{
	m_dst.x -= GetSpeed() * dt; // dt = deltaTime from Engine.
}

const float Enemy::GetSpeed()
{
	return m_speed;
}