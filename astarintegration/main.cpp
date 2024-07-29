#include "astar.h"
#include<vector>

int main() {
    int n = 5;  // Grid size
    vector<Point> starts = {{2, 3}, {2, 1}};
    vector<Point> goals = {{4, 2}, {4, 4}};

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

        vector<vector<int>> pathi;
        int x, y;
        // Display each separated path (each column) horizontally
        cout << "\nAgent paths (each column):" << endl;
        for (const auto& agentPath : agentPaths) {
            for (const auto& pos : agentPath) {
                pathi.push_back({pos.x , pos.y});
            }
        }

        for(const auto& pth: pathi){
            cout<<"( "<<pth[0]<<" , "<<pth[1]<<" )->";
        }
    }

    return 0;
}
