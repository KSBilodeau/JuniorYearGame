//
//  enemy.cpp
//  gameEngine
//
//  Created by Keegan Bilodeau on 3/15/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#include "enemy.hpp"
#include "magicBoomerang.hpp"
#include "main.hpp"

Enemy::Enemy()
{
    mRect = {0, 0, 0, 0};
    
    isHostile = false;
    
    shieldHealth = 0;
    shieldActive = false;
    
    mHealth = 0;
    mTimer = 0;
}

void Enemy::free()
{
    mRect = {0, 0, 0, 0};
    
    isHostile = false;
    
    shieldHealth = 0;
    shieldActive = false;
    
    mHealth = 0;
    mTimer = 0;
}

void Enemy::createEnemy(SDL_Rect rect)
{
    mRect = rect;
    
    mTexture = mTexture.generateTextureFromRect(mRect, (SDL_Color) {0xFF, 0, 0, 0xFF});
    
    mHealth = 4;
    shieldHealth = 4;
    
    shieldActive = true;
    
    std::vector<double> pValues;
    pValues.push_back(20.0);
    pValues.push_back(5.0);
    pValues.push_back(4.0);
    pValues.push_back(mRect.x + (mRect.w / 2));
    pValues.push_back(mRect.y + (mRect.h / 2));
    
    SDL_Rect rect3 {mRect.x + (mRect.w / 2), mRect.y + (mRect.h / 2), 10, 10};
    
//    for (int i = 0; i <= 360; i += 360 / shieldHealth)
//    {
//        pValues[0] = i;
//        KBoomerang boomerang;
//        boomerang.createBoomerang((SDL_Color) {0x00, 0x00, 0xFF, 0xFF}, pValues, (SDL_Point) {(rect3.w / 2), (rect3.h  / 2)}, rect3, true);
//
//        boomerang.disableTimer();
//        boomerang.makeEnemy();
//
//        std::shared_ptr<KBoomerang> sBoomerang =  std::make_shared<KBoomerang>(boomerang);
//
//        boomerangs.push_back(sBoomerang);
//        boomboom.push_back(sBoomerang);
//    }
    
    
}

void Enemy::render()
{
    if (!deleteFlag)
        mTexture.render(mRect.x, mRect.y, false);
}

void Enemy::update(SDL_Event &event)
{
    if (mTimer % 100 == 0 && mTimer != 0 && !deleteFlag)
    {
        double angle = atan2(((gPlayer.getYPos() + 39) - (((mRect.y + (mRect.h / 2))))), ((gPlayer.getXPos() + 30) - (((mRect.x + (mRect.w / 2)))))) * (180 / M_PI);
        if (angle < 0.0)
            angle += 360.0;
        
        std::vector<double> pValues;
        pValues.push_back(angle);
        pValues.push_back(5.0);
        pValues.push_back(4.0);
        pValues.push_back(mRect.x + (mRect.w / 2));
        pValues.push_back(mRect.y + (mRect.h / 2));
        
        SDL_Rect rect3 {mRect.x + (mRect.w / 2), mRect.y + (mRect.h / 2), 10, 10};
        
        KBoomerang boomerang;
        boomerang.createBoomerang((SDL_Color) {0xFF, 0xD7, 0, 0xFF}, pValues, (SDL_Point) {(rect3.w / 2), (rect3.h  / 2)}, rect3);
        boomerang.toggleCircleFlag();
        boomerang.makeEnemy();

        boomboom.push_back(std::make_shared<KBoomerang>(boomerang));
    }
    
    mTimer++;
}

bool Enemy::handleMouseClick(SDL_Event &event)
{
    return false;
}

bool Enemy::checkCollision(GameObject *object)
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

void Enemy::inflictDamage(int damage)
{
    if (mHealth > 0)
        mHealth -= damage;
    else
    {
        free();
        deleteFlag = true;
        return;
    }
}

void Enemy::inflictShieldDamage(int damage)
{
    if (shieldHealth > 0)
        shieldHealth -= damage;
    else
        toggleShield();
}

void Enemy::toggleShield()
{
    shieldActive = false;
    
    for (std::shared_ptr<KBoomerang> boomerang : boomerangs)
    {
        boomerang->free();
        boomerang->getDelete() = true;
    }
}

int &Enemy::getShieldHealth()
{
    return shieldHealth;
}

bool &Enemy::getShieldActive()
{
    return shieldActive;
}

void Enemy::toggleHostility()
{
    isHostile = !isHostile;
}

SDL_Rect &Enemy::getRectangle()
{
    return mRect;
}
