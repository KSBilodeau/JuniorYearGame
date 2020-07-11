//
//  block.hpp
//  gameEngine
//
//  Created by Keegan Bilodeau on 1/12/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#ifndef block_hpp
#define block_hpp

#include <stdio.h>
#include <SDL.h>
#include <map>

#include <boost/serialization/map.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "tile.hpp"

class KBlock
{
public:
    friend class boost::serialization::access;
    
    // Initialize variables
    KBlock();
    
    // Deallocate variables
    void free();
    
    // Creates a default tile and adds it to block
    void loadDefaultTile(std::pair<int, int> tileID);
    
    // Render entire block
    void render();
    
    // Attach tile collision event
    void attachTileCollsionEvent(int x, int y, int &id, int &tileSheet, int &storageID, std::pair<int, int> &tileID, EventType eventType);
    
    // Set block position
    void setBlockPosition(int x, int y);
    
    // Handles click events on block
    void handleMouseClick(SDL_Event &event);
    
    // Serialize block
    template<class Archive>
    void serialize(Archive &ar, unsigned int version)
    {
        ar & mRectangle.x;
        ar & mRectangle.y;
        ar & mRectangle.w;
        ar & mRectangle.h;
        
        ar & mMappedTiles;
        
        ar & mDefaultTile;
    }
    
    // Returns true if block contains no mapped tiles
    bool isEmpty();
    
    // Returns all mapped tiles in the block
    std::map<std::pair<int, int>, KTile> &getMappedTiles();
    
    // Returns block's dimensions
    SDL_Rect &getRectangle();
    
private:
    // All mapped tiles in the block
    std::map<std::pair<int, int>, KTile> mMappedTiles;
    
    // Block dimensions
    SDL_Rect mRectangle;
    
    // Default tile that will render in the background
    std::pair<int, int> mDefaultTile;
};

#endif /* block_hpp */
