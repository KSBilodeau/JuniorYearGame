//
//  enemy.hpp
//  gameEngine
//
//  Created by Keegan Bilodeau on 3/15/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#ifndef enemy_hpp
#define enemy_hpp

#include <stdio.h>
#include <vector>
#include "gameObject.hpp"
#include "textureWrapper.hpp"
#include "magicBoomerang.hpp"

class Enemy : GameObject
{
public:
    // Allocate variables
    Enemy();
    
    // Frees variables
    void free() override;
    
    void createEnemy(SDL_Rect rect);
    
    // Renders game object to screen
    void render() override;
    
    // Updates game object for non click relate SDL Events
    // TODO: REMOVE THE SDL_EVENT PART I DON'T USE IT
    void update(SDL_Event &event) override;
    
    // Handle mouse clicks in relation to game object
    bool handleMouseClick(SDL_Event &event) override;
    
    // Check collision with another object
    bool checkCollision(GameObject* object) override;
    
    // Toggles hostility
    void toggleHostility();
    
    // Damage enemy
    void inflictDamage(int damage);
    
    // Damages enemy shield
    void inflictShieldDamage(int damage);
    
    // Toggle shield activation
    void toggleShield();
    
    // Returns game object's dimensions
    SDL_Rect &getRectangle() override;
    
    bool &getShieldActive();
    
    int &getShieldHealth();
    
    GameObjectType getType() override
    {
        return GameObjectType::gameObject;
    }
    
private:
    std::vector<std::shared_ptr<KBoomerang>> boomerangs;
    
    // Texture for enemy
    KTexture mTexture;
    
    // Rectangle for enemy
    SDL_Rect mRect;
    
    // Timer for attacks
    int mTimer;
    
    // Indicates enemy health
    int mHealth;
    
    // Indicates if shield is active
    bool shieldActive;
    
    bool deleteFlag;
    
    // Indicates sheild health
    int shieldHealth;
    
    // Indcates if enemy is hostile
    bool isHostile;
};

#endif /* enemy_hpp */
