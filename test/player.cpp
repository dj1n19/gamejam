#include "player.hpp"

Player::Player() {}

Player::Player(unsigned int x, unsigned int y): _x(x), _y(y) {}

Player::~Player() {}

void Player::move(unsigned int x, unsigned int y)
{
	_x = x;
	_y = y;
}

unsigned int Player::getFuel(void) const
{
	return _fuel;
}

void Player::setFuel(unsigned int fuel)
{
	_fuel = fuel;
}
