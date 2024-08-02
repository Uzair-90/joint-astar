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
bool isValid(const Point& pt, int n) { return pt.x >= 0 && pt.x < n && pt.y >= 0 && pt.y < n; }

// Calculate the Manhattan distance between two points
int manhattanDistance(const Point& a, const Point& b) { return abs(a.x - b.x) + abs(a.y - b.y); }

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
  cout << "Generating successors for state with g = " << current.g << " and positions: ";
  for (const auto& pos : current.positions) {
    cout << "(" << pos.x << "," << pos.y << ") ";
  }
  cout << "\n";

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
      cout << "Generated successor with positions: ";
      for (const auto& pos : new_positions) {
        cout << "(" << pos.x << "," << pos.y << ") ";
      }
      cout << endl;
    } else {
      cout << "Conflict detected for positions: ";
      for (const auto& pos : new_positions) {
        cout << "(" << pos.x << "," << pos.y << ") ";
      }
      cout << "\n";
    }
  }

  cout << "Generated " << successors.size() << " successors.\n";
  return successors;
}

// Implement the Joint State A* algorithm
vector<vector<Point>> jointStateAStar(const vector<Point>& starts, const vector<Point>& goals, int n) {
  cout << "Starting Joint State A* with grid size " << n << "\n";

  // Define the heuristic function
  auto heuristic = [&goals](const vector<Point>& positions) {
    int h = 0;
    for (size_t i = 0; i < positions.size(); ++i) {
      h += manhattanDistance(positions[i], goals[i]);
    }

    // cout<<"Value of H: "<<h<<"\n";

    return h;
  };

  // Define the priority queue and other structures
  priority_queue<State, vector<State>, greater<State>> openSet;
  unordered_map<vector<Point>, int, VectorPointHash, VectorPointEqual> gScore;
  unordered_map<vector<Point>, vector<Point>, VectorPointHash, VectorPointEqual> cameFrom;

  // Initialize the start state
  State startState = {starts, 0, heuristic(starts)};
  openSet.push(startState);
  gScore[starts] = 0;

  cout << "Initial state with g=" << startState.g << " and h=" << startState.h
       << " pushed to openSet\n";

  // Main loop of the A* algorithm
  while (!openSet.empty()) {
    State current = openSet.top();
    openSet.pop();

    cout << "Exploring state with g=" << current.g << " and positions: ";
    for (const auto& pos : current.positions) {
      cout << "(" << pos.x << "," << pos.y << ") ";
    }
    cout << "\n";

    // Check if we have reached the goal
    if (current.positions == goals) {
      cout << "Goal reached!" << "\n";
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

    // Generate and evaluate successors
    for (const auto& neighbor : getSuccessors(current, n)) {
      int tentative_gScore = gScore[current.positions] + 1;
      if (gScore.find(neighbor.positions) == gScore.end() ||
          tentative_gScore < gScore[neighbor.positions]) {
        cout << "Updating state with new gScore " << tentative_gScore << "\n";
        cameFrom[neighbor.positions] = current.positions;
        gScore[neighbor.positions] = tentative_gScore;
        int h = heuristic(neighbor.positions);
        openSet.push({neighbor.positions, tentative_gScore, h});
        cout << "Pushing new state to openSet with g=" << tentative_gScore << " and h=" << h
             << "\n";
      }
    }
  }

  cout << "No path found\n";
  return {};  // Return empty path if no solution found
}

// Main function to test the Joint State A* algorithm
int main() {
  int n = 3;  // Grid size
  vector<Point> starts = {{1, 1}, {1, 0}, {2, 1}};
  vector<Point> goals = {{1, 0}, {1, 2}, {0, 1}};

  vector<vector<Point>> path = jointStateAStar(starts, goals, n);

  // Print the found path or indicate no path found
  if (!path.empty()) {
    cout << "Path found:\n";
    for (const auto& state : path) {
      for (const auto& pos : state) {
        cout << "(" << pos.x << "," << pos.y << ") ";
      }
      cout << endl;
    }
  } else {
    cout << "No path found\n";
  }

  return 0;
}