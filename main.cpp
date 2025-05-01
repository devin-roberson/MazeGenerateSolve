#include <time.h>
#include <stdlib.h>
#include "maze.h"

int main(int argc, char *argv[]) {
  srand(time(0));
  if(argc > 1) {
    int n = std::atoi(argv[1]);
    if(n > 2) {
      maze maze(n);
      maze.generate();
      maze.printMaze();
    }
  }
}
