//
//  main.cpp
//  gameEngine
//
//  Created by Keegan Bilodeau on 12/30/19.
//  Copyright © 2019 Keegan Bilodeau. All rights reserved.
//
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <cmath>
#include <chrono>
#include <ctime>
#include <sstream>

#include "main.hpp"
#include "event.hpp"
#include "magicBoomerang.hpp"
#include "textboxProcessor.hpp"
#include "gameEventHandler.hpp"
#include "sprite.hpp"
#include "textureWrapper.hpp"
#include "textureHandler.hpp"
#include "textbox.hpp"
#include "guiButtons.hpp"
#include "clock.hpp"
#include "block.hpp"
#include "keyHandler.hpp"
#include "enemy.hpp"

SDL_Window* gWindow = nullptr;

SDL_Renderer* gRenderer = nullptr;

TTF_Font* gFont = nullptr;

Mix_Music* gIntroMusic = nullptr;

KeyHandler keyHanlder;

GameEventHandler eventHandler;
TextboxEventProcessor textboxProcessor;
TileCollisionProcessor tileProcessor;

Player gPlayer;

Enemy gEnemy;

KMap gMap;

TextureRegister textureRegistry;

std::vector<KTilesheet> tileSheets;
KTilesheet tiles;
KTilesheet tiles2;

SDL_Rect camera;

std::pair<int, int> selectedTileID {0, 0};

std::vector<std::shared_ptr<KBoomerang>> boomboom;
std::vector<Mix_Music*> music;

int selectedMusicID = 0;
bool setTilesCollidable = false;

bool init()
{
    // Initialization success flag
    bool success = true;
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    // If SDL successfully initialized
    else
    {
        // Creates a visible window
        gWindow = SDL_CreateWindow("My Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == nullptr)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        // If window could be created
        else
        {
            // Create renderer with hardware acceleration and V-Sync enabled
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == nullptr)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            // If renderer could be created
            else
            {
                // Initialize draw color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                
                // Initialize image loading (specifically pngs; however, more can be added)
                int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
                
                // Initialize SDL mixer
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048))
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
                
                // Initalize font loading for future text rendering
                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
                
                // Start the key handler
                keyHanlder.startKeyHandler();
            }
        }
    }
    
    return success;
}

