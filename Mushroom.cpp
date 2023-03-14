#include <chrono>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Headers/Animation.hpp"
#include "Headers/Global.hpp"
#include "Headers/MapManager.hpp"
#include "Headers/Mushroom.hpp"

Mushroom::Mushroom(const float i_x, const float i_y) :
	
	x(i_x),
	y(i_y),
	start_y(i_y),
	death(0),
	side_direction(1),
	up_motion(0)
	
{
	texture.loadFromFile("Images/Mushroom.png");
}

void Mushroom::draw(const unsigned i_view_x, sf::RenderWindow& i_window)
{
	if (-CELL_DIMENSION < round(y) && round(x) > static_cast<int>(i_view_x) - CELL_DIMENSION && round(x) < SCREEN_WIDTH + i_view_x && round(y) < SCREEN_HEIGHT)
	{
		sprite.setPosition(round(x), round(y));
		sprite.setTexture(texture);

		i_window.draw(sprite);
	}
}
bool Mushroom::get_dead() const
{
	return death;
}



void Mushroom::set_dead(const bool i_value)
{
	death = i_value;
}
 void Mushroom::update(const unsigned i_view_x, const MapManager& i_map_manager)
 
 {
	

	if (0 == death)

 	{

		if (-CELL_DIMENSION < y && x >= static_cast<int>(i_view_x) - CELL_DIMENSION - ENTITY_UPDATE_AREA && x < ENTITY_UPDATE_AREA + SCREEN_WIDTH + i_view_x && y < SCREEN_HEIGHT)

 	{

			if (y > start_y - CELL_DIMENSION)

 			{

				y -= MUSHROOM_VELOCITY;


				if (y <= start_y - CELL_DIMENSION)


				{
				y = start_y - CELL_DIMENSION;



					start_y = 2 * SCREEN_HEIGHT;

				}

			}






			else

			{

				std::vector<unsigned char> collision;



				sf::FloatRect horizontal_hit_box;



				up_motion = std::min(GRAVITY_ACCELERATION + up_motion, MAX_VERTICAL_VELOCITY);



				collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, sf::FloatRect(x, up_motion + y, CELL_DIMENSION, CELL_DIMENSION));


				if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)

				{

					return 0 == i_value;

				}))

				{

					if (0 > up_motion)

					{

						y = CELL_DIMENSION * (1 + floor((up_motion + y) / CELL_DIMENSION));

					}

					else

					{

						y = CELL_DIMENSION * (ceil((up_motion + y) / CELL_DIMENSION) - 1);
					}



					up_motion = 0;

				}


				else
				{


					y += up_motion;

				}



				horizontal_hit_box = {MUSHROOM_VELOCITY * side_direction + x, y, CELL_DIMENSION, CELL_DIMENSION};



				collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, horizontal_hit_box);



				if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)

				{

					return 0 == i_value;

				}))
				{

					switch (side_direction > 0) {

				case true:


					x = CELL_DIMENSION * (ceil((MUSHROOM_VELOCITY * side_direction + x) / CELL_DIMENSION) - 1);

					break;

				case false:
					x = CELL_DIMENSION * (1 + floor((MUSHROOM_VELOCITY * side_direction + x) / CELL_DIMENSION));


					break;

			}
			side_direction *= -1;

		} else {

			x += MUSHROOM_VELOCITY * side_direction;


		}





	if (y > SCREEN_HEIGHT) {

		death = 1;

	}

}
	}}}


sf::FloatRect Mushroom::hitting_box() const

{

	return sf::FloatRect(x, y, CELL_DIMENSION, CELL_DIMENSION);

}
