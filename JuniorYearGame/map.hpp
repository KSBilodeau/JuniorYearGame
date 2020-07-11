//
//  map.hpp
//  gameEngine
//
//  Created by Keegan Bilodeau on 1/12/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#ifndef map_hpp
#define map_hpp

#include <stdio.h>
#include <SDL.h>
#include <map>
#include <vector>

#include <boost/serialization/map.hpp>

#include "tile.hpp"
#include "block.hpp"
#include "gameObject.hpp"

class KMap
{
public:
    // Initalize variables
    KMap();
    
    // Clear and erase map
    ~KMap();
    
    // Load a map
    void loadMap();
    
    // Updates all the tiles and objects in the game
    void update(SDL_Event &event);
    
    // Renders all the tiles and objects
    void render();
    
    // Saves the mapped tiles to a text file
    void save();
    
    // Loads blocks at specific coordinates block coords if not already loaded
    void loadBlock(std::pair<int, int> coords);
    
    // Erases block at specific block coords if there is anything there
    void eraseBlock(std::pair<int, int> coords);
    
    // Attach tile event
    void attachTileCollisionEvent(int &x, int &y, int &id, int &tileSheet, int &storageID, std::pair<int, int> tileID, EventType eventType);
    
    // Clears map of all mapped tiles
    void clearMap();
    
    // Checks if an object is colliding with a tile
    bool checkCollision(std::pair<int, int> blockCoords, SDL_Rect objectRect, bool isPlayer = true);
    // Checks if an object is colliding with another object
    bool checkCollision(GameObject* object);
    
    // Add a single object to list
    void addObject(GameObject* object);
    
    // Remove a single object from the list
    void removeObject(GameObject* object);
    
    // Add multiple objects
    void addObjects(int argc, ...);
    
    // Boost saving function (Is inline for the sake of space)
    template<class Archive>
    void serialize(Archive &ar, unsigned int version)
    {
        ar & mBlockList;
    };
    
private:
    // All loaded blocks
    std::map<std::pair<int, int>, KBlock> mBlockList;
    
    // All loaded objects
    std::vector<GameObject*> objects;
};

#endif /* map_hpp */
