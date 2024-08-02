#include "grid_level.h"
#include <iostream>
#include <fstream>
#include <sstream>

void Grid::Load(const char *file, unsigned int levelWidth, unsigned int levelHeight, vector<glm::vec3> &col, vector<Point> &goals)
{
    // clear old data
    this->Bricks.clear();
    // load from file
    unsigned int tileCode;
    Grid level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0){
            
            for(int i = 0; i < goals.size(); ++i){
                glm::vec3 color(col[i]);
                int x = goals[i].x;
                int y = goals[i].y;
                this->init(tileData, levelWidth, levelHeight, col, goals);
            }
            
        }
    }
}

void Grid::Draw(SpriteRenderer &renderer)
{
    for (SimObject &tile : this->Bricks)
            tile.Draw(renderer);
}

void Grid::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight, vector<glm::vec3> &colors, vector<Point> &goals)
{
    // calculate dimensions
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size(); // note we can index vector at [0] since this function is only called if height > 0
    float unit_width = levelWidth / static_cast<float>(width), unit_height = levelHeight / height;
    this->unitWidth = unit_width;
    this->unitHeight = unit_height;
    
     
    //initialize positions in the grid where the goals state is

    for(int i = 0; i < goals.size(); i++){
        tileData[goals[i].y][goals[i].x] = 0;
        glm::vec3 color = glm::vec3(colors[i]);
        glm::vec2 pos(unit_width * goals[i].x, unit_height * goals[i].y);
        glm::vec2 size(unit_width, unit_height);
        this->Bricks.push_back(SimObject(pos, size, ResourceManager::GetTexture("block"), color));
    }


    // initialize level tiles based on tileData		
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            
            if (tileData[y][x] == 1) // solid // check block type from level data (2D level array)
            {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                SimObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
                this->Bricks.push_back(obj);
            }
            else if (tileData[y][x] > 1)	// non-solid; now determine its color based on level data
            {
                glm::vec3 color = glm::vec3(1.0f); // original: white
                if (tileData[y][x] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[y][x] == 3)
                    color = glm::vec3(0.0f, 0.7f, 1.0f);
                else if (tileData[y][x] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[y][x] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                this->Bricks.push_back(SimObject(pos, size, ResourceManager::GetTexture("block"), color));
            }
        }
    }
}