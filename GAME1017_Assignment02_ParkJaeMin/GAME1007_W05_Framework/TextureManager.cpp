#include "TextureManager.h"
#include "RenderManager.h"
#include "Animation.h"
#include "Frame.h"

#include <iostream>
#include <fstream>
#include <array>

void TextureManager::Init()
{
	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
	{
		std::cout << "Image init success" << std::endl;
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
		SDL_SetRenderDrawBlendMode(REMA::GetRenderer(), SDL_BLENDMODE_BLEND);
	}
	else
		std::cout << "Image init failed: ERROR - " << IMG_GetError() << std::endl; // Error for image
}

void TextureManager::Load(const char* path, const std::string key)
{
	SDL_Texture* temp = IMG_LoadTexture(REMA::GetRenderer(), path);
	if (temp == nullptr) // Loading texture check
		std::cout << "Could not load texture: Error - " << IMG_GetError() << std::endl; 
	else
		s_textures.emplace(key, temp);
}

void TextureManager::LoadSpriteMap(const char* text_file, const char* sprite_file, std::string key)
{
	Load(sprite_file, key);

	std::ifstream data_file;

	std::string anim_name = "";
	std::string prev_anim_name = "";
	std::array<int, 4> vec4 = { 0, 0, 0, 0 };
	Animation new_anim;
	SpriteSheet* new_spriteSheet = new SpriteSheet(key.c_str(), s_textures[key]); // When creating a spritesheet

	data_file.open(text_file);

	if (data_file.is_open()) { // if file is obtainable
		while (data_file >> anim_name >> vec4[0] >> vec4[1] >> vec4[2] >> vec4[3]) { 
			if (prev_anim_name == anim_name) { // Same animation
				new_anim.m_frames.push_back(Frame(vec4)); // addition to the current animation
			}
			else {
				if (new_anim.name != anim_name && new_anim.name != "") { // When starting an animation.
					new_spriteSheet->AddAnimation(new_anim);
				}
				new_anim = Animation();
				new_anim.name = anim_name;
				new_anim.m_frames.push_back(Frame(vec4)); // Push back newly made frames with values
			}

			prev_anim_name = anim_name;

		}
		new_spriteSheet->AddAnimation(new_anim); // Needed for final animation that does not get added.
	}
	else {
		std::cout << "Could not process .txt file, probability a spelling error\n\n";
	}

	s_spritesheets.emplace(key, new_spriteSheet);
}


void TextureManager::Unload(const std::string key) // Unload
{
	if (s_textures.find(key) != s_textures.end())
	{
		SDL_DestroyTexture(s_textures[key]);
		s_textures.erase(key);
	}
	else
		std::cout << "Could not unload '" << key << "' - id not found!" << std::endl;
}

SDL_Texture* TextureManager::GetTexture(const std::string key) { return s_textures[key]; } //Get Texture
SpriteSheet* TextureManager::GetSpriteSheet(const std::string key) { return s_spritesheets[key]; } //Get SpriteSheet


void TextureManager::Quit()
{
	for (auto const& i : s_textures)
	{
		SDL_DestroyTexture(s_textures[i.first]);
	}
	s_textures.clear();
	IMG_Quit();
}

std::map<std::string, SDL_Texture*> TextureManager::s_textures;
std::map<std::string, SpriteSheet*> TextureManager::s_spritesheets;
