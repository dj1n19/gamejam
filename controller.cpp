#include "controller.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>

using namespace std;


struct Position {
    size_t row;
    size_t col;
};

// Function to calculate the Euclidean distance between two points
double calculateDistance(int x1, int y1, int x2, int y2) {
    return std::sqrt(std::pow(static_cast<double>(x2 - x1), 2) + 
                     std::pow(static_cast<double>(y2 - y1), 2));
}

// Function to find the closest '2' to the player's position
double findClosest2ToPlayer(const std::vector<std::vector<char>>& table, int playerX, int playerY) {
    // Check grid boundaries
    if (playerY < 0 || playerY >= table.size() || playerX < 0 || playerX >= table[0].size()) {
        std::cerr << "Player's position is out of bounds." << std::endl;
        return -1.0;
    }

    // Find all '2' positions and calculate the closest distance
    double minDistance = std::numeric_limits<double>::max();
    for (size_t i = 0; i < table.size(); ++i) {
        for (size_t j = 0; j < table[i].size(); ++j) {
            if (table[i][j] == '2') {
                double distance = calculateDistance(playerX, playerY, static_cast<int>(j), static_cast<int>(i));
                minDistance = std::min(minDistance, distance);
            }
        }
    }

    // If no '2' is found, return -1
    if (minDistance == std::numeric_limits<double>::max()) {
        std::cerr << "No '2' found in the table." << std::endl;
        return -1.0;
    }

    return minDistance;
}

std::string Controller::distanceToItem(double distance){
    if(distance >= 20){
        return "0.1";
    }else if (distance >= 10){
        return "5.1";
    }else{
        return "10.0";
    }
}


Controller::Controller() : playerX(-1), playerY(-1), collectedItems(0) {}

void Controller::loadLevel(const string& filename) {
    ifstream file(filename);

    if (!file) {
        cout << "Error opening file." << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        vector<char> row;
        for (char ch : line) {
            row.push_back(ch);
            if (ch == 'X') {
                playerX = row.size() - 1; // Set the X coordinate of the player
                playerY = level.size();  // Set the Y coordinate of the player
            }
        }
        level.push_back(row);
    }

    file.close();

    if (playerX == -1 || playerY == -1) {
        cout << "Player not found in the level." << endl;
        exit(1);
    }

    // Initialize the collected items grid
    collected.resize(level.size(), vector<bool>(level[0].size(), false));
    // Mark collected items by default in the `collected` grid
    for (int y = 0; y < level.size(); ++y) {
        for (int x = 0; x < level[y].size(); ++x) {
            if (level[y][x] == '2') {
                collected[y][x] = false;  // Initially, the items are not collected
            }
        }
    }
}

void Controller::movePlayer(char direction) {
    // Helper function for updating coordinates
    auto move = [](int& x, int& y, char direction) {
        switch (direction) {
            case 0X00: y--; break; // Move up
            case 0X04: y++; break; // Move down
            case 0X06: x--; break; // Move left
            case 0X02: x++; break; // Move right
            default: cout << "Invalid input. Please use z, q, s, d to move, i to interact, or e to exit.\n"; return;
        }
    };

    int newX = playerX, newY = playerY;

    // If invalid direction
    /*if (direction != 'z' && direction != 's' && direction != 'q' && direction != 'd' && direction != 'e' && direction != 'i') {
        cout << "Invalid input. Please use z, q, s, d to move, i to interact, or e to exit.\n";
        return;
    }*/

    // Move player based on input
    move(newX, newY, direction);

    // Check for boundary conditions
    if (newY < 0 || newY >= level.size() || newX < 0 || newX >= level[0].size()) {
        cout << "Cannot move there. It's out of bounds.\n";
        return;
    }

    // Check if the new position is a wall
    if (level[newY][newX] == '#') {
        cout << "Cannot move there. It's a wall (#).\n";
        return;
    }

    // Restore the previous position's original value if needed
    restorePreviousPosition(playerX, playerY);

    // Move player to the new position
    playerX = newX;
    playerY = newY;
    std::ostringstream positions;
    positions << std::setfill('0') << std::setw(2) << playerX
       << std::setfill('0') << std::setw(2) << playerY;
    writeToSpecificLine(fileName, 1, positions.str());
    // Place the player on top of the current position
    if (std::find(forbidden.begin(), forbidden.end(), level[playerY][playerX]) == forbidden.end()) {
        level[playerY][playerX] = 'X'; // Display player on the new position
    }
    fuel --;
    std::string lightValue = convertLight(fuel);
    writeToSpecificLine(fileName, 2, lightValue);
    double closestDistance = findClosest2ToPlayer(level, playerX, playerY);
    
    std::string value = distanceToItem(closestDistance);
    writeToSpecificLine(fileName, 3, value);

    //writeToSpecificLine(fileName, 6, "The closest position of a 2 is:"+std::to_string(distanceToItem(closestDistance)));
    if (level[playerY][playerX] == 'B'){
        writeToSpecificLine(fileName, 4, "1");
    }else{
        writeToSpecificLine(fileName, 4, "0");
        
    }

}

void Controller::restorePreviousPosition(int oldX, int oldY) {
    if (level[oldY][oldX] == 'X') {
        level[oldY][oldX] = ' ';
    } else if (!collected[oldY][oldX] && level[oldY][oldX] == '2') {
        level[oldY][oldX] = '2';
    }
}


