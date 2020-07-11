//
//  player.cpp
//  gameEngine
//
//  Created by Keegan Bilodeau on 1/13/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//
#include <cmath>
#include <iostream>

#include "player.hpp"
#include "main.hpp"

Player::Player()
{
    mTexture = nullptr;
    
    mRect = {0, 0, 0, 0};
    
    for (int y = 0; y < (sizeof(mClipRects) / sizeof(*mClipRects)); y++)
        for (int x = 0; x < (sizeof(mClipRects[y]) / sizeof(*mClipRects[y])); x++)
            mClipRects[y][x] = {0, 0, 0, 0};
    
    xVelocityDirection = false;
    yVelocityDirection = false;
    
    mBlockX = 0;
    mBlockY = 0;
    
    speed = 4;
    
    xVelocity = 0;
    yVelocity = 0;
    
    mHealth = 100;
    maxHealth = 100;
    
    mMana = 4;
    mMaxMana = 4;
    
    manaTimer = 0;
    
    ticks = 0;
    
    discusFlag = false;
}

Player::~Player()
{
    mTexture = nullptr;
    
    mRect = {0, 0, 0, 0};
    
    for (int y = 0; y < (sizeof(mClipRects) / sizeof(*mClipRects)); y++)
        for (int x = 0; x < (sizeof(mClipRects[y]) / sizeof(*mClipRects[y])); x++)
            mClipRects[y][x] = {0, 0, 0, 0};
    
    mBlockX = 0;
    mBlockY = 0;
    
    speed = 0;
    
    xVelocity = 0;
    yVelocity = 0;
    
    mHealth = 0;
    maxHealth = 0;
    
    mMana = 0;
    mMaxMana = 0;
    
    manaTimer = 0;
    
    ticks = 0;
    
    discusFlag = false;
}

void Player::createPlayer(std::shared_ptr<KTexture> texture, int spriteSizeX, int spriteSizeY, SDL_Rect &startingRect)
{
    mTexture = std::move(texture);
    
    for (int y = 0; y < (sizeof(mClipRects) / sizeof(*mClipRects) * spriteSizeY); y += spriteSizeY)
        for (int x = 0; x < (sizeof(mClipRects[y]) / sizeof(*mClipRects[y]) * spriteSizeX); x += spriteSizeX)
            mClipRects[y / spriteSizeY][x / spriteSizeX] = {x, y, spriteSizeX, spriteSizeY};
    
    mRect.x = startingRect.x;
    mRect.y = startingRect.y;
    
    mRect.w = startingRect.w;
    mRect.h = startingRect.h;
}

void Player::update(SDL_Event &event)
{
    // Checks if key is pressed
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        // Forwards keys are W and UP ARROW
        if (event.key.keysym.sym == SDLK_w)
        {
            yVelocity -= 1.0;
            mDirection = 2;
        }
        // Down keys are S and DOWN ARROW
        else if (event.key.keysym.sym == SDLK_s)
        {
            yVelocity += 1.0;
            mDirection = 3;
        }
        // Left keys are A and LEFT ARROW
        else if (event.key.keysym.sym == SDLK_a)
        {
            xVelocity -= 1.0;
            mDirection = 1;
        }
        // Right keys are D and RIGHT ARROW
        else if (event.key.keysym.sym == SDLK_d)
        {
            xVelocity += 1.0;
            mDirection = 0;
        }
    }
    
    // Checks if key has been released
    if (event.type == SDL_KEYUP && event.key.repeat == 0)
    {
        // Forwards keys are W and UP ARROW
        if (event.key.keysym.sym == SDLK_w)
            yVelocity += 1.0;
        // Down keys are S and DOWN ARROW
        else if (event.key.keysym.sym == SDLK_s)
            yVelocity -= 1.0;
        // Left keys are A and LEFT ARROW
        else if (event.key.keysym.sym == SDLK_a)
            xVelocity += 1.0;
        // Right keys are D and RIGHT ARROW
        else if (event.key.keysym.sym == SDLK_d)
            xVelocity -= 1.0;
    }
}

void Player::updateMana()
{
    if (manaTimer % 125 == 0 && mMana < mMaxMana)
        mMana++;
    
    manaTimer++;
}

void Player::render()
{
    // Renders player texture at its location
    mTexture->render(mRect.x, mRect.y, false, &mClipRects[mDirection][mCurrentSprite]);
    
    SDL_Rect rect {10, SCREEN_HEIGHT - 30, mHealth * 2, 25};
    SDL_Rect rect2 {10, SCREEN_HEIGHT - 30, maxHealth * 2, 25};
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, &rect);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
    SDL_RenderDrawRect(gRenderer, &rect2);
    
    rect = {rect.x + 210, rect.y, mMana * 50, 25};
    rect2 = {rect2.x + 210, rect2.y, mMaxMana * 50, 25};
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0xFF, 0xFF);
    SDL_RenderFillRect(gRenderer, &rect);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
    SDL_RenderDrawRect(gRenderer, &rect2);
    
    // Draws players noncollision rectangle
    SDL_Rect noncollisionRect {mRect.x - camera.x, mRect.y - camera.y, mRect.w, mRect.h};
    SDL_RenderDrawRect(gRenderer, &noncollisionRect);
}

