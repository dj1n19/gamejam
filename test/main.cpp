#include "map.hpp"
#include "kbinput.hpp"

int main(void)
{
	const std::string path("map.txt");
	Map map(path);
	KbInput kbinput;

	map.display();
	map.printStart();

	while (1)
	{
		char input = kbinput.handler();
		if (input == -127)
			break ;
		if (input > 0)
			std::cout << "INPUT: " << input << std::endl;	
	}

	return 0;
}
