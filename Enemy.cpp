#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Headers/Animation.hpp"
#include "Headers/Global.hpp"
#include "Headers/MapManager.hpp"
#include "Headers/Mushroom.hpp"
#include "Headers/Mario.hpp"
#include "Headers/Enemy.hpp"

Enemy::Enemy(const float j, const float k) :
	death(0),
	side_motion(0),
	up_motion(0),
	x(j),
	y(k)
{
	
}


bool Enemy::set_death(const bool m) const
{
	return death;
}


void Enemy::die(const unsigned char mdead)
{
	death = 1;
}


sf::FloatRect Enemy::hitting_box() const
{
	return sf::FloatRect(x, y, CELL_DIMENSION, CELL_DIMENSION);
}