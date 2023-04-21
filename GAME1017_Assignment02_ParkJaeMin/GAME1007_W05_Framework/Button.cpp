#include "Button.h"
#include "Engine.h"
#include "StartButton.h"
#include "PauseButton.h"
#include "EndButton.h"
#include "RenderManager.h"
#include "EventManager.h"

std::array<CButton*, NUM_BUTTONS> CButton::s_buttons;
std::array<SDL_Texture*, NUM_BUTTONS> CButton::s_buttonTextures;
CButton* CButton::s_currentButton = nullptr;
SDL_Texture* CButton::s_currentTexture = nullptr;

void CButton::Init()
{
	// Loads all components into the 2 arrays for textures and the actual buttons.
	s_buttons[START] = new CStartButton;
	s_buttons[PAUSE] = new CPauseButton;
	s_buttons[END] = new CEndButton;
	s_buttonTextures[START] = IMG_LoadTexture(REMA::GetRenderer(), "../Assets/img/Play.png");
	s_buttonTextures[PAUSE] = IMG_LoadTexture(REMA::GetRenderer(), "../Assets/img/Resume.png");
	s_buttonTextures[END] = IMG_LoadTexture(REMA::GetRenderer(), "../Assets/img/Menu.png");

	// Error checker for textures.
	for (SDL_Texture* texture : s_buttonTextures) {
		if (texture == nullptr) {
			std::cout << SDL_GetError() << std::endl;
		}
	}

	// Start button is enabled by default.
	s_currentButton = s_buttons[START];
	s_currentTexture = s_buttonTextures[START];
	std::cout << s_currentButton << std::endl;
	std::cout << s_currentTexture << std::endl;
	SetEnabled(true);
}

void CButton::Update()
{
	// If there is no button to render OR the button in question is not enabled.
	if (!s_currentButton->isEnabled) { return; }
	int mouseX = static_cast<int>(EVMA::GetMousePos().x);
	int mouseY = static_cast<int>(EVMA::GetMousePos().y);


	if (SDL_GetMouseState(&mouseX, &mouseY) == 1) // If left click is pressed
	{
		if (IsMouseOver())
		{
			s_currentButton->isActive = true;
		}
	}

	if (s_currentButton->isActive) // Checks if button is active
	{
		s_currentButton->m_alpha = 127;
		s_currentButton->Execute();
		s_currentButton->isActive = false;
	}
	else
	{
		s_currentButton->m_alpha = 255;
	}
}

void CButton::Render()
{
	// If there is no button to render OR the button in question is not enabled.
	if (!s_currentButton->isEnabled) { return; }

	// If there is a button, render it!
	SDL_Rect src_rect{};
	SDL_Rect dst_rect{};

	int texture_width;
	int texture_height;

	SDL_QueryTexture(s_currentTexture, nullptr, nullptr, &texture_width, &texture_height);

	// Grabbing values from the QueryTexture
	src_rect.w = dst_rect.w = texture_width;
	src_rect.h = dst_rect.h = texture_height;


	dst_rect.x = s_currentButton->m_dst.x;
	dst_rect.y = s_currentButton->m_dst.y;

	s_currentButton->m_dst.w = src_rect.w;
	s_currentButton->m_dst.h = src_rect.h;


	// For when button is pressed
	SDL_SetTextureAlphaMod(s_currentTexture, s_currentButton->m_alpha);
	SDL_RenderCopyEx(REMA::GetRenderer(), s_currentTexture, &src_rect, &dst_rect, 0, nullptr, SDL_FLIP_NONE);

}

void CButton::Exit()
{
}

void CButton::SetPosition(int x, int y)
{
	s_currentButton->m_dst = { x, y, s_currentButton->m_dst.w, s_currentButton->m_dst.h };
}

void CButton::SetEnabled(bool enabled)
{
	s_currentButton->isEnabled = enabled;
}

bool CButton::IsMouseOver() {
	/* Is the mouse position:
	* - Greater than button.x
	* - Less than button.x + button.w
	* - Greater than button.y
	* - Less than button.y + button.h
	*/
	SDL_Point temp_mouse_pos = EVMA::GetMousePos();
	SDL_Rect temp_button_pos = s_currentButton->m_dst;
	if (static_cast<int>(temp_mouse_pos.x) >= temp_button_pos.x &&
		static_cast<int>(temp_mouse_pos.x) <= temp_button_pos.x + temp_button_pos.w &&
		static_cast<int>(temp_mouse_pos.y) >= temp_button_pos.y &&
		static_cast<int>(temp_mouse_pos.y) <= temp_button_pos.y + temp_button_pos.h)
	{
		return true;
	}
	return false;
}

void CButton::ChangeButton(EButtons button)
{
	s_currentButton = s_buttons[button];
	s_currentButton->isActive = false;
	s_currentButton->OnEnter();
}