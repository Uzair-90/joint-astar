#ifndef JOINT_STATE_ASTAR_H
#define JOINT_STATE_ASTAR_H

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <set>
#include <algorithm>
#include <functional>

using namespace std;

// Define the Point structure representing a grid cell
struct Point {
    int x, y;

    // Define the equality operator for Point
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }

    // Define the less-than operator for Point to be used in std::set
    bool operator<(const Point& other) const { return tie(x, y) < tie(other.x, other.y); }
};

// Define a hash function for Point to be used in unordered_map
struct PointHash {
    size_t operator()(const Point& p) const { return hash<int>()(p.x) ^ hash<int>()(p.y); }
};

// Define a comparator for Point to be used in std::set
struct PointComparator {
    bool operator()(const Point& a, const Point& b) const { return tie(a.x, a.y) < tie(b.x, b.y); }
};

// Define a hash function for vector<Point> to be used in unordered_map
struct VectorPointHash {
    size_t operator()(const vector<Point>& v) const {
        size_t hash_value = 0;
        for (const auto& p : v) {
            hash_value ^= PointHash()(p) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
        }
        return hash_value;
    }
};

// Define an equality function for vector<Point> to be used in unordered_map
struct VectorPointEqual {
    bool operator()(const vector<Point>& a, const vector<Point>& b) const { return a == b; }
};

// Define the State structure representing a state in the search
struct State {
    vector<Point> positions;  // Positions of all agents
    int g, h;                 // Cost and heuristic

    // Custom comparator for State to be used in priority_queue
    bool operator>(const State& other) const { return g + h > other.g + other.h; }
};

// Check if a point is within the grid bounds
bool isValid(const Point& pt, int n);

// Calculate the Manhattan distance between two points
int manhattanDistance(const Point& a, const Point& b);

// Check if there is a vertex conflict among multiple agents
bool hasVertexConflict(const vector<Point>& positions);

// Check if there is an edge conflict between two states
bool hasEdgeConflict(const vector<Point>& prev_positions, const vector<Point>& new_positions);

// Generate successors of the current state
vector<State> getSuccessors(const State& current, int n);

// Implement the Joint State A* algorithm
vector<vector<Point>> jointStateAStar(const vector<Point>& starts, const vector<Point>& goals, int n);

#endif // JOINT_STATE_ASTAR_H
