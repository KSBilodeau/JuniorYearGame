//
//  textureWrapper.hpp
//  gameEngine
//
//  Created by Keegan Bilodeau on 12/30/19.
//  Copyright © 2019 Keegan Bilodeau. All rights reserved.
//

#ifndef textureWrapper_hpp
#define textureWrapper_hpp

#include <SDL.h>

#include <stdio.h>
#include <string>

class KTexture
{
public:
    // Initialize variables
    KTexture();
    
//    // My beautiful copy constructor
//    KTexture(const KTexture &oldObject);
//    
//    // Free hardware texture and variables
//    ~KTexture();
    
    // Free textures and variables
    void free();
    
    // Create texture from image file
    bool loadFromFile(std::string path, double scale);
    
    // Create a texture from a string (For text rendering)
    bool loadFromString(std::string text, SDL_Color textColor);
    
    // Creates a wrapped texture from an already existing SDL Texture
    bool loadFromTexture(std::shared_ptr<SDL_Texture> &texture);
    
    // Creates and returns a texture of a rectangle of a specific color
    KTexture generateTextureFromRect(SDL_Rect rect, SDL_Color color);
    
    // Render texture to screen
    void render(int x, int y, bool fixed, SDL_Rect* clipRect = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
    
    // Enable alpha channel
    void setBlendMode(SDL_BlendMode blendMode);
    
    // Apply color filter
    void modifyTextureColor(Uint8 red, Uint8 green, Uint8 blue);
    
    // Modify texture opacity
    void modifyAlpha(Uint8 alpha);
    
    // Modify texture scale
    void modifyTextureScale(double xScale, double yScale);
    
    // Return texture's original dimensions
    int getWidth();
    int getHeight();
    
private:
    // The SDL texture being wrapped
    std::shared_ptr<SDL_Texture> mTexture;
    
    // Texture dimensions
    int mWidth, mHeight;
    
    // Texture scale factor
    double mXScale, mYScale;
};

#endif /* textureWrapper_hpp */
