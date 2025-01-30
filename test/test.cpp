#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

/*
 * KEY_ESC = 1;
 * KEY_W = 17
 * KEY_A = 30
 * KEY_S = 31
 * KEY_D = 32
 */

termios term;

void disable_echo()
{
	tcgetattr(fileno(stdin), &term);
	term.c_lflag &= ~ECHO;
	tcsetattr(fileno(stdin), 0, &term);
}

void enable_echo()
{
	term.c_lflag |= ECHO;
	tcsetattr(fileno(stdin), 0, &term);
}

char getKey(int code)
{
	switch(code)
	{
		case 17:
			return 'w';
		case 30:
			return 'a';
		case 31:
			return 's';
		case 32:
			return 'd';
		default:
			return -1;
	}
}

int main(void)
{
	char dev[] = "/dev/input/event0";	
	int fd = -1;
	input_event ev;

	fd = open(dev, O_RDONLY);
	disable_echo();

	while (1)
	{
		read(fd, &ev, sizeof(ev));
		if (ev.type == EV_KEY && ev.value == 0)
		{
			if (ev.code == 1)
				break ;
			std::cout << getKey(ev.code) << std::endl;
		}
	}
	
	close(fd);
	enable_echo();

	return 0;
}
