// #pragma once
#ifndef __ENGINE_H__
#define __ENGINE_H__
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <chrono>
#include <vector>
#include "GameObject.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"
using namespace std;

#define WIDTH 1024
#define HEIGHT 768
#define FPS 60

class Engine
{
public: // Put public heading first so you ALWAYS question why things are public!
	int Run();
	static Engine& Instance(); // Access	or for Engine object
	static map<std::string, Mix_Music*> backgroundMusic; // Stores all the music!
	bool KeyDown(SDL_Scancode);
	double GetDeltaTime();
	void SetRunning(bool run);

private:
	// For fixed timestep.
	chrono::time_point<chrono::steady_clock> m_start, m_end;
	chrono::duration<double> m_diff;
	double m_fps; // Changed to double.

	const Uint8* m_pKeystates;
	SDL_Window* m_pWindow; // Pointers are normal variables that hold addresses.
	SDL_Renderer* m_pRenderer; // Pointer to "back buffer"
	bool m_isRunning;

	//// Example-specific properties.
	//SDL_Texture* m_pBGTexture;
	//SDL_Texture* m_pEnemyTexture;
	//Sprite* m_pBackground;
	//Player* m_pPlayer;
	//Mix_Chunk* m_pSlacker;
	//Mix_Chunk* m_pJump;
	//Mix_Music* m_pGuile;
	//vector<PlayerBullet*> m_playerBullets;
	//vector<Enemy*> m_enemies;
	//int m_eCtr, m_eCtrMax;

	chrono::time_point<chrono::steady_clock> lastFrameTime, thisFrameTime; // Cleaned this up.
	chrono::duration<double> lastFrameDuration;
	double deltaTime;

	int Init(const char*, const int, const int, const int, const int, const int);
	void HandleEvents();
	void Wake();
	bool KeyDown(SDL_Scancode);
	void Update();
	void Sleep();
	void Render();
	void Clean();	
};

#endif
