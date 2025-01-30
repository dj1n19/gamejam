#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

using namespace std;

// Function to handle the movement logic
void move(vector<vector<char>>& level, int& x, int& y, char direction) {
    int newX = x;
    int newY = y;

    if (direction == 'z') {
        newY--; // Move up
    } else if (direction == 's') {
        newY++; // Move down
    } else if (direction == 'q') {
        newX--; // Move left
    } else if (direction == 'd') {
        newX++; // Move right
    } else if (direction != 'e') {
        cout << "Invalid input. Please use z, q, s, d to move or e to exit.\n";
        return;
    }

    // Check if the new position is within bounds
    if (newY >= 0 && newY < level.size() && newX >= 0 && newX < level[0].size()) {
        // Check if the new position is not a wall ('#') or a forbidden tile ('1')
        if (level[newY][newX] == '1') {
            cout << "Move error: Cannot move onto tile '1'.\n";
        } else if (level[newY][newX] != '#') {
            // Update the player's position on the map
            level[y][x] = ' ';  // Replace old position with a blank space
            x = newX;
            y = newY;
            level[y][x] = 'X';  // Place the player in the new position
        } else {
            cout << "Cannot move there. It's a wall (#).\n";
        }
    } else {
        cout << "Cannot move there. It's out of bounds.\n";
    }
}

int main() {
    // Open the file
    ifstream file("level_1.txt");

    if (!file) {
        cout << "Error opening file." << endl;
        return 1;
    }

    vector<vector<char>> level;  // 2D vector to store the level
    string line;
    int playerX = -1, playerY = -1;

    // Read each line of the file and store it in the level array
    while (getline(file, line)) {
        vector<char> row;
        for (char ch : line) {
            row.push_back(ch);
            if (ch == 'X') {
                playerX = row.size() - 1;  // Set the X coordinate of the player
                playerY = level.size();   // Set the Y coordinate of the player
            }
        }
        level.push_back(row);
    }

    file.close();  // Close the file

    // Check if the player was found
    if (playerX == -1 || playerY == -1) {
        cout << "Player not found in the level.\n";
        return 1;
    }

    // Main game loop
    char moveInput;
    while (true) {
        // Display the level
        cout << "Level:\n";
        for (const auto& row : level) {
            for (char ch : row) {
                cout << ch;
            }
            cout << "\n";
        }

        // Print the player's position
        cout << "Player is at (" << playerX << ", " << playerY << ")\n";

        // Ask for user input
        cout << "Move (z = up, q = left, s = down, d = right, e = exit): ";
        cin >> moveInput;

        if (moveInput == 'e') {
            cout << "Exiting the game.\n";
            break;
        }

        // Call the move function
        move(level, playerX, playerY, moveInput);

        // Update the first line of ../config.txt
        fstream configFile("../config.txt", ios::in | ios::out);
        if (configFile) {
		printf("config file OPEN");
            // Read all lines into a vector
            vector<string> lines;
            string currentLine;
            while (getline(configFile, currentLine)) {
                lines.push_back(currentLine);
            }

            // Modify the first line
            if (!lines.empty()) {
                lines[0] = to_string(playerX) + "," + to_string(playerY);
            } else {
                lines.push_back(to_string(playerX) + "," + to_string(playerY));
            }

            // Move back to the beginning and overwrite the file
            configFile.clear();
            configFile.seekp(0);
            for (const string& updatedLine : lines) {
                configFile << updatedLine << "\n";
            }

            configFile.close();
        } else {
            cout << "Error: Could not open ../config.txt for writing.\n";
        }
    }

    return 0;
}

