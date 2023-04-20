// #pragma once
#ifndef __ENGINE_H__
#define __ENGINE_H__
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <chrono>
#include <vector>
#include "Bullet.h"
#include "Enemy.h"
using namespace std;

#define WIDTH 1024
#define HEIGHT 768
#define FPS 60

class Engine
{
public: // Put public heading first so you ALWAYS question why things are public!
	Engine(); // What? What is this?
	int Run();
	
	static vector<Bullet*> m_enemyBulletVec; // Enemy bullets. New for Lab 13.

private:
	// For fixed timestep.
	chrono::time_point<chrono::steady_clock> m_start, m_end;
	chrono::duration<double> m_diff;
	double m_fps; // Changed to double.

	const Uint8* m_pKeystates;
	SDL_Window* m_pWindow; // Pointers are normal variables that hold addresses.
	SDL_Renderer* m_pRenderer; // Pointer to "back buffer"
	bool m_isRunning;

	vector<Bullet*> m_bulletVec; // Vector of bullet addresses (pointers)
	vector<Enemy*> m_enemyVec; // Enemy vector. New for Lab 13.
	vector<Bullet*> m_playerVec;
	int nextCol, nextRow;
	
	
	// Example-specific properties.
	double speed, angle, m_scrollspeed;
	SDL_Texture *m_pShipTexture, *m_pBGTexture, * m_pBulletTexture; // Also a *m_pBulletTexture?
	SDL_Texture* m_pEnemyTexture; // New for Lab 13.
	SDL_Rect m_srcShip, m_srcBG, m_dstBG;
	SDL_FRect m_dstShip; // Floating-point precision.
	SDL_FRect m_bg1, m_bg2;
	int m_enemyCtr, m_enemyMax; // Enemy timer properties. Frame-based.
	Mix_Chunk* m_laser, * m_splode;
	Mix_Music* m_desperateCrusader;

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
