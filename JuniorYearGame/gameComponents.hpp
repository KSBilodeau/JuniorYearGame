//
//  gameComponents.hpp
//  JuniorYearGame
//
//  Created by Keegan Bilodeau on 6/3/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#ifndef gameComponents_hpp
#define gameComponents_hpp

#include <memory>
#include <SDL.h>
#include <stdio.h>

struct PlayerData
{
    SDL_Rect playerRect;
    
    int xVelocity, yVelocity;
};

template<typename DataType>
class RenderComponent
{
public:
    virtual ~RenderComponent() {}
    
    static void attach(std::shared_ptr<SDL_Renderer> &renderer, std::shared_ptr<PlayerData> &data)
    {
        if (!mInit)
            mInit = true;
        else
            return;
        
        mData = data;
        mRenderer = renderer;
    }
    
    virtual void update() = 0;
    
protected:
    static std::shared_ptr<SDL_Renderer> mRenderer;
    
    static std::shared_ptr<DataType> mData;
    
private:
    static bool mInit;
};

template<typename DataType>
bool RenderComponent<DataType>::mInit = false;

template<typename DataType>
std::shared_ptr<SDL_Renderer> RenderComponent<DataType>::mRenderer = nullptr;

template<typename DataType>
std::shared_ptr<DataType> RenderComponent<DataType>::mData = nullptr;

template<typename DataType>
class InputComponent
{
public:
    virtual ~InputComponent() {}
    
    static void attach(std::shared_ptr<PlayerData> &data)
    {
        if (!mInit)
            mInit = true;
        else
            return;
        
        mData = data;
    }
    
    virtual void update(SDL_Event &event) = 0;
    
protected:
    static std::shared_ptr<DataType> mData;
    
private:
    static bool mInit;
};

template <typename DataType>
bool InputComponent<DataType>::mInit = false;

template <typename DataType>
std::shared_ptr<DataType> InputComponent<DataType>::mData = nullptr;

template<typename DataType>
class MoveComponent
{
public:
    virtual ~MoveComponent() {}
    
    static void attach(std::shared_ptr<DataType> &data)
    {
        if (!mInit)
            mInit = true;
        else
            return;
        
        mData = data;
    }
    
    virtual void update(float delta = 1.0f) = 0;
    
protected:
    static std::shared_ptr<DataType> mData;
    
private:
    static bool mInit;
};

template<typename DataType>
std::shared_ptr<DataType> MoveComponent<DataType>::mData = nullptr;

template<typename DataType>
bool MoveComponent<DataType>::mInit = false;

// PLAYER GAME COMPONENTS

class PlayerRenderComponent : public RenderComponent<PlayerData>
{
public:
    virtual ~PlayerRenderComponent() {}
    
    virtual void update() override;
};

class PlayerInputComponent : public InputComponent<PlayerData>
{
public:
    virtual ~PlayerInputComponent() {}
    
    virtual void update(SDL_Event &event) override;
};

class PlayerMoveComponent : public MoveComponent<PlayerData>
{
public:
    virtual ~PlayerMoveComponent() {}
    
    virtual void update(float delta = 1.0f) override;
};

#endif /* gameComponents_hpp */
