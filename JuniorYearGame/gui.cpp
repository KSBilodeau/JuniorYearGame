//
//  gui.cpp
//  gameEngine
//
//  Created by Keegan Bilodeau on 12/30/19.
//  Copyright © 2019 Keegan Bilodeau. All rights reserved.
//

#include "gui.hpp"

GUI::GUI()
{
    mButtons.clear();
    
    mRect = SDL_Rect{0, 0, 0, 0};
}

void GUI::free()
{
    mButtons.clear();
    
    mRect = SDL_Rect{0, 0, 0, 0};
}

void GUI::createGUI(SDL_Rect rect, std::vector<MainMenuButton> *buttons)
{
    mRect = rect;
    
    mButtons = *buttons;
}

void GUI::render()
{
    for (MainMenuButton button : mButtons)
        button.render();
}

bool GUI::handleMouseClick(SDL_Event &event)
{
    return true;
}

SDL_Rect &GUI::getRectangle()
{
    return mRect;
}
