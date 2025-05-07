#ifndef DISJOINTSET_H
#define DISJOINTSET_H

#include <vector>

class disjointSet {
private:
    std::vector<int> parent;
    std::vector<int> rank;
    int numSet;
    void link(int x, int y);        // link two items into the same set

public:
    disjointSet(int n);
    int findParent(int x);          // finds root of set x is in
    void unionSet(int x, int y);    // public function for unioning into same set
    int getSize();
};

#endif 