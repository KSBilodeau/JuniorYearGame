//
//  gameObject.hpp
//  JuniorYearGame
//
//  Created by Keegan Bilodeau on 5/30/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#ifndef gameObject_hpp
#define gameObject_hpp

#include <memory>
#include <SDL.h>
#include <stdio.h>

#include "gameComponents.hpp"

template<typename ObjectType>
class GameObject
{
public:
    static std::unique_ptr<GameObject<PlayerData>> createPlayer(std::shared_ptr<SDL_Renderer> &renderer, SDL_Event &event, PlayerData* data)
    {
        return std::unique_ptr<GameObject<PlayerData>>(new GameObject<PlayerData>(new PlayerMoveComponent, new PlayerInputComponent, new PlayerRenderComponent, data, renderer, event));
    }
    
    void update(SDL_Event &event)
    {
        mInputComponent->update(event);
    }
    
    void move(float delta = 1.0f)
    {
        mMoveComponent->update(delta);
    }
    
    void render()
    {
        mRenderComponent->update();
    }
    
private:
    template<typename GenType>
    GameObject<GenType>(MoveComponent<GenType>* move, InputComponent<GenType>* input, RenderComponent<GenType>* render, GenType* objectData, std::shared_ptr<SDL_Renderer> &renderer, SDL_Event &event) : mMoveComponent(move), mInputComponent(input), mRenderComponent(render), mObjectData(objectData)
    {
        MoveComponent<GenType>::attach(mObjectData);
        InputComponent<GenType>::attach(mObjectData);
        RenderComponent<GenType>::attach(renderer, mObjectData);
    }
    
    std::shared_ptr<ObjectType> mObjectData;
    std::unique_ptr<MoveComponent<ObjectType>> mMoveComponent;
    std::unique_ptr<InputComponent<ObjectType>> mInputComponent;
    std::unique_ptr<RenderComponent<ObjectType>> mRenderComponent;
};

#endif /* gameObject_hpp */
