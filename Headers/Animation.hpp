#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
class Animation
{   
	unsigned short present_frame;

	unsigned short frame_breadth;

	unsigned short totFrames;
	
	bool flip;

	//We'll increment the current_frame as soon as this iterator reaches the animation_speed.
	unsigned short ani_repeat;
	
	unsigned short ani_speed;
	
	

	sf::Sprite sprite;

	sf::Texture texture;
public:

	Animation(const unsigned short mFrame_breadth, const std::string& mtexture_position, const unsigned short forani_speed = 1);

    void for_flip(const bool z);

	void draw(sf::RenderWindow& screen);
	
	
	void for_place(const short j, const short k);

	void for_texture_position(const std::string& mtexture_position);

	void for_ani_speed(const unsigned short forani_speed);

	void update();

};
