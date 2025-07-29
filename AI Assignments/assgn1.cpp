#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>

using namespace std;

const vector<vector<int>> GOAL = {{1,2,3}, {4,5,6}, {7,8,0}};

struct State {
    vector<vector<int>> board;
    vector<string> path; // For showing moves (optional)
    int x, y; // Position of zero (blank tile)

    State(vector<vector<int>> b) {
        board = b;
        for(int i=0; i<3; i++)
            for(int j=0; j<3; j++)
                if(board[i][j] == 0) {
                    x = i;
                    y = j;
                }
    }

    bool operator<(const State& other) const {
        return board < other.board;
    }

    bool isGoal() const {
        return board == GOAL;
    }

    void print() const {
        for(auto& row : board) {
            for(auto& cell : row)
                cout << (cell == 0 ? ' ' : '0'+cell) << " ";
            cout << endl;
        }
        cout << "--------" << endl;
    }
};

// Directions: up, down, left, right
const vector<pair<int, int>> DIRECTIONS = {{-1,0}, {1,0}, {0,-1}, {0,1}};
const vector<string> DIR_NAMES = {"Up", "Down", "Left", "Right"};

// Generate next states
vector<State> get_neighbors(const State& current) {
    vector<State> neighbors;
    for(int i=0; i<4; i++) {
        int newX = current.x + DIRECTIONS[i].first;
        int newY = current.y + DIRECTIONS[i].second;

        if(newX >= 0 && newX < 3 && newY >= 0 && newY < 3) {
            State next = current;
            swap(next.board[current.x][current.y], next.board[newX][newY]);
            next.x = newX;
            next.y = newY;
            next.path = current.path;
            next.path.push_back(DIR_NAMES[i]);
            neighbors.push_back(next);
        }
    }
    return neighbors;
}

// BFS
bool bfs(State start) {
    set<vector<vector<int>>> visited;
    queue<State> q;
    q.push(start);

    while(!q.empty()) {
        State current = q.front(); q.pop();

        if(current.isGoal()) {
            cout << "BFS: Goal reached in " << current.path.size() << " moves.\n";
            for(const string& move : current.path)
                cout << move << " ";
            cout << "\nPath:\n";
            current.print();
            return true;
        }

        visited.insert(current.board);
        for(auto& neighbor : get_neighbors(current)) {
            if(visited.find(neighbor.board) == visited.end())
                q.push(neighbor);
        }
    }

    cout << "BFS: No solution found.\n";
    return false;
}

// DFS with depth limit
bool dfs(State start, int maxDepth = 30) {
    set<vector<vector<int>>> visited;
    stack<pair<State, int>> stk;
    stk.push({start, 0});

    while(!stk.empty()) {
        auto top = stk.top(); stk.pop();
        State current = top.first;
        int depth = top.second;

        if(current.isGoal()) {
            cout << "DFS: Goal reached in " << current.path.size() << " moves.\n";
            for(const string& move : current.path)
                cout << move << " ";
            cout << "\nPath:\n";
            current.print();
            return true;
        }

        if(depth > maxDepth)
            continue;

        visited.insert(current.board);
        for(auto& neighbor : get_neighbors(current)) {
            if(visited.find(neighbor.board) == visited.end())
                stk.push({neighbor, depth + 1});
        }
    }

    cout << "DFS: No solution found (depth limit = " << maxDepth << ").\n";
    return false;
}

int main() {
    // Example start state (can be changed)
    vector<vector<int>> startBoard = {
        {1, 2, 3},
        {4, 0, 6},
        {7, 5, 8}
    };

    State start(startBoard);

    cout << "Initial State:\n";
    start.print();

    bfs(start);
    cout << endl;
    dfs(start, 20); // Limit DFS depth to prevent infinite loops
    return 0;
}
