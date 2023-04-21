#include "Obstacle.h"
#include "RenderManager.h"

Obstacle::Obstacle(const SDL_FRect dst, bool hasImage, const SDL_Rect src, const char* key)
    :m_hasImage(hasImage), m_pImage(nullptr)
{
    m_pos = { dst.x, dst.y };
}

Obstacle::Obstacle(const SDL_FRect dst, bool hasImage, Image* image) : m_hasImage(hasImage), m_pImage(image)
{
    m_pos = { dst.x, dst.y };

}

Obstacle::~Obstacle()
{
    if (m_hasImage)
    {
        delete m_pImage;
        m_pImage = nullptr;
    }
}

void Obstacle::Update()
{

    if (m_hasImage)
    {
        m_pos.x -= kScrollSpeed;
        // Update the image dst rect.
        m_pImage->GetDst()->x = m_pos.x;
        m_pImage->GetDst()->y = m_pos.y - m_pImage->GetSrc()->h / 5.0f;

        m_pImage->Update();
    }
    else {
        m_pos.x -= kScrollSpeed;
    }
}

void Obstacle::Render()
{
    SDL_FRect m_dst = { m_pos.x, m_pos.y, 128, 128 };
    if (m_hasImage)
    {
        // Render image
        m_pImage->Render();
    }

    // SDL_SetRenderDrawColor(REMA::GetRenderer(), 128, 0, 128, 255);
     //SDL_RenderDrawRectF(REMA::GetRenderer(), &m_dst);
}

SDL_FRect Obstacle::GetDst()
{
    SDL_FRect temp = { m_pos.x, m_pos.y,128,128 };
    return temp;
}

bool Obstacle::GetisHazard()
{
    return m_isHazard;
}

void Obstacle::SetIsHazard(bool hazard)
{
    m_isHazard = hazard;
}
