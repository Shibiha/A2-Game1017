// #pragma once
#ifndef __ENGINE_H__
#define __ENGINE_H__
#include <SDL.h>
#include <SDL_image.h>
#include <chrono>
#include <vector>
#include <map>
#include <string>
#include <SDL_mixer.h>

using namespace std;

const int WIDTH = 1536;
const int HEIGHT = 768;
const int FPS = 120;
const int PAUSE_WIDTH = 768;
const int PAUSE_HEIGHT = 384;


class Engine
{
public: // Put public heading first so you ALWAYS question why things are public!
	int Run();
	static Engine& Instance(); // Access	or for Engine object
	static map<std::string, Mix_Music*> backgroundMusic; // Stores all the music!
	bool KeyDown(SDL_Scancode);
	double GetDeltaTime();
	void SetRunning(bool run);

	// Yes I know this is not the greatest, but I don't want unneccesary accessors and not keen on creating an EventManager component at the moment.
	SDL_FPoint m_mousePosition;
private:
	Engine();
	// For fixed timestep.
	chrono::time_point<chrono::steady_clock> m_start, m_end;
	chrono::duration<double> m_diff;
	double m_fps; // Changed to double.

	const Uint8* m_pKeystates;
	bool m_isRunning;

	// Example-specific properties.
	//double speed;
	//SDL_Texture *m_pShipTexture, *m_pBGTexture, *m_pBulTexture, *m_pEnemyTexture;
	//SDL_Rect m_srcShip;
	//Player* m_ship; // Floating-point precision.
	//SDL_FRect m_bg1, m_bg2;
	//float m_scrollSpeed;
	//int m_enemyCtr, m_enemyMax; // Enemy timer properties. Frame-based
	//vector<Bullet*>  m_bulletVec;
	//vector<Enemy*> m_enemyVec;
	//Mix_Music* m_bgMusic;
	//bool plrCanShoot;
	//map<string, Mix_Chunk*> m_sfx; // Holding player shoot, enemy shoot, and collider sounds

	chrono::time_point<chrono::steady_clock> lastFrameTime, thisFrameTime; // Cleaned this up.
	chrono::duration<double> lastFrameDuration;
	double deltaTime;

	int Init(const char*, const int, const int, const int, const int, const int);
	void HandleEvents();
	void Wake();
	void Update();
	void Sleep();
	void Render();
	void Clean();
};

#endif
