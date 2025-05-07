#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <utility>

class maze {
private:
    std::vector<int> _maze; 
    std::vector<std::pair<int, int>> walls;
    int n;                          // # of rows/cols
    int size;                       // n x n
    const int RIGHT_WALL = 1;       // right wall mapped to 0001 / 2^0
    const int BOTTOM_WALL = 2;      // bottom wall mapped to 0010 / 2^1
    const int LEFT_WALL = 4;        // left wall mapped to 0100 / 2^2
    const int TOP_WALL = 8;         // top wall mapped to 1000 / 2^3
    
    void removeWall(int src, int adj);          // removes a wall between two cells
    std::vector<int> getNeighbors(int cell);    // gets valid neighbors of a cell
    std::pair<int, int> toCoords(int cell);     // converts cell into coordinates - (row,col)

public:
    maze(int n);
    void generate();
    void printMaze();
    std::vector<std::pair<int,int>> solveMaze();
};

#endif
