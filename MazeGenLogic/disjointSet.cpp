#include "disjointSet.h"

disjointSet::disjointSet(int n) {
    parent.resize(n);
    rank.resize(n);
    numSet = n;
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
}

void disjointSet::link(int x, int y) {
    if (x == y)
        return;
    if (rank[x] > rank[y]) {
        parent[y] = x; 
    } else {
        parent[x] = y;
        if (rank[x] == rank[y])
            rank[y]++;
    }
    numSet--;
}

int disjointSet::findParent(int x) {
    if (x != parent[x]) {
        parent[x] = findParent(parent[x]);
    }
    return parent[x];
}

void disjointSet::unionSet(int x, int y) {
    link(findParent(x), findParent(y));
}

int disjointSet::getSize() {
    return numSet;
}