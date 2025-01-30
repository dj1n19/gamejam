#include "map.hpp"

Map::Map() {}

Map::Map(const std::string& path): _path(path)
{
	_if.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	openMapFile();
	setStart();
}

Map::~Map() {}

void Map::openMapFile(void)
{
	try
	{
		_if.open(_path);
		loadMap();
		_if.close();
	}
	catch (const std::ifstream::failure& e)
	{
		std::cerr << "\e[31mError in " << __FILE__ << ": " << __func__ << " line " << __LINE__ << " => " << e.what() << "\e[0m" << std::endl;
	}
}

void Map::loadMap(void)
{
	do
	{
		std::string line;
		try
		{
			std::getline(_if, line);
			std::vector<char> row;
			for (char c : line)
				row.push_back(c);
			_map.push_back(row);
		}
		catch (const std::ifstream::failure& e)
		{
			std::cerr << "\e[31mError in " << __FILE__ << ": " << __func__ << " line "<< __LINE__ << " => " << e.what() << "\e[0m" << std::endl;
		}
	}
	while (!_if.eof());
}

void Map::setStart(void)
{
	for (size_t y = 0; y < _map.size(); y++)
	{
		for (size_t x = 0; x < _map[x].size(); x++)
		{
			if (_map[y][x] == 'X')
			{
				_startX = x;
				_startY = y;
				return ;
			}
		}
	}	
}

void Map::display(void) const
{
	for (auto colIt = _map.begin(); colIt != _map.end(); colIt++)
	{
		for (auto rowIt = colIt->begin(); rowIt != colIt->end(); rowIt++)
			std::cout << *rowIt;
		std::cout << std::endl;
	}
}

void Map::printStart(void) const
{
	std::cout << "Start: (" << _startX << "," << _startX << ")" << std::endl;
}

char Map::getChar(unsigned int x, unsigned int y) const
{
		return _map[y][x];
}

void Map::setChar(unsigned int x, unsigned int y, char c)
{
	_map[y][x] = c;
}
