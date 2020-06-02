//
//  application.hpp
//  JuniorYearGame
//
//  Created by Keegan Bilodeau on 5/29/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#ifndef application_hpp
#define application_hpp

#include <memory>
#include <SDL.h>
#include <stdio.h>

///@brief A wrapper of data and functions necessary for creating the application
class Application
{
public:
    ///@brief Initializes the application's data members in preparation for application initialization
    Application();
    
    ///@brief Calls the application's cleanup when going out of scope
    ~Application();
    
    ///@brief Frees all data members
    void cleanup();
    
    ///@brief Initializes the external apis, window, and renderer
    bool init();
    
    ///@brief Loads external assets like music, maps, and images
    bool loadMedia();
    
    ///@brief Starts the application
    void run();
    
private:
    ///@brief A boolean flag indicating if the application is still running
    bool isRunning;
    
    ///@brief Constant representing the window's width
    const int SCREEN_WIDTH = 750;
    ///@brief Constant representing the window's height
    const int SCREEN_HEIGHT = 750;
    
    ///@brief A pointer to the SDL Window object, representing the application's window
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> mWindow;
    
    ///@brief A shared pointer to the SDL renderer that can be shared to game object render components
    std::shared_ptr<SDL_Renderer> mRenderer;
};

#endif /* application_hpp */
