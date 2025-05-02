#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <utility>

class maze {
private:
    std::vector<int> _maze; 
    std::vector<std::pair<int, int>> walls;
    int n; 
    int size; 
    int rightWall = 1;       // right wall mapped to 0001 / 2^0
    int bottomWall = 2;      // bottom wall mapped to 0010 / 2^1
    int leftWall = 4;        // left wall mapped to 0100 / 2^2
    int topWall = 8;         // top wall mapped to 1000 / 2^3
    
    void removeWall(int src, int adj);

public:
    maze(int n);
    void generate();
    void printMaze();
};

#endif
