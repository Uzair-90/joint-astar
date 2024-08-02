#include "sim.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "sim_object.h"
#include "robot.h"
#include <iostream>
#include <cmath>
#include "jointastar.h"

// Game-related State data
SpriteRenderer  *Renderer, *renderer2;
std::vector<glm::vec2> InitialPositions;

Sim::Sim(unsigned int width, unsigned int height) 
    : Width(width), Height(height)
{ 

}

Sim::~Sim()
{
    delete Renderer;
    for (auto robot : Robots) {
        delete robot;
    }
    Robots.clear();
}

void Sim::Init()
{
    // load shaders
    ResourceManager::LoadShader("C:/Users/Lenovo/Desktop/simulator/shaders/sprite.vs", "C:/Users/Lenovo/Desktop/simulator/shaders/sprite.fs", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), 
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    renderer2 = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    
    // load textures
    ResourceManager::LoadTexture("C:/Users/Lenovo/Desktop/robotsimulator/textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("C:/Users/Lenovo/Desktop/robotsimulator/textures/robot.png", true, "face");
    ResourceManager::LoadTexture("C:/Users/Lenovo/Desktop/robotsimulator/textures/block.png", false, "block");



    for(int i = 0; i < NUM_OF_ROBOTS; i++){
        Robots.push_back(new Robot());
    }

    //testing colors
    std::vector<glm::vec3> colors = {
        glm::vec3(1.0f, 0.2f, 1.0f), // vivid pink
        glm::vec3(0.2f, 1.0f, 1.0f), // bright cyan
        glm::vec3(0.2f, 0.4f, 1.0f), // medium blue
        glm::vec3(1.0f, 1.0f, 1.0f), // white
        glm::vec3(0.9f, 0.3f, 0.2f), // bright green
        glm::vec3(1.0f, 0.4f, 0.8f), // pinkish
        glm::vec3(0.6f, 0.6f, 1.0f), // light lavender
        glm::vec3(0.1f, 0.8f, 1.0f), // light blue
        glm::vec3(1.0f, 0.7f, 1.0f), // pastel pink
        glm::vec3(0.5f, 0.4f, 1.0f)  // muted periwinkle
    };


    
    for(int i = 0; i < NUM_OF_ROBOTS; i++)
    {
        glm::vec3 color(colors[i]);
        Robots[i]->setColor(color);
    }

    
    vector<Point> starts = {{2, 3}, {2, 1}, {3, 0}, {1, 1}, {2, 0}, {3, 1}};
    vector<Point> goals = {{0, 0}, {3, 3}, {0, 3}, {2, 2}, {0, 2}, {2, 1}};

    Level.Load("C:/Users/Lenovo/Desktop/robotsimulator/levels/one.lvl", this->Width,this->Height,colors,goals);

    //grid size
    int n = 4;
    vector<vector<Point>> path = jointStateAStar(starts, goals, n);

    // Print the found path or indicate no path found
    if (!path.empty()) {
        cout << "Path found:" << endl;
        // Vector to store each column as a separate path
        vector<vector<Point>> agentPaths(path[0].size());

        for (const auto& state : path) {
            for (size_t col = 0; col < state.size(); ++col) {
                agentPaths[col].push_back(state[col]);
            }
        }

        // Display each separated path (each column) horizontally
        for (size_t col = 0; col < agentPaths.size(); ++col) {
            for (const auto& pos : agentPaths[col]) {
                if (col < Robots.size()) {
                    Robots[col]->Path.push_back({pos.x, pos.y});
                }
            }
            cout << endl;
        }

        int x = 0;
        // Call startpos for each robot
        for (auto& robot : Robots) {
            if (robot) {
                robot->startpos(Width, Height, NUM_OF_ROBOTS);
                x++;
            }
        }
    } else {
        cout << "No path found." << endl;
    }


}

void Sim::Update(float dt)
{
    //int i = 0;
    // Update each robot
    for (auto robot : Robots) {

        //cout<<"Starting simulation for robot: "<<i<<"\n";
        //i++;

        if (robot->currentPathIndex < robot->Path.size()) { // Ensure there is a next point in the path

            int x = robot->Path[robot->currentPathIndex ][0] - robot->Path[robot->currentPathIndex - 1][0];
            int y = robot->Path[robot->currentPathIndex ][1] - robot->Path[robot->currentPathIndex - 1][1];

            //std::cout<<"X: "<<x<<" Y: "<<y<<"\n";

            //setting current direction variable 
            robot->currentDirection= glm::vec2(x,y);   

            robot->rotateRobot(robot->currentDirection, dt);

            if (!robot->isrotating)
            {
                glm::vec2 targetPosition = glm::vec2(robot->InitialPosition.x + (x * 200.0f), robot->InitialPosition.y + (y * 150.0f));
                glm::vec2 currentPosition = robot->GetPosition();

                // Check if the robot has reached the target position
                if (glm::distance(currentPosition, targetPosition) < 1.0f) {
                    // Move to the next target position
                    robot->isrotating = true;
                    robot->InitialPosition = currentPosition;
                    robot->currentPathIndex++;
                } 
                    // Update the robot's position
                robot->Move(dt, x, y, targetPosition);
                
            }
        }
    }
}





void Sim::Render()
{
    // Draw background, levels, and all robots
    Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
    this->Level.Draw(*Renderer);
    for (auto robot : Robots) {

        robot->Draw(*renderer2); 
    }
    
}

