#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "grid_level.h"
#include "robot.h"

#define NUM_OF_ROBOTS 6
#define NUM_OF_BLOCKS 4

const glm::vec2 INITIAL_VELOCITY(100.0f, 100.0f);

const float RADIUS = 40.0f;

class Sim
{
public:
    // game state
    unsigned int Width, Height;
    Grid  Level;  
    // constructor/destructor
    Sim(unsigned int width, unsigned int height);
    ~Sim();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void Update(float dt);
    void Render();

private:
    std::vector<Robot*> Robots;
    //float lastRotationTime;
};

#endif
