#include "robot.h"
#include "sim.h"
#include<vector>
#include<cmath>

// Constructor with default values
Robot::Robot()
    : InitialPosition(0.0f, 0.0f),
      CurrentPosition(0.0f, 0.0f),
      Velocity(200.0f, 200.0f),
      angularvelocity(50.0f),
      rotationangle(0.0f),
      Radius(RADIUS), 
      Sprite(ResourceManager::GetTexture("face")),
      isrotating(false),
      color(1.0f),
      previousDirection(0.0f),
      currentDirection(0.0f)
{ }

// Constructor with specified values
Robot::Robot(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : InitialPosition(pos),
      CurrentPosition(pos),
      Velocity(velocity),
      Radius(radius),
      rotationangle(0.0f),
      Sprite(sprite),
      angularvelocity(100.0f),
      isrotating(false) // Remove this comma
{
    color = glm::vec3(0.5f);
}


void Robot::setColor(glm::vec3 col){
    color = col;
}


void Robot::startpos(unsigned int width, unsigned int height, int NR ){
    //the parameter x is useless it is just used to overload the function
    int x = (Path[0][0] * 200.0f) + 100.0f - RADIUS;
    int y = (Path[0][1] * 150.0f) + (75.0f)  - RADIUS;

    InitialPosition = glm::vec2(x, y);
    CurrentPosition = InitialPosition;
    
    std::cout<<"Current position of the robot is: ("<<CurrentPosition.x<<" , "<<CurrentPosition.y<<")"<<"\n";
}


void Robot::rotateRobot(glm::vec2 currentdirection, float dt){

     if(currentdirection.x == 1)
        targetangle =  90.0f;  
    if(currentdirection.x == -1)
        targetangle =  -90.0f;
    if(currentdirection.y == 1)
        targetangle = 180.0f; 
    if(currentdirection.y == -1)
        targetangle = 0.0f; 

    if(std::abs(this->rotationangle - targetangle) < 1.0f)
        this->isrotating = false;

    //float direction = glm::normalize(targetangle - this->rotationangle);
    //float rotation = rotationangle; //direction * this->angularvelocity;
    this->rotationangle = targetangle;
}




// Function to move the ball
void Robot::Move(float dt, int x, int y, glm::vec2 targetPosition)
{
    
    glm::vec2 velocity = this->Velocity;
    velocity.x = x * velocity.x;
    velocity.y = y * velocity.y;
    // Calculate movement amount
    glm::vec2 movement = velocity * dt;

    if(x > 0){
        if(CurrentPosition.x + movement.x > targetPosition.x) {CurrentPosition.x = targetPosition.x; movement.x = 0;}
    }
    else if(x < 0){
        if(CurrentPosition.x + movement.x < targetPosition.x) {CurrentPosition.x = targetPosition.x; movement.x = 0;}
    }
    if(y > 0){
        if(CurrentPosition.y + movement.y > targetPosition.y) {CurrentPosition.y = targetPosition.y; movement.y = 0;}
    }
    else if(y < 0){
        if(CurrentPosition.y + movement.y < targetPosition.y) {CurrentPosition.y = targetPosition.y; movement.y = 0;}
    }
    // Update the ball's CurrentPosition
    CurrentPosition += movement;
}



// Function to reset the ball's CurrentPosition and velocity
void Robot::Reset(glm::vec2 CurrentPosition, glm::vec2 velocity)
{
    CurrentPosition = CurrentPosition;
    Velocity = velocity;
}

// Function to draw the ball
void Robot::Draw(SpriteRenderer &renderer)
{
    glm::vec3 test(1.0f);
    renderer.DrawSprite(Sprite, CurrentPosition, glm::vec2(Radius * 2, Radius * 2), rotationangle, color);
}

// Getter function for retrieving the ball's CurrentPosition
glm::vec2 Robot::GetPosition()
{
    return CurrentPosition;
}
