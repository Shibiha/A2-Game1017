#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__
#include <map>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "Spritesheet.h"

class TextureManager
{
public: // Public methods.
	static void Init();
	static void Load(const char* path, const std::string key); 
	static void LoadSpriteMap(const char* text_file, const char* sprite_file, std::string key);
	static void Unload(const std::string key);
	static SDL_Texture* GetTexture(const std::string key);
	static SpriteSheet* GetSpriteSheet(const std::string key);
	static void Quit();

private: // Private members.
	static std::map<std::string, SDL_Texture*> s_textures;
	TextureManager() = default;
	static std::map<std::string, SpriteSheet*> s_spritesheets;
};

typedef TextureManager TEMA;

#endif