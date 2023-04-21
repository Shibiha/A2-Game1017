#include "States.h"
#include "StateManager.h"
#include <iostream>

#include "CollisionManager.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "FontManager.h"	
#include "TextureManager.h"

void States::Render()
{
	SDL_RenderPresent(REMA::GetRenderer());
}

TitleStates::TitleStates() {}


void TitleStates::Enter()
{
	std::cout << "Entering TitleStates!" << std::endl;

	Mix_VolumeMusic(50);

	Mix_PlayMusic(Engine::backgroundMusic["titleMusic"], -1);
	CButton::SetPosition(512, 128);
	CButton::SetEnabled(true);

	m_background = IMG_LoadTexture(REMA::GetRenderer(), "../Assets/img/menuBackground.jpg");
}

void TitleStates::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_N))
	{
		std::cout << "Changing to GameStates!" << std::endl;
		STMA::ChangeState(new GameStates()); // Transitioning to new GameStates
	}
}

void TitleStates::Render()
{
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 0, 0, 255, 255);
	SDL_RenderClear(REMA::GetRenderer());

	SDL_RenderCopyF(REMA::GetRenderer(), m_background, NULL, &m_bg1);

}

void TitleStates::Exit()
{
	std::cout << "exiting TitleStates" << std::endl;
	// Make sure to invoke mix_freemusic
}


GameStates::GameStates() = default;

void GameStates::Enter() // Initializing everything
{
	SDL_Renderer* engine_renderer = REMA::GetRenderer(); // Making it easier instead of multiple callbacks to Engine
	std::cout << "Entering GameStates!" << std::endl;

	FOMA::Load("../Assets/font/OpenSans-Medium.ttf", "sans", 24);

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

void GameStates::Update()
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
		std::cout << "Changing to PauseStates" << std::endl;
		Mix_PauseMusic(); // Pauses all sounds, head empty.
		STMA::PushState(new PauseStates()); // Pause time!
	}

	// When X is pressed go to EndStates
	if (Engine::Instance().KeyDown(SDL_SCANCODE_X))
	{
		std::cout << "Changing to EndStates" << std::endl;
		STMA::ChangeState(new EndStates());
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
		STMA::ChangeState(new EndStates);
	}

}

void GameStates::Render()
{
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 0, 0, 255, 255);
	SDL_RenderClear(REMA::GetRenderer());

	SDL_Renderer* engine_renderer = REMA::GetRenderer(); // Making it easier instead of multiple callbacks to Engine
	// Any unique rendering in TitleStates goes here ...
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

void GameStates::Exit()
{
	std::cout << "Exiting GameStates!" << std::endl;


	// Deallocating all sound pointers :)
	for (pair<string, Mix_Chunk*> soundFX : m_sfx)
	{
		Mix_FreeChunk(soundFX.second);
	}
	m_sfx.clear(); // Finally, clearing out the map
	Mix_HaltMusic();
}

void GameStates::Resume()
{
	std::cout << "resuming gameStates" << std::endl;
	Mix_ResumeMusic();
}

PauseStates::PauseStates() {}

void PauseStates::Enter()
{
	std::cout << "Entering PauseStates" << std::endl;
	CButton::SetPosition(615, 500);
}

void PauseStates::Update()
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

void PauseStates::Render()
{
	STMA::GetStates().front()->Render(); // Basically, renders the GameStates as well that has not been booted out yet of the vector
	SDL_SetRenderDrawBlendMode(REMA::GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 255, 0, 0, 128); // Making window transparent
	SDL_Rect pause_rect = { 512, 128, 512, 512 };
	SDL_RenderFillRect(REMA::GetRenderer(), &pause_rect);
}

void PauseStates::Exit()
{
}

EndStates::EndStates() {}

void EndStates::Enter()
{
	CButton::ChangeButton(END);
	CButton::SetPosition(500, 500);
	CButton::SetEnabled(true);
}

void EndStates::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_R))
	{
		STMA::ChangeState(new TitleStates());
	}
}

void EndStates::Render()
{
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 0, 255, 0, 255);
	SDL_RenderClear(REMA::GetRenderer());
}

void EndStates::Exit()
{
	std::cout << "Now exiting ExitStates" << std::endl;
}


