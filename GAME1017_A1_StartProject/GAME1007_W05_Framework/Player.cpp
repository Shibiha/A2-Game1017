#include "Player.h"

Player::Player(const SDL_FRect dst, float speed)
	:GameObject(dst), m_speed(speed){}

const float Player::GetSpeed()
{
	return m_speed;
}
