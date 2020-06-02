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

struct B
{
    int someData;
};

template<typename Type>
class A
{
public:
    template<typename T>
    A<T>(T* MEM) : mem(MEM) {}
    
    static A<B>* createA(B* newMem)
    {
        return new A<B>(newMem);
    }
    
    std::shared_ptr<Type> &getData()
    {
        return mem;
    }
    
private:
    std::shared_ptr<Type> mem;
};

template<typename RenderableType>
class RenderComponent;
class PlayerRenderComponent;
template<typename UpdatableType>
class InputComponent;
class PlayerInputComponent;

class ObjectData;
class PlayerData;

template<typename ObjectType>
class GameObject
{
public:
    template<typename GenType>
    GameObject<GenType>(GameObject<GenType>* moveObject) : mObjectData(std::move(moveObject->mObjectData)), mInputComponent(std::move(moveObject->mInputComponent)), mRenderComponent(std::move(moveObject->mRenderComponent))
    {
        delete moveObject;
        moveObject = nullptr;
    }
    
    static GameObject<PlayerData> createPlayer(PlayerInputComponent* iComponent, PlayerRenderComponent* rComponent, PlayerData* objectData)
    {
        return new GameObject(iComponent, rComponent, objectData);
    }
    
    void update(SDL_Event &event)
    {
        mInputComponent->update(mObjectData, event);
    }
    
    void render(std::shared_ptr<SDL_Renderer> &renderer)
    {
        mRenderComponent->update(mObjectData, renderer);
    }
    
private:
    template<typename GenType>
    GameObject<GenType>(InputComponent<GenType>* input, RenderComponent<GenType>* render, GenType* objectData) : mInputComponent(input), mRenderComponent(render), mObjectData(objectData)
    {
        
    }
    
    std::shared_ptr<ObjectType> mObjectData;
    std::unique_ptr<InputComponent<ObjectType>> mInputComponent;
    std::unique_ptr<RenderComponent<ObjectType>> mRenderComponent;
};

template<typename RenderableType>
class RenderComponent
{
public:
    virtual ~RenderComponent() {}
    
    virtual void update(std::shared_ptr<RenderableType> &objectData, std::shared_ptr<SDL_Renderer> &renderer) = 0;
};

class PlayerRenderComponent : public RenderComponent<PlayerData>
{
public:
    virtual ~PlayerRenderComponent() {}
    
    virtual void update(std::shared_ptr<PlayerData> &objectData, std::shared_ptr<SDL_Renderer> &renderer) override;
};

template<typename UpdatableType>
class InputComponent
{
public:
    virtual ~InputComponent() {}
    
    virtual void update(std::shared_ptr<UpdatableType> &objectData, SDL_Event &event) = 0;
};

class PlayerInputComponent : public InputComponent<PlayerData>
{
public:
    virtual ~PlayerInputComponent() override
    {
        
    }
    
    virtual void update(std::shared_ptr<PlayerData> &objectData, SDL_Event &event) override;
};

class ObjectData
{
public:
    virtual ~ObjectData() {};
    
    virtual int getID() = 0;
};

class PlayerData : ObjectData
{
public:
    PlayerData(SDL_Rect startingRect, int xVel = 0, int yVel = 0) : mPlayerRect(startingRect), xVelocity(xVel), yVelocity(yVel)
    {
        
    }
    
    virtual ~PlayerData() override
    {
        
    }
    
    virtual int getID() override
    {
        return 0;
    }
    
    SDL_Rect mPlayerRect;
    
    int xVelocity, yVelocity;
};

#endif /* gameObject_hpp */
