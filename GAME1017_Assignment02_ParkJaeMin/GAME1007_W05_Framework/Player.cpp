#include "Player.h"
#include "Engine.h"
#include "EventManager.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include <cmath>
#include <iostream>

#include "StateManager.h"
using namespace std;

const std::string plrSpriteFile = "../Assets/img/Player.png";
const std::string plrTextFile = "../Assets/dat/Player.txt";
const std::string plrKey = "player";

Player::Player(SDL_Rect src, SDL_FRect dst) :AnimatedSprite(src, dst, STATE_IDLING),
m_grounded(false), m_facingLeft(false), m_maxVelX(4.0),
m_maxVelY(kJumpForce), m_grav(kGrav), m_drag(0.8), m_isHit(false)
{
	TEMA::LoadSpriteMap(plrTextFile.c_str(), plrSpriteFile.c_str(), plrKey);
	SetSpriteSheet(TEMA::GetSpriteSheet(plrKey));

	//TEMA::Load("../Assets/img/player.png", "player");
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	SetAnimation(m_state, "idle"); // Initialize idle animation.

	camera = m_dst;
	m_isCameraDisabled = true;
}

void Player::Update()
{
	// Updating Camera
	if (!m_isCameraDisabled)
	{
		camera = m_dst;
		camera.x -= WIDTH / 2;
		camera.y = 0;

		m_dst = { m_dst.x - camera.x, m_dst.y, m_dst.w, m_dst.h };
	}
	else {
		camera = { 0,0,0,0 };
	}

	// Checking states.
	switch (m_state)
	{
	case STATE_IDLING:
		// Transition to jump.
		if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_grounded)
		{
			m_accelY = -kJumpForce; // SetAccelY(-JUMPFORCE);
			m_grounded = false; // SetGrounded(false);
			//SOMA::PlaySound("jump");
			SetAnimation(STATE_JUMPING, "jump");
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D) || EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			SetAnimation(STATE_RUNNING, "run");
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_S))
		{
			SetAnimation(STATE_DUCKING, "duck");
		}
		break;
	case STATE_JUMPING:
		// Move left and right
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			m_accelX = -1.5;
			m_facingLeft = true;
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			m_accelX = 1.5;
			m_facingLeft = false;
		}

		// Hit the ground, transition to run.
		if (m_grounded)
		{
			SetAnimation(STATE_IDLING, "idle");
		}
		break;
	case STATE_RUNNING:
		// Move left and right
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			m_accelX = -1.5;
			m_facingLeft = true;
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			m_accelX = 1.5;
			m_facingLeft = false;
		}
		// Transition to jump.
		if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_grounded)
		{
			m_accelY = -kJumpForce; // SetAccelY(-JUMPFORCE);
			m_grounded = false; // SetGrounded(false);
			//SOMA::PlaySound("jump");
			SetAnimation(STATE_JUMPING, "jump");
		}
		else if (EVMA::KeyReleased(SDL_SCANCODE_A) || EVMA::KeyReleased(SDL_SCANCODE_D))
		{
			SetAnimation(STATE_IDLING, "idle");
		}
		break;
	case STATE_DUCKING:
		// Move left and right
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			m_accelX = -1.5;
			m_facingLeft = true;
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			m_accelX = 1.5;
			m_facingLeft = false;
		}
		if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_grounded)
		{
			m_accelY = -kJumpForce; // SetAccelY(-JUMPFORCE);
			m_grounded = false; // SetGrounded(false);
			//SOMA::PlaySound("jump");
			SetAnimation(STATE_JUMPING, "jump");
		}
		if (EVMA::KeyReleased(SDL_SCANCODE_S)) {
			SetAnimation(STATE_IDLING, "idle");
		}
		break;
	}
	// Player movement. X axis first.
	m_velX += m_accelX; // Acceleration
	m_velX *= (m_grounded ? m_drag : 1.0);
	m_velX = std::min(std::max(m_velX, -m_maxVelX), m_maxVelX);
	m_dst.x += static_cast<float>(m_velX);
	// Y axis now.
	m_velY += m_accelY + m_grav;
	m_velY = std::min(std::max(m_velY, -m_maxVelY), m_maxVelY);
	m_dst.y += (float)m_velY;

	// Wrapping on screen
	if (m_dst.x < -m_dst.w) {
		m_dst.x = WIDTH;
	}
	else if (m_dst.x > WIDTH) {
		m_dst.x = -m_dst.w;
	}
	if (m_dst.y >= 448.0f)
	{
		m_dst.y = 448.0f;
		m_grounded = true;
		m_velY = 0.0f;
	}

	m_accelX = m_accelY = 0.0; // Resetting accel every frame.
	if (m_isHit)
	{
		m_isHit = false;
		m_velX = 0.0;
		m_velY = 0.0;
		SetAnimation(STATE_DEATH, "jump");
	}
	// Invoke the animation.
	Animate();
}

void Player::Render()
{
	if (m_facingLeft) {
		SDL_RenderCopyExF(REMA::GetRenderer(), TEMA::GetTexture(plrKey),
			GetSrc(), GetDst(), 0, NULL, SDL_FLIP_HORIZONTAL);
	}
	else {
		SDL_RenderCopyExF(REMA::GetRenderer(), TEMA::GetTexture(plrKey),
			GetSrc(), GetDst(), 0, NULL, SDL_FLIP_NONE);
	}

}

void Player::Stop()
{
	StopX();
	StopY();
}

void Player::StopX() { m_velX = 0.0; }

void Player::StopY() { m_velY = 0.0; }

void Player::SetAccelX(double accel) { m_accelX = accel; }

void Player::SetAccelY(double accel) { m_accelY = accel; }

bool Player::IsGrounded() { return m_grounded; }

void Player::SetGrounded(bool grounded) { m_grounded = grounded; }

bool Player::isHit()
{
	return m_isHit;
}

void Player::SetIsHit(bool hit)
{
	m_isHit = hit;
}

double Player::GetVelX() { return m_velX; }

double Player::GetVelY() { return m_velY; }

void Player::SetX(float x) { m_dst.x = x; }

void Player::SetY(float y) { m_dst.y = y; }

double Player::GetAccelY()
{
	return m_accelY;
}
