//
//  gameObject.hpp
//  gameEngine
//
//  Created by Keegan Bilodeau on 12/31/19.
//  Copyright © 2019 Keegan Bilodeau. All rights reserved.
//

#ifndef gameObject_hpp
#define gameObject_hpp

#include <stdio.h>
#include <SDL.h>

enum GameObjectType
{
    gameObject,
    player,
    enemy,
    boomerang,
    enemyboomerang,
    discus,
};

class GameObject
{
public:
    // Virtual deconstructor
    virtual ~GameObject() {};
    
    // Frees variables
    virtual void free() = 0;
    
    // Renders game object to screen
    virtual void render() = 0;
    
    // Updates game object for non click relate SDL Events
    // TODO: REMOVE THE SDL_EVENT PART I DON'T USE IT
    virtual void update(SDL_Event &event) = 0;
    
    // Handle mouse clicks in relation to game object
    virtual bool handleMouseClick(SDL_Event &event) = 0;
    
    // Check collision with another object
    virtual bool checkCollision(GameObject* object) = 0;
    
    // Returns game object's dimensions
    virtual SDL_Rect &getRectangle() = 0;
    
    virtual GameObjectType getType()
    {
        return GameObjectType::gameObject;
    }
};

#endif /* gameObject_hpp */
