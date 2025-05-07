#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <utility>

class maze {
private:
    std::vector<int> _maze; 
    std::vector<std::pair<int, int>> walls;     
    int n;                                      // # of rows/cols
    int size;                                   // n x n
    const int RIGHT_WALL = 1;                   // right wall mapped to 0001
    const int BOTTOM_WALL = 2;                  // bottom wall mapped to 0010
    const int LEFT_WALL = 4;                    // left wall mapped to 0100
    const int TOP_WALL = 8;                     // top wall mapped to 1000
    
    void removeWall(int src, int adj);              // removes a wall between two cells
    void initializeWalls();                         // generate list of all valid walls between adjacent cells
    std::vector<int> getNeighbors(int cell);        // gets neighbors of cell with no walls
    std::pair<int, int> toCoords(int cell);         // converts cell into coordinates - (row,col)
    void generate();                                // generates a random valid maze
    std::vector<std::pair<int, int>> solveMaze();   // solves the maze using Dijkstra's Algorithm

public:
    maze(int n);
    void GenerateMazeAndSolution();            // outputs the maze itself and the solution coordinates
};

#endif
