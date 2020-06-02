//
//  gameObject.cpp
//  JuniorYearGame
//
//  Created by Keegan Bilodeau on 5/30/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#include "gameObject.hpp"

#include <iostream>

void PlayerRenderComponent::update(std::shared_ptr<PlayerData> &objectData, std::shared_ptr<SDL_Renderer> &renderer)
{
    if (objectData->xVelocity != 0 || objectData->yVelocity != 0)
    {
        objectData->mPlayerRect.x += objectData->xVelocity;
        objectData->mPlayerRect.y += objectData->yVelocity;
    }
    
    SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer.get(), &objectData->mPlayerRect);
}

void PlayerInputComponent::update(std::shared_ptr<PlayerData> &objectData, SDL_Event &event)
{
    if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && event.key.repeat == 0)
    {
        if (event.key.keysym.scancode == SDL_SCANCODE_W)
            objectData->yVelocity += event.type == SDL_KEYDOWN ? -4 :  4;
        if (event.key.keysym.scancode == SDL_SCANCODE_S)
            objectData->yVelocity += event.type == SDL_KEYDOWN ?  4 : -4;
        if (event.key.keysym.scancode == SDL_SCANCODE_A)
            objectData->xVelocity += event.type == SDL_KEYDOWN ? -4 :  4;
        if (event.key.keysym.scancode == SDL_SCANCODE_D)
            objectData->xVelocity += event.type == SDL_KEYDOWN ? 4 : -4;
    }
}
