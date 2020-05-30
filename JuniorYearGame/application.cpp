//
//  application.cpp
//  JuniorYearGame
//
//  Created by Keegan Bilodeau on 5/29/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#include "application.hpp"

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdexcept>

Application::Application() : mWindow(nullptr, [](SDL_Window* window){SDL_DestroyWindow(window);}), mRenderer(nullptr, [](SDL_Renderer* renderer){SDL_DestroyRenderer(renderer);}), isRunning(true)
{
    run();
}

Application::~Application()
{
    cleanup();
}

void Application::cleanup()
{
    // Shutdown the SDL subsystems
    SDL_Quit();
    
    // Shutdown the SDL_image subsystems
    IMG_Quit();
    
    // Shutdown the SDL_ttf subsystems
    TTF_Quit();
}

bool Application::init()
{
    // Init success flag
    bool success = true;
    
    // Initialize SDL subsystems
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Creates SDL window as a unique pointer with a custom deleter
        mWindow = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>(SDL_CreateWindow("My Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN), [](SDL_Window* window) {SDL_DestroyWindow(window);});
        if (mWindow == nullptr)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Creates SDL renderer as a unique pointer with a custom deleter
            mRenderer = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>(SDL_CreateRenderer(mWindow.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), [](SDL_Renderer* renderer){SDL_DestroyRenderer(renderer);});
            if (mRenderer == nullptr)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                // Set the renderer's draw color
                SDL_SetRenderDrawColor(mRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
                
                // Init the SDL_image subsystems
                int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not intitialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
                
                // Init the SDL_ttf subsystems
                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }
    
    return success;
}

bool Application::loadMedia()
{
    // Loading success flag
    bool success = true;
    
    return success;
}


void Application::run()
{
    // Attempt to init the application
    if (!init())
        throw std::runtime_error("Critical Error: Failed to intialize the application!");
    
    // Attempt to load the application's resources
    if (!loadMedia())
        throw std::runtime_error("Critical Error: Failed to load assets!");
    
    // Variable for interfacing with SDL's event system
    SDL_Event event;
    
    // Main game loop
    while (isRunning)
    {
        // Processes the most recent input
        while (SDL_PollEvent(&event))
        {
            // Shuts the program down when after receiving the quit signal from user
            if (event.type == SDL_QUIT)
                isRunning = false;
            else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                isRunning = false;
        }
        
        // Set the right draw color and clear the screen
        SDL_SetRenderDrawColor(mRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(mRenderer.get());
        
        // Renders the current buffer to the screen and then clears the buffer
        SDL_RenderPresent(mRenderer.get());
        SDL_RenderClear(mRenderer.get());
    }
}
