#pragma once

class Koopa : public Enemy
{
	//We'll use this to make sure that we check the collision with Mario only once when he collides with us.
	bool collision_for;

	bool flipped;
	
	bool hitting_x;

	bool map2;

	unsigned char state;

	unsigned short get_out_timer;

	sf::Sprite sprite;

	sf::Texture texture;

	Animation get_out_animation;

	Animation walk_animation;
public:
	Koopa(const bool mmap2, const float i_x, const float i_y);
	
	void die(const unsigned char n);

	bool set_death(const bool m) const;

	
	void draw(const unsigned view, sf::RenderWindow& window);
	
	void update(const unsigned view, const std::vector<std::shared_ptr<Enemy>>& enemy, const MapManager& i_map_manager, Mario& i_mario);
};