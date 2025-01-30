#ifndef VIEW_H
#define VIEW_H

#include <vector>
#include <string>
using namespace std;

class View {
public:
    void displayLevel(const vector<vector<char>>& level, int playerX, int playerY, int collectedItems, int points, int fuel);
};

#endif