// TODO: Add non-manual way of importing files
bool loadMedia()
{
    // Media import success flag
    bool success = true;
    
    // Load fonts, music, etc...
    gFont = TTF_OpenFont("DejaVuSans.ttf", 20);
    if (gFont == nullptr)
    {
        printf("Game's font could not be loaded! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    
    gIntroMusic = Mix_LoadMUS("Game_Music.wav");
    if (gIntroMusic == nullptr)
    {
        printf("Music could not be loaded! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    
    Mix_Music *indMusic;
    indMusic = Mix_LoadMUS("Game_Music.wav");
    music.push_back(indMusic);
    
    Mix_Music *indMusic2;
    indMusic2 = Mix_LoadMUS("Angel_Music.wav");
    music.push_back(indMusic2);
    
    Mix_Music *indMusic3;
    indMusic3 = Mix_LoadMUS("Azalean_Hymn.wav");
    music.push_back(indMusic3);
    
    Mix_Music *indMusic4;
    indMusic4 = Mix_LoadMUS("Batlle_of_the_Heavens.wav");
    music.push_back(indMusic4);
    
    Mix_Music *indMusic5;
    indMusic5 = Mix_LoadMUS("Sorrow's_Call.wav");
    music.push_back(indMusic5);
    
    // Load images, videos, etc...
    KTexture testTexture;
    if (!testTexture.loadFromFile("background.png", 1))
    {
        printf("Test texture could not be loaded! SDL_image Error: %s\n", IMG_GetError());
        success = false;
    }
    else
    {
        textureRegistry.registerTexture(0, testTexture);
    }
    
    if (!testTexture.loadFromFile("sprites.png", 1))
    {
        printf("Test texture could not be loaded! SDL_image Error: %s\n", IMG_GetError());
        success = false;
    }
    else
    {
        textureRegistry.registerTexture(1, testTexture);
    }
    
    if (!testTexture.loadFromFile("redSheet.png", 1))
    {
        printf("Test texture could not be loaded! SDL_image Error: %s\n", IMG_GetError());
        success = false;
    }
    else
    {
        textureRegistry.registerTexture(2, testTexture);
    }

    if (!testTexture.loadFromFile("musicOn.png", .9))
    {
        printf("Test texture could not be loaded! SDL_image Error: %s\n", IMG_GetError());
        success = false;
    }
    else
    {
        textureRegistry.registerTexture(3, testTexture);
    }
    
    if (!testTexture.loadFromFile("Tiles1-1.png", 3))
    {
        printf("Test texture could not be loaded! SDL_image Error: %s\n", IMG_GetError());
        success = false;
    }
    else
    {
        textureRegistry.registerTexture(4, testTexture);
    }
    
    if (!testTexture.loadFromFile("Player.jpg", 3))
    {
        printf("Player texure could not be loaded! SDL_image Error: %s\n", IMG_GetError());
        success = false;
    }
    else
    {
        textureRegistry.registerTexture(5, testTexture);
    }
    
    if (!testTexture.loadFromFile("TextBox.png", 1.776))
    {
        printf("Textbox texture could not be loaded! SDL_image Error: %s\n", IMG_GetError());
        success = false;
    }
    else
    {
        textureRegistry.registerTexture(6, testTexture);
    }
    
    SDL_Color color {0xFF, 0xFF, 0xFF, 0xFF};
    if (!testTexture.loadFromString("Be wary of those who may bear light / For their souls may not be bright / Do not fight others out of spite / For it feeds the darkness\u0027 might", color))
    {
        printf("Text could not be loaded! SDL_image Error: %s\n", IMG_GetError());
        success = false;
    }
    else
        textureRegistry.registerTexture(7, testTexture);
    
    if (!testTexture.loadFromFile("Tiles2.png", 3))
    {
        printf("Test texture could not be loaded! SDL_image Error: %s\n", IMG_GetError());
        success = false;
    }
    else
    {
        textureRegistry.registerTexture(8, testTexture);
    }
    
    if (!testTexture.loadFromFile("target.png", .9))
    {
        printf("Test texture could not be loaded! SDL_image Error: %s\n", IMG_GetError());
        success = false;
    }
    else
    {
        textureRegistry.registerTexture(9, testTexture);
    }
    
    Textbox textbox;
    textbox.createTextbox(textureRegistry.requestAccess(6), textureRegistry.requestAccess(7), (SCREEN_WIDTH / 2) - (textureRegistry.requestAccess(6)->getWidth() * 1.776 / 2), (SCREEN_HEIGHT / 1.20) - (textureRegistry.requestAccess(6)->getHeight() * 1.776 / 2));
    
    tiles.loadFromTexture(4, 16, 16);
    tiles2.loadFromTexture(8, 16, 16);
    
    tileSheets.push_back(tiles);
    tileSheets.push_back(tiles2);
    
    return success;
}

void close()
{
    // Free all textures that have not already been freed
    textureRegistry.freeAll();
    
    // Destroy font
    TTF_CloseFont(gFont);
    
    // Free music
    Mix_FreeMusic(gIntroMusic);
    Mix_CloseAudio();
    
    // Free renderer
    SDL_DestroyRenderer(gRenderer);
    // Free window
    SDL_DestroyWindow(gWindow);
    
    // Set all pointers back to null
    gWindow = nullptr;
    gRenderer = nullptr;
    gFont = nullptr;
    gIntroMusic = nullptr;
    
    // Close SDL subsystems
    IMG_Quit();
    SDL_Quit();
    Mix_Quit();
}

void handleConsoleCommand()
{
    std::string command;
    std::cout << "$>: ";
    std::getline(std::cin, command, '\n');
    
    if (command.find("SELECT_TILE") != std::string::npos)
    {
        std::istringstream iss(command);
        std::vector<std::string> commands;
        std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(commands));
        
        selectedTileID.first = std::stoi(commands[1]);
        selectedTileID.second = std::stoi(commands[2]);
    }
    else if (command.find("DAMAGE_PLAYER") != std::string::npos)
    {
        std::istringstream iss(command);
        std::vector<std::string> commands;
        std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(commands));
        
        gPlayer.inflictDamage(std::stoi(commands[1]));
    }
    else if (command.find("TOGGLE_SET_COLLIDABLE") != std::string::npos)
    {
        setTilesCollidable = !setTilesCollidable;
    }
}

void activateDiscus()
{
    std::vector<double> pValues;
    pValues.push_back(20.0);
    pValues.push_back(5.0);
    pValues.push_back(4.0);
    pValues.push_back(gPlayer.getXPos() + (gPlayer.getWidth() / 2));
    pValues.push_back(gPlayer.getYPos() + (gPlayer.getHeight() / 2));
    
    SDL_Rect rect3 {gPlayer.getXPos() + (gPlayer.getWidth() / 2), gPlayer.getYPos() + (gPlayer.getHeight() / 2), 10, 10};
    
    for (int i = 0; i <= 360; i += 20)
    {
        pValues[0] = i;
        KBoomerang boomerang;
        boomerang.createBoomerang((SDL_Color) {0xFF, 0xD7, 0, 0xFF}, pValues, (SDL_Point) {(rect3.w / 2), (rect3.h  / 2)}, rect3, true);
        
        boomboom.push_back(std::make_shared<KBoomerang>(boomerang));
    }
    
    gPlayer.toggleDiscusFlag();
}

int main(int argc, const char * argv[])
{
    // Main loop flag
    bool isRunning = true;
    
    // Event handler
    SDL_Event event;
    
    // Fps ticks
    KClock fpsTimer;
    
    // FPS cap timer
    KClock capTimer;
    
    // Games average FPS
    float avgFPS;
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    
    try
    {
        if (!init())
            printf("Failed to initialize!\n");
        else if (!loadMedia())
            printf("Failed to load media!\n");
        
        camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        
        Sprite sprite;
        SDL_Rect rect {0, 0, 100, 100};
        sprite.createSprite(0, rect, true);
        
        SDL_Rect startingRect {0, 0, 20 * SCALE_FACTOR, 26 * SCALE_FACTOR};
        gPlayer.createPlayer(textureRegistry.requestAccess(5), 20, 26, startingRect);
        
        SDL_Rect rect2 {15 * 48 + 8, -8 * 48 + 4, 30, 40};
        gEnemy.createEnemy(rect2);
        
        int countedFrames = 0;
        fpsTimer.start();
        
        SDL_Rect buttonClipRect1 {339, 98, 49, 49};
        SDL_Rect buttonClipRect2 {290, 98, 49, 45};
        SDL_Rect rects[2] {buttonClipRect1, buttonClipRect2};
        
        MainMenuButton button;
        button.createMainMenuButton(textureRegistry.requestAccess(2), textureRegistry.requestAccess(3), 10, 10, rects);
        
        MainMenuButton button2;
        button2.createMainMenuButton(textureRegistry.requestAccess(2), textureRegistry.requestAccess(9), 70, 10, rects);

        button.lambdaActivate = [](MainMenuButton *self, bool &isSelected) { isSelected = !isSelected; };
        button.lambdaActivateMUS = [](MainMenuButton *self, bool &isSelected, Mix_Music* music) { isSelected = !isSelected; Mix_PlayMusic(music, 1); };
        
        button2.lambdaActivate = [](MainMenuButton *self, bool &isSelected) { isSelected = !isSelected; activateDiscus(); };
        
        gMap.loadMap();
        
        textboxProcessor.loadTextboxSequenceFromFile("textboxSequences.txt");
        textboxProcessor.loadTextboxSequenceFromFile("textboxSequences2.txt");
        textboxProcessor.loadTextboxSequenceFromFile("textboxSequences3.txt");
        textboxProcessor.loadTextboxSequenceFromFile("ts4.txt");
        
        textboxProcessor.runTextboxSequence(3);
        
        tileProcessor.loadEventsFromFile("specialTileCollisions.txt");
        tileProcessor.loadEventsFromFile("specialTileCollisions2.txt");
        tileProcessor.loadEventsFromFile("special3.txt");
        
        gMap.addObject((GameObject*) &gPlayer);
        for (int i = 0; i < boomboom.size(); i++)
            gMap.addObject((GameObject*) boomboom[i].get());
        
        clock_t oldTime = clock();
        double deltaTime = 0;
        double accumulator = 0;

        while (isRunning)
        {
            deltaTime = clock() - oldTime;
            oldTime = clock();
            accumulator += deltaTime;
            
            capTimer.start();
            start = std::chrono::system_clock::now();
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    isRunning = false;
                
                if (event.type == SDL_KEYDOWN)
                {
                    if (keyHanlder.queryKeyState(SDL_SCANCODE_LCTRL) && keyHanlder.queryKeyState(SDL_SCANCODE_P))
                    {
                        if (selectedMusicID < music.size() - 1)
                            selectedMusicID++;
                        else
                            selectedMusicID = 0;
                    }
                    else if (keyHanlder.queryKeyState(SDL_SCANCODE_LCTRL) && keyHanlder.queryKeyState(SDL_SCANCODE_C))
                        handleConsoleCommand();
                    else if (keyHanlder.queryKeyState(SDL_SCANCODE_LCTRL) && keyHanlder.queryKeyState(SDL_SCANCODE_S))
                        gMap.save();
                    else if (keyHanlder.queryKeyState(SDL_SCANCODE_P))
                        activateDiscus();
                }

                if (event.type == SDL_MOUSEBUTTONDOWN && button2.handleMouseClick(event))
                    button2.lambdaActivate(button2.getButtonState());
//                if (event.type == SDL_MOUSEBUTTONUP && button2.handleMouseClick(event))
//                    button2.lambdaActivate(button2.getButtonState());
                
                if (event.type == SDL_MOUSEBUTTONDOWN && button.handleMouseClick(event))
                    button.lambdaActivateMUS(button.getButtonState(), music[selectedMusicID]);
                if (event.type == SDL_MOUSEBUTTONUP && button.handleMouseClick(event))
                    button.lambdaActivate(button.getButtonState());
                
                if (!textboxProcessor.updateCurrentEvent(event) && !button.handleMouseClick(event) && !button2.handleMouseClick(event))
                    gPlayer.handleMouseClick(event);
                
                gPlayer.update(event);
            }
            
//            gPlayer.updateMana();
            gEnemy.update(event);
            
            for (std::shared_ptr<KBoomerang> boomerang : boomboom)
                boomerang->update(event);
            
                avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.0f);
                if (avgFPS > 2000000)
                    avgFPS = 0;
            
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            
            gMap.render();
            for (int i = 0; i < boomboom.size(); i++)
            {
                if (!boomboom[i].get()->getDelete())
                    boomboom[i].get()->render();
                else
                {
                    gMap.removeObject((GameObject*)(boomboom[i].get()));
                    boomboom.erase(boomboom.begin() + i);
                    i--;
                }
            }
                    
            button.render();
            button2.render();
            
            gEnemy.render();
            
            gPlayer.move();
            gPlayer.render();
            
            textboxProcessor.renderCurrentEvent();
        
            SDL_RenderPresent(gRenderer);
            SDL_RenderClear(gRenderer);
            
            countedFrames++;
            
            double frameTicks = capTimer.getTicks();
            if (frameTicks < 1000.0 / 62.0)
                SDL_Delay((1000.0 / 60.0) - frameTicks);
                        
//            std::cout << avgFPS << '\n';
            end = std::chrono::system_clock::now();
            accumulator -= 1.0 / 60.0;
        }
    }
    catch (TextureAccessException e)
    {
        std::cout << e.what() << "\n";
    }
    close();
    
    return 0;
}
