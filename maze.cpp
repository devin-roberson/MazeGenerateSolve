#include <random>
#include <algorithm>
#include <iostream>

#include "maze.h"
#include "disjointSet.h"

maze::maze(int n) {
    this->n = n;
    size = n * n;
    _maze.resize(size, 15);         // each cell starts with all 4 walls (1111 = 15)
    _maze[0] -= leftWall;           // entrance - no left wall
    _maze[size - 1] -= rightWall;   // exit - no right wall

    // generate list of all valid walls between adjacent cells (right and bottom)
    for (int i = 0; i < size; i++) {
        int row = i / n;
        int col = i % n;

        if (col < n - 1)                    // right neighbor exists
            walls.emplace_back(i, i + 1);
        if (row < n - 1)                    // bottom neighbor exists
            walls.emplace_back(i, i + n);
    }

    // shuffle walls vector for random generation order
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(walls.begin(), walls.end(), g);
}

void maze::removeWall(int src, int adj) {
    if (adj == src + 1) {                   // right adjacent
        _maze[src] &= ~rightWall;           // remove right wall from src
        _maze[adj] &= ~leftWall;            // remove left wall from adj
    } else if (adj == src - 1) {            // left adjacent
        _maze[src] &= ~leftWall;
        _maze[adj] &= ~rightWall;
    } else if (adj == src + n) {            // bottom adjacent
        _maze[src] &= ~bottomWall;
        _maze[adj] &= ~topWall;
    } else if (adj == src - n) {            // top adjacent
        _maze[src] &= ~topWall;
        _maze[adj] &= ~bottomWall;
    }
}

void maze::generate() {
    disjointSet ds(size);
    for (auto& [a, b] : walls) {
        if (ds.findParent(a) != ds.findParent(b)) {
            removeWall(a, b);
            ds.unionSet(a, b);
        }
    }
}

void maze::printMaze() {
    for (int i = 0; i < size; i++) {
        if (i % n == 0 && i > 0)
            std::cout << '\n';
        std::cout << std::hex << _maze[i];
    }
    std::cout << std::endl;
}
