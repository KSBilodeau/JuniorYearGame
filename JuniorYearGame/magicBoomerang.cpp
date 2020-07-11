//
//  magicBoomerang.cpp
//  gameEngine
//
//  Created by Keegan Bilodeau on 2/21/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#include "magicBoomerang.hpp"
#include "main.hpp"

#include <cmath>
#include <iostream>
#include <chrono>
#include <SDL.h>

KBoomerang::KBoomerang()
{
    mVelAngle = 0;
    mRadius = 0;
    mRotateAngle = 0;
    mRotateSpeed = 0;
    
    mPosX = 0;
    mPosY = 0;
    
    numOfBounces = 0;
    
    mCenter = {0, 0};
    mColor = {0, 0, 0, 0};
    mRect = {0, 0, 0, 0};
    
    deleteFlag = false;
    circleFlag = true;
    timerEnabled = false;
}

void KBoomerang::free()
{
    mVelAngle = 0;
    mRadius = 0;
    mRotateAngle = 0;
    mRotateSpeed = 0;
    
    mPosX = 0;
    mPosY = 0;
    
    numOfBounces = 0;
    
    mCenter = {0, 0};
    mColor = {0, 0, 0, 0};
    mRect = {0, 0, 0, 0};
    
    deleteFlag = true;
    circleFlag = false;
    timerEnabled = false;
    animationFlag = false;
}

void KBoomerang::createBoomerang(SDL_Color color, std::vector<double> pValues, SDL_Point center, SDL_Rect rect, bool isAnimation)
{
    mColor = color;
    mCenter = center;
    mRect = rect;

    // START OF PVALUES SECTION
    // While seemingly unclear, pValues is a vector that contains the various starting values
    // for the projectile creation process.  The order of the variables stored is as follows:
    mVelAngle = pValues[0];
    mRadius = pValues[1];
    mRotateSpeed = pValues[2];
    mPosX = pValues[3];
    mPosY = pValues[4];
    // END OF PVALUES SECTION
    
    timerEnabled = true;
    
    animationFlag = isAnimation;
        
    mTexture = mTexture.generateTextureFromRect(mRect, color);
}

void KBoomerang::update(SDL_Event &event)
{
    if (circleFlag)
        rotateInCircle();
    else
        sendInLine();
}

void KBoomerang::sendInLine()
{
    if (!deleteFlag)
        {
            if (numOfBounces == 2)
            {
                free();
                deleteFlag = true;
                return;
            }
            
            if (gMap.checkCollision(std::make_pair(mRect.x / (48 * BLOCK_WIDTH), mRect.y / (48 * BLOCK_HEIGHT)), mRect, false))
            {
                mRotateAngle = 0;
                mVelAngle = (180 - mVelAngle) * -1;
            }
            else if ((gPlayer.checkCollision(this) && enemyFlag))
            {
                free();
                deleteFlag = true;
                if (enemyFlag)
                    gPlayer.inflictDamage(5);
                
                return;
            }
            else if ((gEnemy.checkCollision(this) && discusFlag)|| (gEnemy.checkCollision(this) && !discusFlag))
            {
                if (discusFlag && gEnemy.getShieldHealth() >= 0)
                {
                    gEnemy.inflictShieldDamage(1);
                    
                    free();
                    deleteFlag = true;
                    return;
                }
                else if (!discusFlag && !gEnemy.getShieldActive() && !enemyFlag)
                {
                    gEnemy.inflictDamage(1);
                    
                    free();
                    deleteFlag = true;
                    return;
                }
                else
                    mVelAngle = (180 - mVelAngle) * -1;
            }
            else if ((((int) mRotateAngle) % 360 == 0 && mRotateAngle != 0))
            {
                mVelAngle = (180 - mVelAngle) * -1;
                mRotateAngle *= -1;
                numOfBounces++;
            }
            
            mPosX +=  mRadius * (std::cos(mVelAngle * (M_PI / 180.0)));
            mPosY +=  mRadius * (std::sin(mVelAngle * (M_PI / 180.0)));
            
            mRect.x = (int) mPosX;
            mRect.y = (int) mPosY;
            
            mRotateAngle += mRotateSpeed;
        }
}

