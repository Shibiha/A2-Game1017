#include "Background.h"
#include "TextureManager.h"
#include "RenderManager.h"

#include <iostream>

Background::Background(const SDL_Rect src, const SDL_FRect dst, float speed, std::string path, std::string key) : m_src(src), m_scrollSpeed(speed),
key(key)
{
	TEMA::Load(path.c_str(), key);
	m_dst = dst;
	m_start.x = m_dst.x;
	m_end.x = m_start.x - m_dst.w;
}

void Background::Update()
{
	// Check if journey is over
	if (m_dst.x <= m_end.x) {
		m_dst.x = m_start.x;
	}

	// Scroll the background
	m_dst.x -= m_scrollSpeed * m_playerSpeed;
}

void Background::Render()
{
	SDL_RenderCopyF(REMA::GetRenderer(), TEMA::GetTexture(key), &m_src, &m_dst);
}