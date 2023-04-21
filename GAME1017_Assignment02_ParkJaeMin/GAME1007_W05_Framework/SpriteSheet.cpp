#include "Spritesheet.h"

SpriteSheet::SpriteSheet(const char* name, SDL_Texture* texture)
{
	m_name = static_cast<std::string>(name);
	m_pTexture = texture;
}

SpriteSheet::SpriteSheet()
= default;

SpriteSheet::~SpriteSheet()
= default;

Animation SpriteSheet::GetAnimation(const std::string& anim_name)
{
	return m_animations[anim_name];
}

SDL_Texture* SpriteSheet::GetTexture() const
{
	return m_pTexture;
}

void SpriteSheet::AddAnimation(const Animation& anim)
{
	m_animations.emplace(anim.name, anim);
}

void SpriteSheet::SetTexture(SDL_Texture* texture)
{
	m_pTexture = texture;
}
