#include "GameObject.h"
#include <iostream>

GameObject::GameObject() :m_dst({ 0.0f, 0.0f, 0.0f, 0.0f }), m_enabled(true) {}

GameObject::GameObject(const SDL_FRect dst)
	:m_dst(dst), m_enabled(true) {}

SDL_FRect* GameObject::GetDst()
{
	return &m_dst;
}

SDL_FPoint GameObject::GetCenter() const
{
	return { m_dst.x + m_dst.w / 2, m_dst.y + m_dst.h / 2 };
}

const bool GameObject::GetEnabled() const
{
	return m_enabled;
}

void GameObject::SetEnabled(const bool e)
{
	m_enabled = e;
}

Sprite::Sprite(const SDL_Rect src, const SDL_FRect dst)
	:GameObject(dst), m_src(src) {}

SDL_Rect* Sprite::GetSrc()
{
	return &m_src;
}

void AnimatedSprite::SetSpriteSheet(SpriteSheet* sprite)
{
	m_spriteSheet = sprite;
}

SpriteSheet* AnimatedSprite::GetSpriteSheet()
{
	return m_spriteSheet;
}

AnimatedSprite::AnimatedSprite(const SDL_Rect src, const SDL_FRect dst, AnimState state)
	:Sprite(src, dst), m_state(state), m_frame(0), m_frameMax(0), m_sprite(0), m_spriteMin(0), m_spriteMax(0), m_spriteSheet(nullptr) {}

void AnimatedSprite::SetAnimation(AnimState state, const unsigned short frameMax, const unsigned short spriteMin,
	const unsigned short spriteMax, const int srcY)
{
	m_state = state;
	m_frame = 0;
	m_frameMax = frameMax;
	m_sprite = m_spriteMin = spriteMin;
	m_spriteMax = spriteMax;
	m_src.x = m_src.w * m_sprite;
	m_src.y = srcY;
}
void AnimatedSprite::SetAnimation(AnimState state, std::string animation_name)
{
	if (m_spriteSheet != nullptr) {
		m_currentAnimation = m_spriteSheet->GetAnimation(animation_name);
		m_state = state;
		m_frame = m_currentAnimation.current_frame = 0;

		m_frameMax = m_currentAnimation.m_frames.size();
		m_sprite = m_spriteMin = m_currentAnimation.m_frames[m_currentAnimation.current_frame].w % 128;
		m_spriteMax = m_currentAnimation.m_frames.size();
		m_src.w = m_currentAnimation.m_frames[m_currentAnimation.current_frame].w;
		m_src.h = m_currentAnimation.m_frames[m_currentAnimation.current_frame].h;


		m_src.x = m_src.w * m_sprite;
		m_src.y = m_currentAnimation.m_frames[m_currentAnimation.current_frame].y;
	}
	else {
		std::cout << "spritesheet does not exist!";
	}

}
void AnimatedSprite::Animate()
{
	if (m_frame++ == m_frameMax)
	{
		m_frame = 0;
		m_currentAnimation.current_frame = m_frame;
		if (++m_sprite == m_spriteMax)
			m_sprite = m_spriteMin; // I don't use % in case m_sprite doesn't start at 0.
		m_src.x = m_src.w * m_sprite; // Moved to outside if.

	}
}
