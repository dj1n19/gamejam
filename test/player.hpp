#pragma once

class Player
{
	private:

		unsigned int _x;
		unsigned int _y;
		unsigned int _fuel;

	public:

		Player();
		Player(unsigned int x, unsigned int y);
		~Player();

		void move(unsigned int x, unsigned int y);

		unsigned int getFuel(void) const;
		void setFuel(unsigned int fuel);
};
