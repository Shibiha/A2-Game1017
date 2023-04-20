#include "Engine.h"
#include <iostream>
#include <string>
using namespace std;

Engine::Engine():m_pWindow(nullptr), m_pRenderer(nullptr), m_isRunning(false)
{
	m_eCtr = 0;
	m_eCtrMax = 120; // Every 2 seconds, an enemy will spawn at a random Y.
}

int Engine::Run()
{
	if (m_isRunning)
	{
		return 1; // 1 arbitrarily means that engine is already running.
	}
	if (Init("GAME1017 Framework", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL))
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
	cout << "Initializing framework..." << endl;
	srand((unsigned)time(NULL)); // Seeding for rand().
	SDL_Init(SDL_INIT_EVERYTHING);
	m_pWindow = SDL_CreateWindow(title,	xPos, yPos, width, height, flags);
	if (m_pWindow == nullptr)
	{
		cout << "Error during window creation!" << endl;
		return 1;
	}
	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
	if (m_pRenderer == nullptr)
	{
		cout << "Error during renderer creation!" << endl;
		return 1;
	}
	// Initialize SDL sublibraries.
	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
	{
		m_pBGTexture = IMG_LoadTexture(m_pRenderer, "../Assets/img/background.jpg");
		m_pEnemyTexture = IMG_LoadTexture(m_pRenderer, "../Assets/img/enemies.png");
	}
	else return 1; // Image init failed.
	if (Mix_Init(MIX_INIT_MP3) != 0)
	{
		Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048);
		Mix_AllocateChannels(16);
		m_pSlacker = Mix_LoadWAV("../Assets/aud/slacker.wav");
		m_pJump = Mix_LoadWAV("../Assets/aud/jump.wav");
		m_pGuile = Mix_LoadMUS("../Assets/aud/guile.mp3");
	}
	else return 1; // Mixer init failed.
	// Example-specific initialization.
	m_pBackground = new Sprite({0,0,1024,768}, {0.0f,0.0f,1024.0f,768.0f});
	m_pPlayer = new Player({ 400.0f, 400.0f, 50.0f, 50.0f }, 300.0f);
	Mix_VolumeMusic(16); // 0-128.
	Mix_PlayMusic(m_pGuile, -1);
	// Initialize rest of framework.
	m_fps = 1.0 / (double)FPS; // Converts FPS into a fraction of seconds.
	m_pKeystates = SDL_GetKeyboardState(nullptr);
	lastFrameTime = chrono::high_resolution_clock::now();
	m_isRunning = true; // Start your engine.
	return 0;
}

void Engine::HandleEvents()
{
	cout << "Handling events..." << endl;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: // Pressing 'X' icon in SDL window.
			m_isRunning = false; // Tell Run() we're done.
			break;
		case SDL_KEYDOWN:
			// Spawn player bullet.
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
			{
				m_playerBullets.push_back( new PlayerBullet(
					{m_pPlayer->GetDst()->x, m_pPlayer->GetDst()->y, 8.0f, 8.0f },
					500.0f) );
				Mix_PlayChannel(-1, m_pSlacker, 0);
			}
			break;
		}
	}
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

void Engine::Update()
{
	cout << "Updating frame..." << endl;
	// Move player vertically.
	if (KeyDown(SDL_SCANCODE_W)) { m_pPlayer->GetDst()->y -= m_pPlayer->GetSpeed() * deltaTime; }
	else if (KeyDown(SDL_SCANCODE_S)) { m_pPlayer->GetDst()->y += m_pPlayer->GetSpeed() * deltaTime; }
	// Move player horizontally.
	if (KeyDown(SDL_SCANCODE_A)) { m_pPlayer->GetDst()->x -= m_pPlayer->GetSpeed() * deltaTime; }
	else if (KeyDown(SDL_SCANCODE_D)) { m_pPlayer->GetDst()->x += m_pPlayer->GetSpeed() * deltaTime; }
	for (PlayerBullet* p : m_playerBullets)
	{
		p->Update(deltaTime);
	}
	for (Enemy* e : m_enemies)
	{
		e->Update(deltaTime);
	}
	if (m_eCtr++ % m_eCtrMax == 0)
	{
		m_enemies.push_back(new Enemy({0,0,40,57},
			{ 1024.0, (float)(rand() % 711), 40.0f, 57.0f},
			200.0f));
	}
	// Collision between player bullets and enemies.
	for (unsigned b = 0; b < m_playerBullets.size(); b++)
	{
		for (unsigned e = 0; e < m_enemies.size(); e++)
		{
			if (SDL_HasIntersectionF(m_playerBullets[b]->GetDst(), m_enemies[e]->GetDst()))
			{
				Mix_PlayChannel(-1, m_pJump, 0);
				delete m_playerBullets[b];
				m_playerBullets[b] = nullptr;
				m_playerBullets.erase(m_playerBullets.begin() + b);
				delete m_enemies[e];
				m_enemies[e] = nullptr;
				m_enemies.erase(m_enemies.begin() + e);
				goto end; // Or return; since we're at the end of the Update().
			}
		}
	}
	end:; // Semicolon needed here because no statements follow.
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
	cout << "Rendering changes..." << endl;
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer);
	// Render example objects.
	SDL_RenderCopyF(m_pRenderer, m_pBGTexture, m_pBackground->GetSrc(), m_pBackground->GetDst());
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 255, 255);
	SDL_RenderFillRectF(m_pRenderer, m_pPlayer->GetDst());
	for (PlayerBullet* p : m_playerBullets)
	{
		SDL_SetRenderDrawColor(m_pRenderer, 0, 255, 0, 255);
		SDL_RenderFillRectF(m_pRenderer, p->GetDst());
	}
	for (Enemy* e : m_enemies)
	{
		SDL_RenderCopyExF(m_pRenderer, m_pEnemyTexture, e->GetSrc(), e->GetDst(), 
			-90.0, NULL, SDL_FLIP_NONE );
	}
	// Flips the buffer and draw to window.
	SDL_RenderPresent(m_pRenderer); 
}

void Engine::Clean()
{
	cout << "Cleaning up..." << endl;
	delete m_pBackground;
	delete m_pPlayer;
	for (PlayerBullet* p : m_playerBullets)
	{
		delete p;
		p = nullptr;
	}
	m_playerBullets.clear();
	m_playerBullets.shrink_to_fit();
	for (Enemy* e : m_enemies)
	{
		delete e;
		e = nullptr;
	}
	m_enemies.clear();
	m_enemies.shrink_to_fit();
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyTexture(m_pBGTexture);
	SDL_DestroyTexture(m_pEnemyTexture);
	Mix_FreeChunk(m_pSlacker);
	Mix_FreeChunk(m_pJump);
	Mix_FreeMusic(m_pGuile);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}