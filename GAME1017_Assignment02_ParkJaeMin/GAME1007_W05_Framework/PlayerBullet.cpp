#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(const SDL_FRect dst, float speed)
	:GameObject(dst), m_speed(speed) {}

void PlayerBullet::Update(const double dt)
{
	m_dst.x += GetSpeed() * dt; // dt = deltaTime from Engine.
}

const float PlayerBullet::GetSpeed()
{
	return m_speed;
}