void Controller::interact() {
    if (level[playerY][playerX] == '2' && !collected[playerY][playerX]) {
        if (collectedItems == 0){
            collectedItems++;
            collected[playerY][playerX] = true;  // Mark the item as collected
            level[playerY][playerX] = 'X'; // Replace the item with player position
            cout << "Item collected! Total items collected: " << collectedItems << "\n";
            writeToSpecificLine(fileName, 3, "0.0");
        }else{
            cout << "Can't collect items! Total items collected: " << collectedItems << "\n";
        }
    }else if (level[playerY][playerX] == 'B'){
        if (collectedItems != 0){
            collectedItems = 0;
            points += 100;
            fuel = 100;
            //writeToSpecificLine(fileName, 5, std::to_string(points));
        }
        cout << "Item dropped! Total points: " << points << "\n";
    } else {
        cout << "No item to interact with here.\n";
    }
}

void Controller::startGame() {
    loadLevel("level_1.txt");
    View view;

    char input;
    if (hid_init()) {
        std::cerr << "Failed to initialize HIDAPI\n";
        
    }

    // Open the device using its VID and PID
    hid_device* handle = hid_open(VID, PID, nullptr);
    if (!handle) {
        std::cerr << "Unable to open HID device\n";

    }else {
        unsigned char data[64];
        int res;
        std::ostringstream positions;
        positions << std::setfill('0') << std::setw(2) << playerX
            << std::setfill('0') << std::setw(2) << playerY;
        writeToSpecificLine(fileName, 1, positions.str());
        writeToSpecificLine(fileName, 2, "100");
        writeToSpecificLine(fileName, 3, "10");
        writeToSpecificLine(fileName, 4, "1");
        //writeToSpecificLine(fileName, 4, "-0\n");
        //writeToSpecificLine(fileName, 5, "0\n");
        while (fuel > 0) {
            view.displayLevel(level, playerX, playerY, collectedItems,points,fuel);

            cout << "Move (z = up, q = left, s = down, d = right, i = interact, e = exit): ";
            res = hid_read(handle, data, sizeof(data));
            int hexValue = static_cast<int>(data[4]); 
            if (std::find(allowed_keys.begin(), allowed_keys.end(), hexValue) != allowed_keys.end()) {
                if (hexValue == 0x18) {
                    interact();
                } else {
                    movePlayer(hexValue);
                }
            }
        }
    }
    cout << "YOU HAVE NO MORE FUEL";
}


void Controller::writeToSpecificLine(const std::string& fileName, int lineNumber, const std::string& newContent) {
    std::vector<std::string> lines;
    std::string line;

    // Read all lines into a vector
    std::ifstream inputFile(fileName);
    if (!inputFile) {
        std::cerr << "Error: Unable to open file for reading.\n";
        return;
    }

    // Read lines and strip unnecessary empty or carriage return-only lines
    while (std::getline(inputFile, line)) {
        // Remove trailing carriage return or newline characters from each line
        if (!line.empty() && (line.back() == '\r' || line.back() == '\n')) {
            line = line.substr(0, line.size() - 1);
        }

        // Skip lines that are just empty (empty after stripping)
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    inputFile.close();

    // If the line number is greater than the current number of lines,
    // add empty lines until we reach the desired line number.
    while (lines.size() < lineNumber - 1) {
        lines.push_back("");  // Add empty lines to make space if needed
    }

    // Modify the specified line (lineNumber is 1-based)
    if (lineNumber <= lines.size()) {
        lines[lineNumber - 1] = newContent;  // Modify the existing line
    } else {
        lines.push_back(newContent);  // Add the new line if it's beyond the current size
    }

    // Print all lines (for debugging or verification)
    std::cout << "Current lines before writing to file:\n";
    for (const auto& l : lines) {
        std::cout << l << std::endl;
    }

    // Write the modified lines back to the file
    std::ofstream outputFile(fileName);
    if (!outputFile) {
        std::cerr << "Error: Unable to open file for writing.\n";
        return;
    }

    // Loop through the lines and write them back to the file
    for (size_t i = 0; i < lines.size(); ++i) {
        // For the last line, don't add a newline
        if (i == lines.size() - 1) {
            outputFile << lines[i];
        } else {
            // For all other lines, add a newline
            outputFile << lines[i] << "\n";
        }
    }

    outputFile.close();

    std::cout << "Line " << lineNumber << " updated successfully!\n";
}









std::string Controller::convertLight(int value){
    if (value / 10 >= 10) {
        return "0.1";
    } else if (value / 10 >= 9) {
        return "0.1";
    } else if (value / 10 >= 8) {
        return "2.1";
    } else if (value / 10 >= 7) {
        return "3.1";
    } else if (value / 10 >= 6) {
        return "4.1";
    } else if (value / 10 >= 5) {
        return "5.1";
    } else if (value / 10 >= 4) {
        return "6.1";
    } else if (value / 10 >= 3) {
        return "8.1";
    } else if (value / 10 >= 2) {
        return "9.1";
    } else if (value / 10 >= 1) {
        return "10";
    }else {
        return "10";
    }

}

std::string Controller::floatToString(float value, int precision) {
    std::ostringstream oss;
    oss.precision(precision);      // Set the precision
    oss << std::fixed << value;   // Use fixed-point notation
    return oss.str();
}