void Player::move()
{
    // Used in cases where player is moving diagonally
//    float normalizeX = 0, normalizeY = 0;
//    double len = 0;
    
    if (xVelocity != 0 || yVelocity != 0)
        isMoving = true;
    else
        isMoving = false;
    
    if (xVelocity != 0)
        movingX = true;
    else
        movingX = false;

    if (yVelocity != 0)
        movingY = true;
    else
        movingY = false;
    
    if (isMoving)
    {
        if (movingX)
        {
            if (xVelocity < 0 && mDirection != 1)
                mDirection = 1;
            else if (xVelocity > 0 && mDirection != 0)
                mDirection = 0;
        }
        else if (movingY)
        {
            if (yVelocity < 0 && mDirection != 2)
                mDirection = 2;
            else if (yVelocity > 0 && mDirection != 3)
                mDirection = 3;
        }

//        if (movingX && movingY)
//        {
//            len = std::sqrt((std::pow(xVelocity, 2) + std::pow(yVelocity, 2)));
//
//            normalizeX = xVelocity / len * speed;
//            normalizeY = yVelocity / len * speed;
//        }
        
        // Move player along the x-axis
        mRect.x += xVelocity * speed;
            
        if (gMap.checkCollision(std::make_pair(mBlockX, mBlockY), mRect))
            mRect.x -= xVelocity * speed;
        
        // Move player along the y-axis
        mRect.y += yVelocity * speed;
        
        if (gMap.checkCollision(std::make_pair(mBlockX, mBlockY), mRect))
            mRect.y -= yVelocity * speed;
        
        if (!(mCurrentSprite < 7))
            mCurrentSprite = 0;
        else if (ticks % 5 == 0)
            mCurrentSprite++;
    }
    else
        mCurrentSprite = 0;
    
    mBlockX = std::floor(((double) mRect.x) / ((double) TILE_WIDTH) / ((double) BLOCK_WIDTH));
    mBlockY = std::floor(((double) mRect.y) / ((double) TILE_HEIGHT) / ((double) BLOCK_HEIGHT));
    printf("BlockX: %d :: BlockY: %d\n", mBlockX, mBlockY);
    
    for (int y = mBlockY - 2; y < mBlockY + 2; y++)
    {
        for (int x = mBlockX - 2; x < mBlockX + 2; x++)
        {
            gMap.loadBlock(std::make_pair(x, y));
        }
    }
        
    updateCamera();
    ticks++;
}

bool Player::checkCollision(GameObject *object)
{
    // Collision flag
    bool colliding = false;

    // Copy of colliding object rect
    SDL_Rect cRect = object->getRectangle();
    
    // Adjusted rect for this object
    SDL_Rect pRect = {mRect.x - camera.x, mRect.y - camera.y, mRect.w, mRect.h};
    // Adjusted rect for the colliding object
    SDL_Rect aRect = {cRect.x - camera.x, cRect.y - camera.y, cRect.w, cRect.h + 2};
    
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderDrawRect(gRenderer, &pRect);
    SDL_RenderDrawRect(gRenderer, &aRect);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    
    if ((aRect.x < pRect.x + pRect.w && aRect.x + aRect.w > pRect.x) &&
        (aRect.y < pRect.y + pRect.h && aRect.y + aRect.h > pRect.y))
    {
        colliding = true;
    }

    return colliding;
}

bool Player::handleMouseClick(SDL_Event &event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN && mMana > 0)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        double angle = atan2((mouseY - (((mRect.y + (mRect.h / 2)) - 15) - camera.y)), (mouseX - (((mRect.x + (mRect.w / 2)) - 15) - camera.x))) * (180 / M_PI);
        if (angle < 0.0)
            angle += 360.0;
        
        std::vector<double> pValues;
        pValues.push_back(angle);
        pValues.push_back(4.0);
        pValues.push_back(4.0);
        pValues.push_back((mRect.x + (mRect.w / 2)) - 15);
        pValues.push_back((mRect.y + (mRect.h / 2)) - 15);
        
        KBoomerang boomerang;
        if (discusFlag)
        {
            boomerang.createBoomerang((SDL_Color) {0xFF, 0xD7, 0x00, 0xFF}, pValues, (SDL_Point) {15, 15}, (SDL_Rect) {(mRect.x + (mRect.w / 2)) - 15, (mRect.y + (mRect.h / 2)) - 15, 30, 30});
            boomerang.toggleCircleFlag();
            
            boomboom.push_back(std::make_shared<KBoomerang>(boomerang));
        }
        else if (!discusFlag)
        {
            boomerang.createBoomerang((SDL_Color) {0, 0, 0, 0xFF}, pValues, (SDL_Point) {15, 15}, (SDL_Rect) {(mRect.x + (mRect.w / 2)) - 15, (mRect.y + (mRect.h / 2)) - 15, 30, 30});
            boomerang.toggleCircleFlag();
            
            boomboom.push_back(std::make_shared<KBoomerang>(boomerang));
        }
    }
    
    // Will always return true because this function does not check for anything
    return true;
}

void Player::updateCamera()
{
    camera.x = mRect.x - (camera.w / 2);
    camera.y = mRect.y - (camera.h / 2);
}

void Player::inflictDamage(int damage)
{
    if (mHealth - damage > 0)
        mHealth -= damage;
    else
        mHealth = 0;
}

void Player::restoreHealth(int health)
{
    if (mHealth + health < maxHealth)
        mHealth += health;
    else
        mHealth = maxHealth;
}

void Player::toggleDiscusFlag()
{
    discusFlag = !discusFlag;
}

int &Player::getXPos()
{
    return mRect.x;
}

int &Player::getYPos()
{
    return mRect.y;
}

int &Player::getBlockX()
{
    return mBlockX;
}

int &Player::getBlockY()
{
    return mBlockY;
}

int &Player::getWidth()
{
    return mRect.w;
}

int &Player::getHeight()
{
    return mRect.h;
}

SDL_Rect &Player::getRectangle()
{
    return mRect;
}


