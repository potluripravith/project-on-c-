#pragma once

#include "Animation.hpp"
#include "Enemy.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
class Goomba : public Enemy
{
	bool map2;

	bool isDyingFromCollision;
	

	unsigned short death_duration;

	sf::Sprite sprite;

	sf::Texture texture;

	Animation walk_animation;

public:
	Goomba(const bool mmap2, const float j, const float k);

	void die(const unsigned char mdead);

	bool set_death(const bool m) const;

	void draw(const unsigned x, sf::RenderWindow& window);
	
	void update(const unsigned ix, const std::vector<std::shared_ptr<Enemy>>& Enemy, const MapManager& i_map_manager, Mario& i_mario);
};
