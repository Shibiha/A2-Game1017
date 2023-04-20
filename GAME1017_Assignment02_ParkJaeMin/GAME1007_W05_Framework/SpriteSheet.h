#pragma once
#ifndef __SPRITE_SHEET_H__
#define __SPRITE_SHEET_H__

#include "Animation.h"
#include <vector>
#include <unordered_map>
#include <SDL.h>
#include <SDL_image.h>

class SpriteSheet {
public:
	SpriteSheet(const char* name, SDL_Texture* texture);
	SpriteSheet();
	~SpriteSheet();

	// Accessors
	Animation GetAnimation(const std::string& anim_name);
	[[nodiscard]] SDL_Texture* GetTexture() const;

	// Mutators
	void AddAnimation(const Animation& anim);
	void SetTexture(SDL_Texture* texture);

private:
	std::string m_name;

	std::unordered_map<std::string, Animation> m_animations;

	SDL_Texture* m_pTexture;
};

#endif // __SPRITE_SHEET_H__