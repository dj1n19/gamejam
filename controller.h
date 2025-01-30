#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "view.h"
#include <vector>
#include <string>
#include <algorithm>
#include <hidapi/hidapi.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

#define VID 0x046d  // Vendor ID
#define PID 0xc218  // Product ID

class Controller {

	private:

		struct Position {
			unsigned int row;
			unsigned int col;
		};

		unsigned int playerX, playerY;
		unsigned int collectedItems = 0;
		unsigned int points;
		unsigned int fuel;

		const std::string mapPath;

public:
    Controller();
    void loadLevel(const std::string& filename);
    void restorePreviousPosition(int oldX, int oldY);
    void movePlayer(char direction);
    void interact();
    void startGame();
    void writeToSpecificLine(const std::string& fileName, int lineNumber, const std::string& newContent);
    std::string floatToString(float value, int precision = 2);
    std::string convertLight(int value);
    std::string distanceToItem(double distance);


private:
    std::vector<std::vector<char>> level;
    std::vector<std::vector<bool>> collected;  // Add this line to track collected items
    std::vector<char> forbidden = {'2', 'B'};
    std::vector<int> allowed_keys = {0X00,0X02,0X04,0X06,0X18};
    int playerX, playerY;
    int collectedItems = 0;
    int points = 0;
    int fuel = 100;
    std::string fileName ="config.txt";
};

#endif
