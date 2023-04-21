#include "State.h"
#include "StateManager.h"
#include <iostream>

#include "CollisionManager.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "FontManager.h"	
#include "TextureManager.h"

void State::Render()
{
	SDL_RenderPresent(REMA::GetRenderer());
}

TitleState::TitleState() {}


void TitleState::Enter()
{
	std::cout << "Entering TitleState!" << std::endl;

	Mix_VolumeMusic(50);

	Mix_PlayMusic(Engine::backgroundMusic["titleMusic"], -1);
	CButton::SetPosition(512, 128);
	CButton::SetEnabled(true);

	m_background = IMG_LoadTexture(REMA::GetRenderer(), "../Assets/img/menuBackground.png");
}

void TitleState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_N))
	{
		std::cout << "Changing to GameState!" << std::endl;
		STMA::ChangeState(new GameState()); // Transitioning to new GameState
	}
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 0, 0, 255, 255);
	SDL_RenderClear(REMA::GetRenderer());

	SDL_RenderCopyF(REMA::GetRenderer(), m_background, NULL, &m_bg1);

}

void TitleState::Exit()
{
	std::cout << "exiting TitleState" << std::endl;
	// Make sure to invoke mix_freemusic
}


GameState::GameState() = default;

void GameState::Enter() // Initializing everything
{
	SDL_Renderer* engine_renderer = REMA::GetRenderer(); // Making it easier instead of multiple callbacks to Engine
	std::cout << "Entering GameState!" << std::endl;

	FOMA::Load("../Assets/font/Roboto-Medium.ttf", "Roboto", 24);

	// Error checking sounds...... (maybe no errors?)
	for (std::pair<string, Mix_Chunk*> soundFX : m_sfx)
	{
		if (soundFX.second == nullptr)
		{
			cout << Mix_GetError() << endl;
		}
	}

	m_pPlayer = new Player({ 0,0,0,0 }, { 128,576,64,64 });
	m_background = new BackgroundScrolling();

	m_pPlayer->SetX(500.0f);
	m_pPlayer->SetY(500.0f);

	obstacleRow = new ObstacleRow;

	elapsedTime = 0;


	//Mix_VolumeMusic(16);

	//Mix_PlayMusic(Engine::backgroundMusic["playMusic"], -1);
}

void GameState::Update()
{
	elapsedTime += std::round(Engine::Instance().GetDeltaTime() * 100) / 100;
	m_pPlayer->Update();
	obstacleRow->Update();
	m_background->Update();
	if (m_pPlayer->GetVelX() <= 0)
	{
		m_background->playerSpeed = 0;
	}
	else
	{
		m_background->playerSpeed = m_pPlayer->GetVelX();
	}

	// Probably can put this in HandleEvents, will do that later
	if (Engine::Instance().KeyDown(SDL_SCANCODE_P))
	{
		std::cout << "Changing to PauseState" << std::endl;
		Mix_PauseMusic(); // Pauses all sounds, head empty.
		STMA::PushState(new PauseState()); // Pause time!
	}

	// When X is pressed go to EndState
	if (Engine::Instance().KeyDown(SDL_SCANCODE_X))
	{
		std::cout << "Changing to EndState" << std::endl;
		STMA::ChangeState(new EndState());
	}
	SDL_FRect* p = m_pPlayer->GetDst();
	for (int i = 0; i < obstacleRow->GetObstacles().size(); i++)
	{
		SDL_FRect t = obstacleRow->GetObstacles()[i]->GetDst();
		if (COMA::AABBCheck(*p, t)) // Collision check between player rect and platform rect.
		{
			//std::cout << "Collision\n";
			if ((p->y + p->h) - (float)m_pPlayer->GetVelY() >= t.y) // If bottom of player < top of platform in "previous frame"
			{ // Colliding with top side of tile. Or collided from top.
				m_pPlayer->StopY();
				m_pPlayer->SetY(t.y - p->h);
				m_pPlayer->SetGrounded(true);
				//std::cout << "TOP SIDE\n";
			}
			else if (p->y - static_cast<float>(m_pPlayer->GetVelY()) >= t.y + t.h)
			{ // Colliding with bottom side of tile
				m_pPlayer->StopY();
				m_pPlayer->SetY(t.y + t.h);
				//std::cout << "BOTTOM SIDE\n";
			}
			else if ((p->x + p->w) - static_cast<float>(m_pPlayer->GetVelX()) <= t.x)
			{ // Colliding with left side of tile
				m_pPlayer->StopX();
				m_pPlayer->SetX(t.x - p->w);
				//std::cout << "LEFT SIDE\n";
			}
			else if (p->x - static_cast<float>(m_pPlayer->GetVelX()) >= (t.x + t.w))
			{ // Colliding with right side of tile
				m_pPlayer->StopX();
				m_pPlayer->SetX(t.x + t.w);
				//std::cout << "RIGHT SIDE\n";
			}
			if (obstacleRow->GetObstacles()[i]->GetisHazard())
			{
				m_pPlayer->SetIsHit(true);
			}
			// Other checks to come.
		}
		// End collision checks.
	}
	if (m_pPlayer->isHit())
	{
		STMA::ChangeState(new EndState);
	}

}

