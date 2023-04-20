#include "GameObject.h"

GameObject::GameObject(const SDL_FRect dst)
    :m_dst(dst){}

SDL_FRect* GameObject::GetDst()
{
    return &m_dst;
}

Sprite::Sprite(const SDL_Rect src, const SDL_FRect dst)
    :GameObject(dst), m_src(src) {}

SDL_Rect* Sprite::GetSrc()
{
    return &m_src;
}
