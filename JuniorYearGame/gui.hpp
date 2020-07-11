//
//  gui.hpp
//  gameEngine
//
//  Created by Keegan Bilodeau on 12/30/19.
//  Copyright © 2019 Keegan Bilodeau. All rights reserved.
//

#ifndef gui_hpp
#define gui_hpp

#include <stdio.h>

#include "gameObject.hpp"
#include "guiButtons.hpp"

class GUI : GameObject
{
public:
    // Init variables
    GUI();
    
    // Deallocate variables
    void free() override;
    
    // Create a GUI
    void createGUI(SDL_Rect rect, std::vector<MainMenuButton> *buttons = NULL);
    
    // Render gui and buttons
    void render() override;
    
    // UNUSED
    void update(SDL_Event &event) override { };
    
    // Activates buttons
    bool handleMouseClick(SDL_Event &event) override;
    
    // Returns gui rectangle
    virtual SDL_Rect &getRectangle() override;
    
private:
    // Stores all buttons
    std::vector<MainMenuButton> mButtons;
    
    // Location of the GUI
    SDL_Rect mRect;
};

#endif /* gui_hpp */
