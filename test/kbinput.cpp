#include "kbinput.hpp"

KbInput::KbInput()
{
	_in = open(_dev, O_RDONLY);
	disable_echo();	
}

KbInput::~KbInput()
{
	close(_in);
	enable_echo();
}

void KbInput::disable_echo(void)
{
	tcgetattr(fileno(stdin), &_term);
	_term.c_lflag &= ~ECHO;
	tcsetattr(fileno(stdin), 0, &_term);	
}

void KbInput::enable_echo(void)
{
	_term.c_lflag |= ECHO;
	tcsetattr(fileno(stdin), 0, &_term);
}

char KbInput::handler(void)
{
	read(_in, &_ev, sizeof(_ev));
	if (_ev.type == EV_KEY && _ev.value == 0)
	{
		switch(_ev.code)
		{
			case 1:
				return -127;
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
	return -1;
}
