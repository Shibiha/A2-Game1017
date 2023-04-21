#include "Engine.h"
#include <iostream>
#include <string>
#include <sstream>
#include "StateManager.h"
#include "Button.h"
#include "TextureManager.h"
#include "EventManager.h"
#include "RenderManager.h"
#include "FontManager.h"

Engine::Engine() :m_isRunning(false)
{
}

int Engine::Run()
{
	if (m_isRunning)
	{
		return 1; // 1 arbitrarily means that engine is already running.
	}
	if (Init("GAME1017_A1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL))
	{
		return 2; // 2 arbitrarily means that something went wrong in init.
	}
	while (m_isRunning) // Main game loop. Run while isRunning = true.
	{
		Wake();
		HandleEvents(); // Input
		Update();       // Processing
		Render();       // Output
		if (m_isRunning == true)
			Sleep();
	}
	Clean(); // Deinitialize SDL and free up memory.
	return 0;
}

int Engine::Init(const char* title, const int xPos, const int yPos,
	const int width, const int height, const int flags)
{
	std::cout << "Initializing framework..." << std::endl;
	srand((unsigned)time(nullptr)); // Seed random sequence. Only once.
	SDL_Init(SDL_INIT_EVERYTHING);
	REMA::Init(title, xPos, yPos, width, height, flags, flags);
	TEMA::Init();
	EVMA::Init();
	if (Mix_Init(MIX_INIT_MP3) != 0) {
		// Configure mixer
		Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
		Mix_AllocateChannels(16);
		// Load background music
		backgroundMusic.emplace("titleMusic", Mix_LoadMUS("../Assets/mus/titleMusic.mp3"));
		backgroundMusic.emplace("playMusic", Mix_LoadMUS("../Assets/mus/bgMusic.mp3"));

		// Ensuring the background music has no errors
		for (pair<string, Mix_Music*> bgMusic : backgroundMusic)
		{
			if (bgMusic.second == nullptr)
			{
				std::cout << SDL_GetError << std::endl;
			}
		}
	}
	else return false;


	std::cout << SDL_GetError() << std::endl;


	m_fps = 1.0 / (double)FPS; // Converts FPS into a fraction of seconds.
	m_pKeystates = SDL_GetKeyboardState(nullptr);
	lastFrameTime = chrono::high_resolution_clock::now();
	m_isRunning = true; // Start your engine.

	CButton::Init();

	STMA::ChangeState(new TitleState());
	return 0;
}

void Engine::HandleEvents()
{
	EVMA::HandleEvents();
}

void Engine::Wake()
{
	thisFrameTime = chrono::high_resolution_clock::now(); // New snapshot of number of seconds.
	lastFrameDuration = thisFrameTime - lastFrameTime;
	deltaTime = lastFrameDuration.count(); // Now we have our deltaTime multiplier.
	lastFrameTime = thisFrameTime;

	m_start = thisFrameTime; // Comment this out to just use deltaTime.
}

bool Engine::KeyDown(SDL_Scancode c)
{
	if (m_pKeystates != nullptr)
	{
		if (m_pKeystates[c] == 1)
			return true;
	}
	return false;
}

Engine& Engine::Instance()
{
	static Engine instance; // Creating Engine Object
	return instance;
}



void Engine::Update()
{
	//cout << "Updating frame..." << endl;
	string tickLabel = "DT: " + to_string(deltaTime);
	SDL_SetWindowTitle(REMA::GetWindow(), tickLabel.c_str()); // c_str just returns the char array (char *)
	STMA::Update(); // Updating current state
}

void Engine::Sleep()
{
	// Note: Not really better, but you can decide to not limit frameRate and just use deltaTime.
	// Comment all this out to just use deltaTime.
	m_end = chrono::high_resolution_clock::now();
	m_diff = m_end - m_start; // Similar to before, but now chrono and double.
	if (m_diff.count() < m_fps)
		SDL_Delay((Uint32)((m_fps - m_diff.count()) * 1000.0)); // Sleep for number of ms.
}

void Engine::Render()
{
	//cout << "Rendering changes..." << endl;
	STMA::Render();

	SDL_RenderPresent(REMA::GetRenderer()); // Flips the buffers.
}

double Engine::GetDeltaTime()
{
	return deltaTime;
}

void Engine::SetRunning(bool running)
{
	m_isRunning = running;
}


void Engine::Clean()
{
	std::cout << "Cleaning up..." << std::endl;

	// Deletion of renderer and window
	REMA::Quit();

	// Deallocating all background music >:)
	for (pair<string, Mix_Music*> music : backgroundMusic)
	{
		Mix_FreeMusic(music.second);
	}

	backgroundMusic.clear(); // Finally, clearing out the math
	Mix_CloseAudio();
	Mix_Quit();
	STMA::Quit();
	TEMA::Quit();
	EVMA::Quit();
	IMG_Quit();
	SDL_Quit();
}

map<std::string, Mix_Music*> Engine::backgroundMusic; // Stores all the music!
