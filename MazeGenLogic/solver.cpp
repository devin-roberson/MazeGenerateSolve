#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <limits>
#include <cmath>

#include "maze.h"

using namespace std;

// wall mapping (maze.h)
const int RIGHT_WALL = 1;   // 0001
const int BOTTOM_WALL = 2;  // 0010
const int LEFT_WALL = 4;    // 0100
const int TOP_WALL = 8;     // 1000

// class solving a maze using Dijkstra's algorithm
class Solver {
private:
    vector<int> maze;       // maze (wall configuration)
    int size;               // size of maze (n for nxn)
    int cells;              // number of cells (n*n)

    // analyze maze hex string from generator outpuut
    bool scanMaze(const string& mazeStr) {
        maze.clear();                           // clear any existing maze data

        size = sqrt(mazeStr.length());          // get maze size

        // check square error handler
        if (size * size != mazeStr.length()) {
            cout << "Error: Maze input length is not a perfect square." << endl;
            return false;
        }

        cells = size * size;

        // convert hex from printMaze() to decimal
        for (char c : mazeStr) {
            int value;
            if (c >= '0' && c <= '9') {
                value = c - '0';
            }
            else if (c >= 'a' && c <= 'f') {
                value = c - 'a' + 10;
            }
            else if (c >= 'A' && c <= 'F') {
                value = c - 'A' + 10;
            }
            else {
                cout << "Error: Invalid hex character in maze: " << c << endl;
                return false;
            }
            maze.push_back(value);
        }

        return true;
    }

    // find neighboring cells with no walls
    vector<int> getNeighbors(int cell) {
        vector<int> neighbors;
        int row = cell / size;      // cell / size n (for nxn) discarding remainder
        int col = cell % size;      // gives remainder for column

        // right neighbor
        if (col < size - 1 && !(maze[cell] & RIGHT_WALL)) {
            neighbors.push_back(cell + 1);
        }

        // left neighbor
        if (col > 0 && !(maze[cell] & LEFT_WALL)) {
            neighbors.push_back(cell - 1);
        }

        // top neighbor
        if (row > 0 && !(maze[cell] & TOP_WALL)) {
            neighbors.push_back(cell - size);
        }

        // bottom neighbor
        if (row < size - 1 && !(maze[cell] & BOTTOM_WALL)) {
            neighbors.push_back(cell + size);
        }

        return neighbors;
    }

    // converts cells to coordinates (row, col) for visualization
    pair<int, int> getCellCoordinates(int cell) {
        int row = cell / size;
        int col = cell % size;
        return make_pair(row, col);
    }

public:
    Solver() : size(0), cells(0) {}             // initialize maze
    bool loadMaze(const string& mazeStr) {      // load maze using hex
        return scanMaze(mazeStr);
    }

    int getMazeSize() const {                   // get size
        return size;
    }

    // solve using Dijkstra's function
    vector<int> solveMaze() {
        int start = 0;
        int end = cells - 1;

        vector<int> distance(cells, numeric_limits<int>::max());        // tracks shortest distance from start to each cell
        vector<int> previous(cells, -1);                                // store previous cell in shortest path
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // priority queue for shortest distance

        distance[start] = 0;            // start at beginning
        pq.push(make_pair(0, start));   // add distance (0) to pq

        // Dijkstra's algorithm loop
        while (!pq.empty()) {                       
            int currentCell = pq.top().second;      // cell identifier for smallest distance
            int currentDist = pq.top().first;       // cell distance
            pq.pop();                               // remove from queue

            if (currentCell == end) {               // if exit, done
                break;
            }

            if (currentDist > distance[currentCell]) {  // maintains shortest path
                continue;
            }

            // check accessible neighbors
            for (int neighbor : getNeighbors(currentCell)) {
                int newDist = distance[currentCell] + 1;        // calculate new distance with current cell

                // if shorter path found,
                if (newDist < distance[neighbor]) {
                    distance[neighbor] = newDist;               // update neighbor distance
                    previous[neighbor] = currentCell;           // set current cell as neighbor's predecessor
                    pq.push(make_pair(newDist, neighbor));      // add neighbor to pq
                }
            }
        }
// Summary: visit cell and find shortest path from it. If theres a shorter path, then update, repeat until reach the end

        // reconstruct path using previous array starting at end
        vector<int> path;
        for (int at = end; at != -1; at = previous[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());      // reverse path to get order
        return path;
    }

    // get coordinates by converting cells to (row, col)
    vector<pair<int, int>> getPathCoordinates(const vector<int>& path) {
        vector<pair<int, int>> coordinates;

        for (int cell : path) {
            coordinates.push_back(getCellCoordinates(cell));
        }
        return coordinates;
    }

    // print table showing each step using cell index and coordinate
    void printSolutionPath(const vector<int>& path) {
        cout << "Solution path:" << endl;
        cout << "----------------" << endl;
        cout << "Step\tCell\t(Row, Col)" << endl;

        for (int i = 0; i < path.size(); i++) {
            int cell = path[i];
            pair<int, int> coords = getCellCoordinates(cell);

            cout << i << "\t" << cell << "\t(" << coords.first << ", " << coords.second << ")" << endl;
        }

        cout << endl << "Path length: " << path.size() << " cells" << endl;
    }

    // print solved maze with path
    void printMazeWithPath(const vector<int>& path) {
        if (maze.empty()) {
            cout << "No maze loaded." << endl;
            return;
        }

        // mark cells on the path
        vector<bool> isOnPath(cells, false);
        for (int cell : path) {
            isOnPath[cell] = true;
        }

        // top border
        cout << "+";
        for (int j = 0; j < size; j++) {
            cout << "---+";
        }
        cout << endl;

        // print by row (i =  row, j = column)
        for (int i = 0; i < size; i++) {
            cout << "|";                        // left border
            for (int j = 0; j < size; j++) {
                int cell = i * size + j;
                if (isOnPath[cell]) {           // print cell with path
                    cout << " * ";
                }
                else {
                    cout << "   ";              // blank for no path
                }
                if (maze[cell] & RIGHT_WALL) {
                    cout << "|";                // close right wall if exists
                }
                else {
                    cout << " ";                // otherwise leave open if no wall
                }
            }
            cout << endl;

            // bottom wall
            cout << "+";
            for (int j = 0; j < size; j++) {
                int cell = i * size + j;            // calculate cell index from 2D coordinates
                if (maze[cell] & BOTTOM_WALL) {
                    cout << "---";
                }
                else {
                    cout << "   ";
                }
                cout << "+";
            }
            cout << endl;
        }
    }
};
// main function, check valid maze command
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <maze_hex_string>" << endl;
        cout << "Example: " << argv[0] << " fed9ba9d9fca8e9ccafacf9a" << endl;
        return 1;
    }
    string mazeStr = argv[1];       // get maze string from command line

    // create solver and load maze
    Solver solver;
    if (!solver.loadMaze(mazeStr)) {
        cout << "Error loading maze." << endl;
        return 1;
    }

    // run solve
    cout << "Solving maze using Dijkstra's algorithm." << endl;
    vector<int> path = solver.solveMaze();

    if (path.empty()) {
        cout << "No solution found!" << endl;
        return 1;
    }

    solver.printMazeWithPath(path);         // print solution path
    solver.printSolutionPath(path);         // print solution coordinates
    return 0;
}