#pragma once

//I used the keyword "this", which returns the memory address of the object. But since I switched to smart pointers, this class must inherit std::enable_shared_from_this<Enemy>. C++ is weird.
#include "MapManager.hpp"

#include "Mario.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
class Enemy : public std::enable_shared_from_this<Enemy>
{
protected:
    float x;
	
	float y;

	bool death;

	float side_motion;

	float up_motion;


public:
	Enemy(const float j, const float k);

	virtual void die(const unsigned char mdead);

	virtual bool set_death(const bool m) const;

	
	
	virtual void draw(const unsigned x, sf::RenderWindow& window) = 0;
	
	virtual void update(const unsigned ix, const std::vector<std::shared_ptr<Enemy>>& enemy, const MapManager& i_map_manager, Mario& i_mario) = 0;

	sf::FloatRect hitting_box() const;
};
