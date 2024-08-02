#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "sim_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"
#include "jointastar.h"


class Grid
{
public:
    // level state
    std::vector<SimObject> Bricks;
    unsigned int unitWidth, unitHeight;
    // constructor
    Grid() { }
    // loads level from file
    void Load(const char *file, unsigned int levelWidth, unsigned int levelHeight, vector<glm::vec3> &col, vector<Point> &goals);
    // render level
    void Draw(SpriteRenderer &renderer);
private:
    // initialize level from tile data
    void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight, vector<glm::vec3> &colors, vector<Point> &goals);
};

#endif