#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__
#include "Primitives.h"

constexpr int kScrollSpeed = 2;

class Obstacle // Not a GameObject.
{
public:
    Obstacle(const SDL_FRect dst, bool hasImage = false, const SDL_Rect src = { 0,0,0,0 }, const char* key = "");
    Obstacle(const SDL_FRect dst, bool hasImage = false, Image* image = nullptr);
    ~Obstacle();
    void Update();
    void Render();
    const SDL_FPoint GetPos() { return m_pos; }
    SDL_FRect GetDst();
    bool GetisHazard();
    void SetIsHazard(bool hazard);
private:
    bool m_hasImage;
    Image* m_pImage; // Pointer to visual component.
    SDL_FPoint m_pos;// Position on screen.
    bool m_isHazard;


};

#endif