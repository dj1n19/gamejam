#pragma once
#include <linux/input.h>
#include <termios.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

class KbInput
{
	private:

		const char *_dev = "/dev/input/event0";
		input_event _ev;
		int _in;
		termios _term;

		void disable_echo(void);
		void enable_echo(void);


	public:

		KbInput();
		~KbInput();

		char handler(void);
};
