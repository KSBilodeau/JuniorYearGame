//
//  guiButtons.cpp
//  gameEngine
//
//  Created by Keegan Bilodeau on 12/31/19.
//  Copyright © 2019 Keegan Bilodeau. All rights reserved.
//

#include "guiButtons.hpp"
#include "main.hpp"

// THIS IS THE START OF THE GUIBUTTON INTERFACE METHOD DECLARATION AREA

GUIButton::~GUIButton()
{
    
}

// THIS IS THE END OF THE GUIBUTTON INTERFACE METHOD DECLARATION AREA



// THIS IS THE START OF THE TEXT BOX METHOD DECLARATION AREA

// THIS IS THE END OF THE TEXT BOX METHOD DECLARATION AREA



// THIS IS THE START OF THE MAIN MENU BUTTON METHOD DECLARATION AREA

MainMenuButton::MainMenuButton()
{
    mBackgroundTexture = nullptr;
    mIcon = nullptr;
    
    isSelected = false;
    
    mRectangle = {0, 0, 0, 0};
    
    mClipRects[0] = {0, 0, 0, 0};
    mClipRects[1] = {0, 0, 0, 0};
}

void MainMenuButton::createMainMenuButton(std::shared_ptr<KTexture> backgroundTexture, std::shared_ptr<KTexture> icon, int x, int y, SDL_Rect rects[2])
{
    mBackgroundTexture = std::move(backgroundTexture);
    mIcon = std::move(icon);
    
    mClipRects[0] = rects[0];
    mClipRects[1] = rects[1];
    
    mRectangle.x = x;
    mRectangle.y = y;
    
    mRectangle.w = mClipRects[0].w;
    mRectangle.h = mClipRects[0].h;
}

void MainMenuButton::render()
{
    if (mBackgroundTexture != nullptr && mIcon != nullptr)
    {
        if (isSelected)
        {
            mBackgroundTexture->render(mRectangle.x, mRectangle.y + 2, true, &mClipRects[isSelected]);
            mIcon->render(mRectangle.x, mRectangle.y + 2, true);
        }
        else
        {
            mBackgroundTexture->render(mRectangle.x, mRectangle.y, true, &mClipRects[isSelected]);
            mIcon->render(mRectangle.x, mRectangle.y, true);
        }
    }
    
    SDL_RenderDrawRect(gRenderer, &mRectangle);
}

void MainMenuButton::update(SDL_Event &event)
{
    
}

void MainMenuButton::activate()
{
    isSelected = !isSelected;
}

void MainMenuButton::free()
{
    
}

bool MainMenuButton::handleMouseClick(SDL_Event &event)
{
    // Click success flag
    bool success = false;
    
    int x, y;
    SDL_GetMouseState(&x, &y);
    
    if ((x < mRectangle.x + mRectangle.w && x + 1 > mRectangle.x) &&
        (y < mRectangle.y + mRectangle.h && y + 1 > mRectangle.y))
    {
//        if (lambdaActivate.lambda != nullptr)
//            lambdaActivate(isSelected);
//        else
//            activate();
        success = true;
    }
    
    return success;
}

void MainMenuButton::toggleSelected()
{
    isSelected = !isSelected;
}

SDL_Rect &MainMenuButton::getRectangle()
{
    return mRectangle;
}

bool &MainMenuButton::getButtonState()
{
    return isSelected;
}

// THIS IS THE END OF THE MAIN MENU BUTTON METHOD DECLARATION AREA
