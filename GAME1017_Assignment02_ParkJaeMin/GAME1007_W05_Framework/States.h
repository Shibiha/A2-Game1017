#pragma once
#ifndef __STATES_H__
#define __STATES_H__

#include <map>
#include <string>
#include <iostream>

#include "Engine.h"
#include "Obstacle.h"
#include "ObstacleRow.h"
#include "StatesManager.h"
#include "Player.h"
#include "BackgroundScrolling.h"

class StatesManager; // Forward Declaration of StatesManager

class States // Abstract Base Class
{ // No objects of States can be created
public:
	virtual void Enter() = 0; // Pure virtual function. No definition
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
	virtual void Resume() {}; // Considered defined for all subclasses
protected: // Private but inherited
	States() = default;
};

class TitleStates : public States
{
public:
	TitleStates();
	virtual void Enter(); // Pure virtual function. No definition
	virtual void Update();
	virtual void Render();
	virtual void Exit();
private:
	// Map for music track goes here
	SDL_Texture* m_background;
	SDL_FRect m_bg1 = { 0.0f, 0.0f, WIDTH, HEIGHT };
};

class GameStates : public States
{
public:
	GameStates();
	virtual void Enter(); // Pure virtual function. No definition
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void Resume();

private:
	map<string, Mix_Chunk*> m_sfx; // Holding player shoot, enemy shoot, and collider sounds
	PlatformPlayer* m_pPlayer;
	ScrollingBackground* m_background;
	SDL_FRect* m_pPlatform;
	ObstacleRow* obstacleRow;
	float elapsedTime;
};

// Pause States
class PauseStates : public States
{
public:
	PauseStates();
	virtual void Enter(); // Pure virtual function. No definition
	virtual void Update();
	virtual void Render();
	virtual void Exit();

private:
};
// Make definition for end States
class EndStates : public States
{
public:
	EndStates();
	virtual void Enter(); // Pure virtual function. No definition
	virtual void Update();
	virtual void Render();
	virtual void Exit();

private:

};


// This is essential for static properties. Declare them OUTSIDE their class but with class scope.

#endif // !__STATES_H__