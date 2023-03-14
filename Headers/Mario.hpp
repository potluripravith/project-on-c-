#pragma once

class Mario
{
	bool crouching;

	bool death;

	bool flipped;

	bool ground;

	bool still_alive ;


	float side_motion;

	float up_motion;

	
	float enemy_bounce_speed;


	float x;

	float y;

	//The longer we press the jump button, the higher Mario jumps. And we use this variable for that.
	unsigned char jump_duration;
	
	unsigned char powerup_state;

	unsigned short invincible_timer;

	unsigned short death_duration;

	unsigned short growth_timer;

	

	std::vector<Mushroom> mushrooms;

	sf::Sprite sprite;

	sf::Texture texture;

	Animation big_walk_animation;

	Animation walk_animation;
public:
	Mario();
    void die(const bool i_death);
    void draw(sf::RenderWindow& window);
    void draw_mushrooms(const unsigned view, sf::RenderWindow& window);
    void reset();
    void set_location(const float j, const float k);
    void set_up_motion(const float i_value);
    void update(const unsigned veiw, MapManager& i_map_manager);

    bool set_death() const;

    float set_up_motion() const;
    float set_x() const;

    sf::FloatRect hitting_box() const;
};