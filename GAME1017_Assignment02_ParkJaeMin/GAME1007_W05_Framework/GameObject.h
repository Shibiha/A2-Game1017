#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
#include <SDL_rect.h>
#include "Spritesheet.h"

enum AnimState { STATE_IDLING, STATE_RUNNING, STATE_JUMPING, STATE_DEATH, STATE_DUCKING };

class GameObject
{
public:
	SDL_FRect* GetDst();
	SDL_FPoint GetCenter() const;
	virtual void Update() = 0;
	virtual void Render() = 0;
	const bool GetEnabled() const;
	void SetEnabled(const bool e);

protected: // private but inherited.
	SDL_FRect m_dst;
	bool m_enabled;
	GameObject();
	GameObject(const SDL_FRect dst);
};

class Sprite : public GameObject
{
public:
	SDL_Rect* GetSrc();
	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	SDL_Rect m_src;
	Sprite(const SDL_Rect src, const SDL_FRect dst);
};

class AnimatedSprite : public Sprite
{
public:
	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	unsigned short m_frame, m_frameMax, m_sprite, m_spriteMin, m_spriteMax;
	AnimState m_state;
	SpriteSheet* m_spriteSheet;
	Animation m_currentAnimation;
	void SetSpriteSheet(SpriteSheet* sprite);
	SpriteSheet* GetSpriteSheet();
	AnimatedSprite(const SDL_Rect src, const SDL_FRect dst, AnimState state);
	void SetAnimation(AnimState state, const unsigned short frameMax, const unsigned short spriteMin,
		const unsigned short spriteMax, const int srcY = 0);
	void SetAnimation(AnimState state, std::string animation_name);
	void Animate();
};

#endif