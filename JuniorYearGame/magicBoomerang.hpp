//
//  magicBoomerang.hpp
//  gameEngine
//
//  Created by Keegan Bilodeau on 2/21/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#ifndef magicBoomerang_hpp
#define magicBoomerang_hpp

#include "gameObject.hpp"
#include "textureWrapper.hpp"

#include <stdio.h>
#include <vector>
#include <chrono>

class KBoomerang : GameObject
{
public:
    // Initialize variables
    KBoomerang();
    
    // Delete boomerang
    void free() override;
    
    // Create boomerang
    void createBoomerang(SDL_Color color, std::vector<double> pValues, SDL_Point center, SDL_Rect rect, bool isAnimation = false);
    
    // Handles the rectanlges' rotation and position
    void update(SDL_Event &event) override;
    
    // Renders the rectangles at their designated location
    void render() override;
    
    // NOT USED FOR PROJECTILES
    bool handleMouseClick(SDL_Event &event) override { return false; };
    
    // Modifies the angle that the projectile is travelling at
    void modVelAngle(double angle);
    
    // Modifies the rectanlge rotation speed
    void modRectRotationSpeed(double speed);
    
    // Returns projectile rectangle
    SDL_Rect &getRectangle() override { return mRect; };
    
    // Returns the state of projectiles deletion flag
    bool &getDelete();
    
    // Check if projectile is colliding with another object
    bool checkCollision(GameObject* object) override;
    
    // Used to rotate projectile in cirle shape
    void rotateInCircle();
    
    // Used to send projectile in straight line
    void sendInLine();
    
    // Toggles the circle flag
    void toggleCircleFlag();
    
    // Makes a boomerang an enemy projectile
    void makeEnemy();
    
    // Disables life timer
    void disableTimer();
    
    GameObjectType getType() override
    {
        if (discusFlag)
            return GameObjectType::discus;
        else if (enemyFlag)
            return GameObjectType::enemyboomerang;
        else
            return GameObjectType::boomerang;
    }
    
private:
    // Flag marking for deletion
    bool deleteFlag;
    
    // Flag indicates circle pattern
    bool circleFlag;
    
    // Flag indicates if boomerang is a discus
    bool discusFlag;
    
    // Flag indicates if boomerang is belongs to enemy
    bool enemyFlag;
    
    // Flag indicates if time is enabled
    bool timerEnabled;
    
    // Indicates if boomerangs are part of animation
    bool animationFlag;
    
    // Angle the projectile was sent
    double mVelAngle;
    // Radius of the circle the projectile is traveling
    double mRadius;
    
    // Angle that the sprite is rotated
    double mRotateAngle;
    // Speed that the projectile is rotating
    double mRotateSpeed;
    
    // Stores a double version of its coordinates for more accurate movement
    double mPosX, mPosY;
    
    // Stores the number of times the boomerang has "bounced"
    int numOfBounces;
    
    // Center of the collision box
    // Centers the sprite rotation for SDL Rendering
    SDL_Point mCenter;
    // Color of the projectile
    SDL_Color mColor;
    // Collision rectangle for projectile
    SDL_Rect mRect;
    // Texture for rotation
    KTexture mTexture;
};

#endif /* magicBoomerang_hpp */
