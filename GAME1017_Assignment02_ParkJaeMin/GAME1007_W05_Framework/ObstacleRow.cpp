#include "ObstacleRow.h"
#include "RenderManager.h"
#include "TextureManager.h"

ObstacleRow::ObstacleRow() :m_gapCtr(0), m_gapMax(3)
{
	// Create the vector now.
	for (int i = 0; i < 1; i++)
	{
		auto obst = new Obstacle({ 128.0f * i * 10, 384.0f, 128.0f, 128.0f }, false, nullptr);
		obst->SetIsHazard(true);
		m_obstacles.push_back(obst);
	}
	m_obstacles.shrink_to_fit();

	// Load the textures
	TEMA::Load("../Assets/img/obstacles/tree-variations.png", "obstacleSheet");
	
}

void ObstacleRow::Update()
{
	for (auto obstacle : m_obstacles)
	{
		// When the first obstacle goes completely offscreen
		if (obstacle != nullptr) // Error checker!
		{
			if (obstacle->GetPos().x <= 0 - 128.0f)
			{
				delete m_obstacles[0];
				m_obstacles[0] = nullptr;
				m_obstacles.erase(m_obstacles.begin());

				if (m_gapCtr++ % m_gapMax == 0)
				{
					PickRandomObstacle();
					Obstacle* obst;
					switch (curObstacle)
					{
					case GREEN_BUSH:
						obst = new Obstacle({ 0 + 128.0f * 10, 384.0f, 128.0f, 168.0f }, true, new Image({ 0, 160, 129, 151 }, { 128 / 2, -(168 / 2), 129, 151 }, "obstacleSheet"));
						//m_obstacles.push_back(new Obstacle({ m_obstacles.back()->GetPos().x + 128.0f, 384.0f, 128.0f, 168.0f }, true, new Image({ 0, 160, 129, 151 }, { 128 / 2, -(168 / 2), 129, 151 }, "obstacleSheet")));
						obst->SetIsHazard(true);
						m_obstacles.push_back(obst);
						break;
					case YELLOWGREEN_BUSH:
						obst = new Obstacle({ 0 + 128.0f * 10, 384.0f, 128.0f, 168.0f }, true, new Image({ 256, 160, 129, 151 }, { 128 / 2, -(168 / 2), 129, 151 }, "obstacleSheet"));
						//m_obstacles.push_back(new Obstacle({ m_obstacles.back()->GetPos().x + 128.0f, 384.0f, 128.0f, 168.0f }, true, new Image({ 256, 160, 129, 151 }, { 128 / 2, -(168 / 2), 129, 151 }, "obstacleSheet")));
						obst->SetIsHazard(true);
						m_obstacles.push_back(obst);
						break;
					case ORANGE_BUSH:
						obst = new Obstacle({ 0 + 128.0f * 10, 384.0f, 128.0f, 168.0f }, true, new Image({ 129, 160, 129, 151 }, { 128 / 2, -(184 / 2), 129, 151 }, "obstacleSheet"));
						//obst = new Obstacle({m_obstacles.back()->GetPos().x + 128.0f, 384.0f - 128.0f, 128.0f, 168.0f}, true, new Image({129, 160, 129, 151}, {128 / 2, -(184 / 2), 129, 151}, "obstacleSheet")));
						obst->SetIsHazard(true);
						m_obstacles.push_back(obst);
						break;
					}
				}
				else 
				{
					auto obst = new Obstacle({ 0 + 128.0f * 10, 384.0f, 110.0f, 184.0f }, true, new Image({ 129, 160, 129, 151 }, { 128 / 2, -(184 / 2), 129, 151 }, "obstacleSheet"));
					obst->SetIsHazard(false);
					m_obstacles.push_back(obst);
				}

			}
			else
			{ 
				obstacle->Update();
			}
		}
	}
}

void ObstacleRow::Render()
{
	// Blue line under obstacles.
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 0, 0, 255, 255);
	//SDL_RenderDrawLine(REMA::GetRenderer(), 0, 512, 1024, 512);
	// Render the obstacles.
	for (const auto obstacle : m_obstacles)
	{
		if (obstacle != nullptr) // Error checker!
		{
			obstacle->Render();
		}
	}
}

void ObstacleRow::PickRandomObstacle()
{
	curObstacle = static_cast<CurrentObstacle>(rand() % static_cast<int>(CurrentObstacle::NUM_OBSTACLES));
}

