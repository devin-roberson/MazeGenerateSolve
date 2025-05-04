#include <random>
#include <algorithm>
#include <iostream>
#include <queue>
#include <limits>
#include <cmath>

#include "maze.h"
#include "disjointSet.h"

maze::maze(int n) {
    this->n = n;
    size = n * n;
    _maze.resize(size, 15);         // each cell starts with all 4 walls (1111 = 15)
    _maze[0] -= LEFT_WALL;           // entrance - no left wall
    _maze[size - 1] -= RIGHT_WALL;   // exit - no right wall

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
        _maze[src] &= ~RIGHT_WALL;           // remove right wall from src
        _maze[adj] &= ~LEFT_WALL;            // remove left wall from adj
    } else if (adj == src - 1) {            // left adjacent
        _maze[src] &= ~LEFT_WALL;
        _maze[adj] &= ~RIGHT_WALL;
    } else if (adj == src + n) {            // bottom adjacent
        _maze[src] &= ~BOTTOM_WALL;
        _maze[adj] &= ~TOP_WALL;
    } else if (adj == src - n) {            // top adjacent
        _maze[src] &= ~TOP_WALL;
        _maze[adj] &= ~BOTTOM_WALL;
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

// find neighboring cells with no walls
std::vector<int> maze::getNeighbors(int cell) {
    std::vector<int> neighbors;
    int row = cell / size;      // cell / size n (for nxn) discarding remainder
    int col = cell % size;      // gives remainder for column

    // right neighbor
    if (col < size - 1 && !(_maze[cell] & RIGHT_WALL)) {
        neighbors.push_back(cell + 1);
    }

    // left neighbor
    if (col > 0 && !(_maze[cell] & LEFT_WALL)) {
        neighbors.push_back(cell - 1);
    }

    // top neighbor
    if (row > 0 && !(_maze[cell] & TOP_WALL)) {
        neighbors.push_back(cell - size);
    }

    // bottom neighbor
    if (row < size - 1 && !(_maze[cell] & BOTTOM_WALL)) {
        neighbors.push_back(cell + size);
    }

    return neighbors;
}

std::vector<int> maze::solveMaze() {
    int start = 0;
    int end = size - 1;

    std::vector<int> distance(size, std::numeric_limits<int>::max());        // tracks shortest distance from start to each cell
    std::vector<int> previous(size, -1);                                // store previous cell in shortest path
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq; // priority queue for shortest distance

    distance[start] = 0;            // start at beginning
    pq.push(std::make_pair(0, start));   // add distance (0) to pq

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
                pq.push(std::make_pair(newDist, neighbor));      // add neighbor to pq
            }
        }
    }
// Summary: visit cell and find shortest path from it. If theres a shorter path, then update, repeat until reach the end

    // reconstruct path using previous array starting at end
    std::vector<int> path;
    for (int at = end; at != -1; at = previous[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());      // reverse path to get order
    return path;
}