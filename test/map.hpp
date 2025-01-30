#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>

class Map
{
	private:

		const std::string _path;
		std::ifstream _if;
		std::vector<std::vector<char>> _map;
		unsigned int _startX;
		unsigned int _startY;

		void openMapFile(void);
		void loadMap(void);
		void setStart(void);

	public:

		Map();
		Map(const std::string& path);
		~Map();

		void display(void) const;
		void printStart(void) const;
		char getChar(unsigned int x, unsigned int y) const;
		void setChar(unsigned int x, unsigned int y, char c);
};
