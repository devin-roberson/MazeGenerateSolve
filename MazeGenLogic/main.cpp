#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "maze.h"

int main(int argc, char *argv[]) {
  srand(time(0));
  if(argc > 1) {
    int n = std::atoi(argv[1]);
    if(n > 2) {
      maze maze(n);
      maze.generate();
      maze.printMaze();
      std::cout << "\n";
      std::vector<std::pair<int,int>> path = maze.solveMaze();
      for (int i = 0; i < path.size(); i++) {
         std::cout << std::dec << "(" << path[i].first << ", " << path[i].second << ")" << "\n";
      }
    }
    
  }
}
