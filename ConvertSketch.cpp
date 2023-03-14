#include <array>
#include <cmath>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Headers/Animation.hpp"
#include "Headers/Global.hpp"
#include "Headers/MapManager.hpp"
#include "Headers/Mushroom.hpp"
#include "Headers/Mario.hpp"
#include "Headers/Enemy.hpp"
#include "Headers/Goomba.hpp"
#include "Headers/Koopa.hpp"
#include "Headers/ConvertSketch.hpp"


void convert_sketch(const unsigned char present_level, unsigned short& finish_level, std::vector<std::shared_ptr<Enemy>>& xenemy, sf::Color& bg_color, MapManager& i_map_manager, Mario& i_mario)
{
	unsigned short map_height;

	i_map_manager.update_map_sketch(present_level);

	i_map_manager.set_map_size(i_map_manager.get_map_sketch_width());

	//We divide the height by 3 because the sketch stores the level as 3 layers: blocks, entities, and background tiles.
	map_height = floor(i_map_manager.get_map_sketch_height() / 3.f);

	bg_color = i_map_manager.get_map_sketch_pixel(0, i_map_manager.get_map_sketch_height() - 1);

	for (unsigned short a = 0; a < i_map_manager.get_map_sketch_width(); a++)

	{
		for (unsigned short b = 0; b < 2 * map_height; b++)

		{
			sf::Color pixel = i_map_manager.get_map_sketch_pixel(a, b);


			if (b < map_height)

			{
				if (sf::Color(182, 73, 0) == pixel)

				{
				   i_map_manager.set_map_cell(a, b, Cell::Brick);

				}
				else if (sf::Color(255, 255, 0) == pixel)
				{

					i_map_manager.set_map_cell(a, b, Cell::Coin);
				}

				else if (sf::Color(0, 146, 0) == pixel || sf::Color(0, 182, 0) == pixel || sf::Color(0, 219, 0) == pixel)
				
				{
					//Multiple colors, because we need to know which part of the pipe we need to draw.
				
					i_map_manager.set_map_cell(a, b, Cell::Pipe);
				}
				
				else if (sf::Color(255, 73, 85) == pixel || sf::Color(255, 146, 85) == pixel)
				
				{
					i_map_manager.set_map_cell(a, b, Cell::QuestionBlock);
				}
				
				else if (sf::Color(0, 0, 0) == pixel || sf::Color(146, 73, 0) == pixel)
				
				{
				
					i_map_manager.set_map_cell(a, b, Cell::Wall);
				
				}
				
				else
				
				{
				
					i_map_manager.set_map_cell(a, b, Cell::Empty);



					if (sf::Color(0, 255, 255) == pixel)

					{

						finish_level = a;

					}
				}

			}

			else

			{

				if (sf::Color(255, 0, 0) == pixel)

				{

					i_mario.set_location(CELL_DIMENSION * a, CELL_DIMENSION * (b - map_height));

				}

				else if (sf::Color(182, 73, 0) == pixel)

				{

					xenemy.push_back(std::make_shared<Goomba>(sf::Color(0, 0, 85) == bg_color, CELL_DIMENSION * a, CELL_DIMENSION * (b - map_height)));
				}

				else if (sf::Color(0, 219, 0) == pixel)

				{

					xenemy.push_back(std::make_shared<Koopa>(sf::Color(0, 0, 85) == bg_color, CELL_DIMENSION * a, CELL_DIMENSION * (b - map_height)));
				}

			}

		}

	}

}




