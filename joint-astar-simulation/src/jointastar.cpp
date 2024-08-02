#include "jointastar.h"

// Check if a point is within the grid bounds
bool isValid(const Point& pt, int n) {
    return pt.x >= 0 && pt.x < n && pt.y >= 0 && pt.y < n;
}

// Calculate the Manhattan distance between two points
int manhattanDistance(const Point& a, const Point& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// Check if there is a vertex conflict among multiple agents
bool hasVertexConflict(const vector<Point>& positions) {
    set<Point, PointComparator> pos_set;
    for (const auto& pos : positions) {
        if (pos_set.count(pos)) return true;
        pos_set.insert(pos);
    }
    return false;
}

// Check if there is an edge conflict between two states
bool hasEdgeConflict(const vector<Point>& prev_positions, const vector<Point>& new_positions) {
    set<pair<Point, Point>> edges;
    for (size_t i = 0; i < prev_positions.size(); ++i) {
        pair<Point, Point> edge1 = {prev_positions[i], new_positions[i]};
        pair<Point, Point> edge2 = {new_positions[i], prev_positions[i]};
        if (edges.count(edge2)) return true;
        edges.insert(edge1);
    }
    return false;
}

// Generate successors of the current state
vector<State> getSuccessors(const State& current, int n) {
    static const vector<Point> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {0, 0}};
    vector<State> successors;
    for (int mask = 0; mask < pow(directions.size(), current.positions.size()); mask++) {
        vector<Point> new_positions = current.positions;
        int tmask = mask;
        for (int j = 0; j < current.positions.size(); j++) {
            new_positions[j].x += directions[tmask % directions.size()].x;
            new_positions[j].y += directions[tmask % directions.size()].y;
            tmask /= directions.size();
        }
        if (!hasVertexConflict(new_positions) && !hasEdgeConflict(current.positions, new_positions)) {
            successors.push_back({new_positions, current.g + 1, 0});  // h will be set later
        }
    }
    return successors;
}

// Implement the Joint State A* algorithm
vector<vector<Point>> jointStateAStar(const vector<Point>& starts, const vector<Point>& goals, int n) {
    auto heuristic = [&goals](const vector<Point>& positions) {
        int h = 0;
        for (size_t i = 0; i < positions.size(); ++i) {
            h += manhattanDistance(positions[i], goals[i]);
        }
        return h;
    };

    priority_queue<State, vector<State>, greater<State>> openSet;
    unordered_map<vector<Point>, int, VectorPointHash, VectorPointEqual> gScore;
    unordered_map<vector<Point>, vector<Point>, VectorPointHash, VectorPointEqual> cameFrom;

    State startState = {starts, 0, heuristic(starts)};
    openSet.push(startState);
    gScore[starts] = 0;

    while (!openSet.empty()) {
        State current = openSet.top();
        openSet.pop();

        if (current.positions == goals) {
            vector<vector<Point>> path;
            vector<Point> state = current.positions;
            while (state != starts) {
                path.push_back(state);
                state = cameFrom[state];
            }
            path.push_back(starts);
            reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& neighbor : getSuccessors(current, n)) {
            int tentative_gScore = gScore[current.positions] + 1;
            if (gScore.find(neighbor.positions) == gScore.end() ||
                tentative_gScore < gScore[neighbor.positions]) {
                cameFrom[neighbor.positions] = current.positions;
                gScore[neighbor.positions] = tentative_gScore;
                int h = heuristic(neighbor.positions);
                openSet.push({neighbor.positions, tentative_gScore, h});
            }
        }
    }

    return {};  // Return empty path if no solution found
}
