#ifndef BALL_OBJECT_H
#define BALL_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <stdio.h>
#include "texture.h"
#include "sprite_renderer.h"
#include <vector>
#include<map>

#define UNIT_ROTATION 1
#define UNIT_FULL_ROTATION 2

// Defines a Ball object with relevant properties and methods
class Robot
{
public:
    // Ball state
    glm::vec2   InitialPosition, CurrentPosition, Velocity, Direction;
    float       Radius;
    float rotationangle;
    bool isrotating = true;
    float targetangle = 0;
    float angularvelocity;
    Texture2D   Sprite;
    std::vector<std::vector<int>> Path;
    unsigned int currentPathIndex = 1;
    glm::vec3 color;
    glm::vec2 previousDirection;
    glm::vec2 currentDirection;

    // Constructor
    Robot();
    Robot(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

    // Functions
    void Move(float dt, int x, int y, glm::vec2 targetPosition);
    void Reset(glm::vec2 position, glm::vec2 velocity);
    void Draw(SpriteRenderer &renderer);
    void startpos(unsigned int width, unsigned int height, int NR);
    void rotateRobot(glm::vec2 previousDirection, float dt);
    void setColor(glm::vec3 col);

    // Getter functions
    glm::vec2 GetPosition();
};

#endif
