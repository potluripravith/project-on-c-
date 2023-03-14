
#include <SFML/Graphics.hpp>

#include "Headers/Animation.hpp"

Animation::Animation(const unsigned short mFrame_breadth, const std::string& mtexture_position, const unsigned short forani_speed) :

	flip(0),

	ani_repeat(0),

	present_frame(0),
	
	frame_breadth(mFrame_breadth)

{
	for_ani_speed(forani_speed);

	for_texture_position(mtexture_position);
}

void Animation::draw(sf::RenderWindow& screen)
{
	sprite.setTexture(texture);
     switch (flip) {
		case 0:
			sprite.setTextureRect(sf::IntRect(present_frame * frame_breadth, 0, frame_breadth, texture.getSize().y));
			break;
		case 1:
			sprite.setTextureRect(sf::IntRect(frame_breadth * (1 + present_frame), 0, -frame_breadth, texture.getSize().y));
			break;
	}

	screen.draw(sprite);
}
void Animation::for_flip(const bool z)
{
  flip = z;
}


void Animation::for_ani_speed(const unsigned short forani_speed)
{
   ani_speed = std::max<unsigned short>(1, forani_speed);
}



void Animation::for_place(const short j, const short k)
{
   sprite.setPosition(j, k);
}


void Animation::for_texture_position(const std::string& mtexture_position)
{
	texture.loadFromFile(mtexture_position);
	totFrames = texture.getSize().x / frame_breadth;
}

void Animation::update()
{
	ani_repeat++;
	if (ani_repeat >= ani_speed) {
		ani_repeat = 0;
		present_frame = (present_frame + 1) % totFrames;
	}
}