void KBoomerang::rotateInCircle()
{
    if (!deleteFlag)
        {
            if (numOfBounces == 2 && timerEnabled)
            {
                free();
                deleteFlag = true;
                return;
            }
            
            if (gMap.checkCollision(std::make_pair(mRect.x / (48 * BLOCK_WIDTH), mRect.y / (48 * BLOCK_HEIGHT)), mRect, false) && !animationFlag)
            {
                mRotateAngle = 0;
//                mVelAngle = (180 - mVelAngle) * -1;
//                mPosX += mRadius * (std::cos(mVelAngle * (M_PI / 180.0)));
//                mPosY += mRadius * (std::sin(mVelAngle * (M_PI / 180.0)));
            }
            else if ((gPlayer.checkCollision(this) && numOfBounces >= 1) && !animationFlag)
            {
                free();
                deleteFlag = true;
                return;
            }
            else if ((((int) mRotateAngle) % 360 == 0 && mRotateAngle != 0))
            {
//                mVelAngle = (180 - mVelAngle) * -1;
                mVelAngle += (360.0 * (0.2 / (16.0 * M_PI))) / (mRadius / 100.0);
                mRotateAngle *= -1;
                numOfBounces++;
            }
            
//            mPosX = 0 + mRadius * (std::cos((mVelAngle * (M_PI / 180.0))));
//            mPosY = 0 + mRadius * (std::sin((mVelAngle * (M_PI / 180.0))));
            
            mPosX +=  mRadius * (std::cos(mVelAngle * (M_PI / 180.0)));
            mPosY +=  mRadius * (std::sin(mVelAngle * (M_PI / 180.0)));
            
            mRect.x = (int) mPosX;
            mRect.y = (int) mPosY;
            
            mRotateAngle += mRotateSpeed;
            mVelAngle += 360.0 * ((0.2 / (16.0 * M_PI))) / (mRadius / 10.0);
            
        }
}

void KBoomerang::toggleCircleFlag()
{
    circleFlag  = !circleFlag;
}

void KBoomerang::render()
{
    if (!deleteFlag)
    {
        if ((mRect.x - camera.x > -mRect.w && mRect.y - camera.y > -mRect.h) && (mRect.x - camera.x < (750 + mRect.w) && mRect.y - camera.y > -(750 + mRect.h)))
        {
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
            SDL_Rect rect {mRect.x - camera.x, mRect.y - camera.y, mRect.w, mRect.h};
            SDL_RenderFillRect(gRenderer, &rect);
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            
            mTexture.render(mRect.x, mRect.y, false, nullptr, mRotateAngle, &mCenter);
            
            if (discusFlag)
            {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
                SDL_RenderFillRect(gRenderer, &mRect);
            }
        }
    }
}

bool KBoomerang::checkCollision(GameObject *object)
{
    // Collision flag
    bool colliding = false;

    // Copy of colliding object rect
    SDL_Rect cRect;
    if (object->getType() == GameObjectType::player)
        cRect = ((Player*) object)->getRectangle();
    else if (object->getType() == GameObjectType::boomerang)
        cRect = ((KBoomerang*) object)->getRectangle();
    
    // Adjusted rect for this object
    SDL_Rect pRect = {mRect.x - camera.x, mRect.y - camera.y, mRect.w, mRect.h};
    // Adjusted rect for the colliding object
    SDL_Rect aRect = {cRect.x - camera.x, cRect.y - camera.y, cRect.w, cRect.h + 2};
    
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderDrawRect(gRenderer, &pRect);
    SDL_RenderDrawRect(gRenderer, &aRect);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    
    if ((cRect.x < pRect.x + pRect.w && cRect.x + cRect.w > pRect.x) &&
        (cRect.y < pRect.y + pRect.h && cRect.y + cRect.h > pRect.y))
    {
        colliding = true;
    }

    return colliding;
}

bool &KBoomerang::getDelete()
{
    return deleteFlag;
}

void KBoomerang::makeEnemy()
{
    enemyFlag = true;
}

void KBoomerang::disableTimer()
{
    timerEnabled = false;
}
