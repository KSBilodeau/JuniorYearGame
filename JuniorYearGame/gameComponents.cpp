//
//  gameComponents.cpp
//  JuniorYearGame
//
//  Created by Keegan Bilodeau on 6/3/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#include "gameComponents.hpp"

#include <cmath>

void PlayerRenderComponent::update()
{
    SDL_SetRenderDrawColor(mRenderer.get(), 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(mRenderer.get(), &mData->playerRect);
}

void PlayerInputComponent::update(SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        if (event.key.keysym.scancode == SDL_SCANCODE_W)
            mData->yVelocity += -4;
        if (event.key.keysym.scancode == SDL_SCANCODE_S)
            mData->yVelocity += 4;
        if (event.key.keysym.scancode == SDL_SCANCODE_A)
            mData->xVelocity += -4;
        if (event.key.keysym.scancode == SDL_SCANCODE_D)
            mData->xVelocity += 4;
    }
    
    if (event.type == SDL_KEYUP && event.key.repeat == 0)
    {
        if (event.key.keysym.scancode == SDL_SCANCODE_W)
            mData->yVelocity += 4;
        if (event.key.keysym.scancode == SDL_SCANCODE_S)
            mData->yVelocity += -4;
        if (event.key.keysym.scancode == SDL_SCANCODE_A)
            mData->xVelocity += 4;
        if (event.key.keysym.scancode == SDL_SCANCODE_D)
            mData->xVelocity += -4;
    }
}

void PlayerMoveComponent::update(float delta)
{
    if (mData->xVelocity != 0 || mData->yVelocity != 0)
    {
        float length = sqrt(mData->xVelocity * mData->xVelocity + mData->yVelocity * mData->yVelocity);
        
        mData->playerRect.x += (4 * ((float) mData->xVelocity / length)) * delta;
        mData->playerRect.y += (4 * ((float) mData->yVelocity / length)) * delta;
    }
}
