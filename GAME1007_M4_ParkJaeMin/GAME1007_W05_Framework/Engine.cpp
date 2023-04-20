#include "Engine.h"
#include <iostream>
#include <string>
#include "StateManager.h"
using namespace std;

Engine::Engine():m_pWindow(nullptr), m_pRenderer(nullptr), m_isRunning(false)
{
	// m_pWindow = nullptr; // Same as above.
	m_pShipTexture = nullptr;
	m_pBGTexture = nullptr;
	m_pBulletTexture = nullptr; // Remember this
	m_scrollspeed = 200.0f;
	speed = 200;
	angle = 90.0;
	// You can do any of the above in the initializer list too.
	m_enemyCtr = 0;
	m_enemyMax = 240; // 2 seconds if our frameRate is 60.
	nextCol = nextRow = 0;
}

int Engine::Run()
{
	if (m_isRunning)
	{
		return 1; // 1 arbitrarily means that engine is already running.
	}
	if (Init("GAME1007_M1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL))
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
	srand((unsigned)time(nullptr)); // Seed random sequence. Only once.
	SDL_Init(SDL_INIT_EVERYTHING);
	m_pWindow = SDL_CreateWindow(title,	xPos, yPos, width, height, flags);
	if (m_pWindow == nullptr) // Or NULL is okay too
	{
		cout << "Error during window creation!" << endl;
		return 1;
	}
	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
	if (m_pRenderer == nullptr) // Or NULL is okay too
	{
		cout << "Error during renderer creation!" << endl;
		return 1;
	}
	if (IMG_Init( IMG_INIT_PNG | IMG_INIT_JPG ) == 0)
	{
		cout << SDL_GetError() << endl; // Prints last SDL error msg.
		return 1;
	}
	//Example specific initialization.
	//STMA::ChangeState(new TitleState());
	//Initialize rest of framework


	m_pShipTexture = IMG_LoadTexture(m_pRenderer, "../Assets/img/ship.png");
	m_pBGTexture = IMG_LoadTexture(m_pRenderer, "../Assets/img/background.png");
	m_pEnemyTexture = IMG_LoadTexture(m_pRenderer, "../Assets/img/enemies.png");
	m_pBulletTexture = IMG_LoadTexture(m_pRenderer, "../Assets/img/bullet.png");

	if (Mix_Init(MIX_INIT_MP3) != 0)
	{
		// Configure mixer.
		Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
		Mix_AllocateChannels(16);
		// Load sounds.
		m_laser = Mix_LoadWAV("../Assets/aud/laser.wav");
		m_splode = Mix_LoadWAV("../Assets/aud/explosion.wav");
		m_desperateCrusader = Mix_LoadMUS("../Assets/aud/DesperateCrusader.mp3");
		// m_jump = Mix_LoadWAV("../Assets/aud/jump.mp3"); // LoadWAV takes MP3s and other formats too!
		//|| m_enemylaser == nullptr
		if (m_laser == nullptr || m_splode == nullptr || m_desperateCrusader == nullptr)
			cout << Mix_GetError() << endl;
	}
	else return false; // Mixer init failed.


	Mix_PlayMusic(m_desperateCrusader, -1); // 0-n for # of loops, or -1 for infinite looping.
	Mix_VolumeMusic(16); // 0-128.

	// Load bullet texture.
	// Since the textures are pointers, we can test to see if they're nullptr.
	m_fps = 1.0 / (double)FPS; // Converts FPS into a fraction of seconds.
	m_pKeystates = SDL_GetKeyboardState(nullptr);
	lastFrameTime = chrono::high_resolution_clock::now();
	m_dstShip = {WIDTH/2.0f - 154.0f/2.0f, HEIGHT/2.0f - 221.0f/2.0f , 154.0f, 221.0f}; // x, y, w, h
	m_isRunning = true; // Start your engine.
	m_bg1 = { 0.0f, 0.0f, 1024.0f, 768.0f };
	m_bg2 = { 1024.0f, 0.0f, 1024.0f, 768.0f };
	return 0;
}

void Engine::HandleEvents()
{
	// cout << "Handling events..." << endl;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: // Pressing 'X' icon in SDL window.
			m_isRunning = false; // Tell Run() we're done.
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
			{
				Mix_PlayChannel(-1, m_laser, 0);
				m_bulletVec.push_back( new Bullet( 
					{ m_dstShip.x + m_dstShip.w / 2, m_dstShip.y + m_dstShip.h / 2 },
					400.0) );
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
	STMA::Update();
	
	
	//background scroll
	m_bg1.x -= (m_scrollspeed / 4) * deltaTime;
	m_bg2.x -= (m_scrollspeed / 4) * deltaTime;
	if (m_bg1.x <= -1024.0f)
	{
		m_bg1.x = 0.0f;
		m_bg2.x = 1024.0f;
	}
	
	// cout << "Updating frame..." << endl;
	string tickLabel = "DT: " + to_string(deltaTime);
	// cout << "Bullets: " << m_bulletVec.size() << endl;
	SDL_SetWindowTitle(m_pWindow, tickLabel.c_str()); // c_str just returns the char array (char *)
	// Move ship.
	if (KeyDown(SDL_SCANCODE_W) && m_dstShip.y > -25)
	{
		m_dstShip.y -= speed * deltaTime; // speed per second, not per frame.
	}
	else if (KeyDown(SDL_SCANCODE_S) && m_dstShip.y < 575)
	{
		m_dstShip.y += speed * deltaTime;
	}
	if (KeyDown(SDL_SCANCODE_A) && m_dstShip.x > 35)
	{
		m_dstShip.x -= speed * deltaTime;
	}
	else if (KeyDown(SDL_SCANCODE_D) && m_dstShip.x < 830)
	{
		m_dstShip.x += speed * deltaTime;
	}
	//// Rotate ship.
	//if (KeyDown(SDL_SCANCODE_Q) || KeyDown(SDL_SCANCODE_LEFT))
	//{
	//	angle -= 90 * deltaTime;
	//}
	//else if (KeyDown(SDL_SCANCODE_E) || KeyDown(SDL_SCANCODE_RIGHT))
	//{
	//	angle += 90 * deltaTime;
	//}
	
	
	// Update all bullets.
	for (auto bullet : m_bulletVec) // For all elements of vector.
	{
		bullet->Update(deltaTime); // -> is the dot (.) operator for pointers.
	}
	for (auto bullet : m_enemyBulletVec) 
	{
		bullet->Update(deltaTime); 
		if (bullet->GetRect()->x < -16.0f) // enemy bullet goes off screen
		{
			bullet->m_deleteMe = true;
			/*bullet->GetRect()->x = nextCol;
			bullet->GetRect()->y = nextRow;
			nextCol += 16;
			if (nextCol == 1024)
			{
				nextCol = 0;
				nextRow += 16;
				if (nextRow == 768)
				{
					//print here from the meeting
				}
			}*/
		}
	}
	// Counter-controlled for loop that checks if the current bullet's .x is outside bound
	// If it is, deallocate bullet pointer
	// Erase bullet pointer from bullet vector
	// break from for loop
	// Note: You can change the above for loop to counter-controlled to do it all in one

	// Enemy spawn code.
	if (m_enemyCtr++ % m_enemyMax == 0) // Spawn a new enemy.
	{
		m_enemyVec.push_back(new Enemy({ (float)(700 +(rand() % 400)), (float)(150+ (rand() % 500)) })); // Random!
		cout << "New enemy spawned!" << endl;
	}
	for (auto enemy : m_enemyVec) // For all elements of vector.
	{
		enemy->Update();
	}
	//Collision checks
	//1: Player bullets vs enemies (M3) (kill both bullet and enemy
	for (unsigned i = 0; i < m_bulletVec.size(); i++) 
	{
		for (unsigned j = 0; j < m_enemyVec.size(); j++)
		{
			if (SDL_HasIntersectionF(m_bulletVec[i]->GetRect(), m_enemyVec[j]->GetRect()))
			{
				// There is collision between Bullet and enemy
				Mix_PlayChannel(-1, m_splode, 0);
				delete m_bulletVec[i];
				m_bulletVec.erase(m_bulletVec.begin() + i);

				delete m_enemyVec[j];
				m_enemyVec.erase(m_enemyVec.begin() + j);
				goto end;
				// return;
			}
		}
	}
end:;
	// enemy bullets vs player bullet 
	for (unsigned i = 0; i < m_enemyBulletVec.size(); i++)
	{
		for (unsigned j = 0; j < m_bulletVec.size(); j++)
		{
			if (SDL_HasIntersectionF(m_enemyBulletVec[i]->GetRect(), m_bulletVec[j]->GetRect()))
			{
				// There is collision between Bullet and enemy bullet
				Mix_PlayChannel(-1, m_splode, 0);
				delete m_enemyBulletVec[i];
				m_enemyBulletVec.erase(m_enemyBulletVec.begin() + i);

				delete m_bulletVec[j];
				m_bulletVec.erase(m_bulletVec.begin() + j);
				goto end;
				// return;
			}
		}
	}
	// enemy bullets vs player bullet 

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
	STMA::Render();


	// cout << "Rendering changes..." << endl;
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer);

	// Render the GameObjects.
	SDL_RenderCopy(m_pRenderer, m_pBGTexture, NULL, NULL); // All the pixels!

	// Render all the bullets.
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 255, 255);
	SDL_RenderCopyF(m_pRenderer, m_pBGTexture, NULL, &m_bg1);
	SDL_RenderCopyF(m_pRenderer, m_pBGTexture, NULL, &m_bg2);

	for (auto bullet : m_bulletVec) // For all elements of vector.
	{
		//SDL_RenderFillRectF(m_pRenderer, bullet->GetRect());

		SDL_RenderCopyExF(m_pRenderer, m_pBulletTexture, NULL, bullet->GetRect(), 90, NULL, SDL_FLIP_NONE); // Get the hint?
	}
	

	// Render all the enemies.
	for (auto enemy : m_enemyVec) // For all elements of vector.
	{
		SDL_RenderCopyExF(m_pRenderer, m_pEnemyTexture, enemy->GetSrc(), enemy->GetRect(), -90, NULL, SDL_FLIP_NONE);
	}

	SDL_RenderCopyExF(m_pRenderer, m_pShipTexture, NULL, &m_dstShip,
		angle, NULL, SDL_FLIP_NONE);
	for (auto bullet : m_enemyBulletVec)
	{
		SDL_SetRenderDrawColor(m_pRenderer, bullet->m_col.r, bullet->m_col.g, bullet->m_col.b, 255);
		SDL_RenderFillRectF(m_pRenderer, bullet->GetRect());
	}
	SDL_RenderPresent(m_pRenderer); // Flips the buffers.
}

void Engine::Clean()
{
	cout << "Cleaning up..." << endl;
	for (auto bullet : m_bulletVec)
	{
		delete bullet; // Deallocate bullet element.
		bullet = nullptr; // Resetting our dangling pointer.
	}
	m_bulletVec.clear(); // Clear all elements. size = 0
	m_bulletVec.shrink_to_fit(); // Shrink capacity to size. capacity = 0
	cout << "m_bulletVec size: " << m_bulletVec.size() << endl;
	for (auto enemy : m_enemyVec)
	{
		delete enemy; // Deallocate bullet element.
		enemy = nullptr; // Resetting our dangling pointer.
	}
	m_enemyVec.clear(); // Clear all elements. size = 0
	m_enemyVec.shrink_to_fit(); // Shrink capacity to size. capacity = 0
	cout << "m_enemyVec size: " << m_enemyVec.size() << endl;
	// Lastly the static vector.
	for (auto bullet : m_enemyBulletVec)
	{
		delete bullet; // Deallocate bullet element.
		bullet = nullptr; // Resetting our dangling pointer.
	}
	m_enemyBulletVec.clear(); // Clear all elements. size = 0
	m_enemyBulletVec.shrink_to_fit(); // Shrink capacity to size. capacity = 0
	cout << "m_enemyBulletVec size: " << m_enemyBulletVec.size() << endl;
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyTexture(m_pShipTexture);
	SDL_DestroyTexture(m_pBGTexture);
	SDL_DestroyTexture(m_pEnemyTexture);
	SDL_DestroyTexture(m_pBulletTexture);
	IMG_Quit();
	SDL_Quit();

	STMA::Quit();
}

// This is essential for static properties. Declare them OUTSIDE their class but with class scope.
vector<Bullet*> Engine::m_enemyBulletVec;