void GameState::Render()
{
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 0, 0, 255, 255);
	SDL_RenderClear(REMA::GetRenderer());

	SDL_Renderer* engine_renderer = REMA::GetRenderer(); // Making it easier instead of multiple callbacks to Engine
	// Any unique rendering in TitleState goes here ...
	SDL_SetRenderDrawColor(engine_renderer, 0, 0, 0, 255);
	SDL_RenderClear(engine_renderer);

	// Rendering the background
	m_background->Render();

	// Render the text!
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(FOMA::GetFont("sans"), std::to_string(elapsedTime).c_str(), { 255, 255, 255 });

	SDL_Texture* Message = SDL_CreateTextureFromSurface(REMA::GetRenderer(), surfaceMessage);

	SDL_Rect msg_rect = { 0, 0, 100, 100 };

	SDL_RenderCopy(REMA::GetRenderer(), Message, NULL, &msg_rect);


	m_pPlayer->Render();



	SDL_SetRenderDrawColor(REMA::GetRenderer(), 255, 0, 0, 255);
	obstacleRow->Render();
}

void GameState::Exit()
{
	std::cout << "Exiting GameState!" << std::endl;


	// Deallocating all sound pointers :)
	for (pair<string, Mix_Chunk*> soundFX : m_sfx)
	{
		Mix_FreeChunk(soundFX.second);
	}
	m_sfx.clear(); // Finally, clearing out the map
	Mix_HaltMusic();
}

void GameState::Resume()
{
	std::cout << "resuming gameState" << std::endl;
	Mix_ResumeMusic();
}

PauseState::PauseState() {}

void PauseState::Enter()
{
	std::cout << "Entering PauseState" << std::endl;
	CButton::SetPosition(615, 500);
}

void PauseState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_R))
	{
		STMA::PopState();
		CButton::SetEnabled(false);
	}
	else
	{
		CButton::SetEnabled(true);
	}
}

void PauseState::Render()
{
	STMA::GetStates().front()->Render(); // Basically, renders the GameState as well that has not been booted out yet of the vector
	SDL_SetRenderDrawBlendMode(REMA::GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 255, 0, 0, 128); // Making window transparent
	SDL_Rect pause_rect = { 512, 128, 512, 512 };
	SDL_RenderFillRect(REMA::GetRenderer(), &pause_rect);
}

void PauseState::Exit()
{
}

EndState::EndState() {}

void EndState::Enter()
{
	CButton::ChangeButton(END);
	CButton::SetPosition(500, 500);
	CButton::SetEnabled(true);
}

void EndState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_R))
	{
		STMA::ChangeState(new TitleState());
	}
}

void EndState::Render()
{
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 0, 255, 0, 255);
	SDL_RenderClear(REMA::GetRenderer());
}

void EndState::Exit()
{
	std::cout << "Now exiting ExitState" << std::endl;
}


