#include <array>
#include <chrono>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/DrawMap.hpp"

void draw_map(unsigned view, const sf::Image& sketch, sf::RenderWindow& screen, const sf::Texture& texture, const Map& i_map)
{
	
	//We'll only draw the part of the map that is visible on the screen.
	unsigned short map_end = ceil((view + SCREEN_WIDTH) / static_cast<float>(CELL_DIMENSION));

	
	unsigned short map_height = floor(sketch.getSize().y / 3.f);
	
	unsigned short map_start = floor(view / static_cast<float>(CELL_DIMENSION));

	sf::Sprite cell_sprite(texture);

	for (unsigned short a = map_start; a < map_end; a++)
	{
		for (unsigned short b = 0; b < map_height; b++)
		{
			unsigned short sprite_x = 0;

			unsigned short sprite_y = 0;



			sf::Color pixel = sketch.getPixel(a, b + 2 * map_height);
			sf::Color pixel_down = sf::Color(0, 0, 0, 0);

			sf::Color pixel_left = sf::Color(0, 0, 0, 0);

			sf::Color pixel_right = sf::Color(0, 0, 0, 0);

			sf::Color pixel_up = sf::Color(0, 0, 0, 0);



			cell_sprite.setPosition(CELL_DIMENSION * a, CELL_DIMENSION * b);




			if (255 == pixel.a)

			{


				//Here we're getting pixels around the pixel we're currently checking.
				if (0 < a)

				{

					pixel_left = sketch.getPixel(a - 1, b + 2 * map_height);

				}



				if (0 < b)

				{

					pixel_up = sketch.getPixel(a, b + 2 * map_height - 1);

				}



				if (a < sketch.getSize().x - 1)

				{

					pixel_right = sketch.getPixel(1 + a, b + 2 * map_height);

				}


				if (b < map_height - 1)

				{

					pixel_down = sketch.getPixel(a, 1 + b + 2 * map_height);

				}





				if (sf::Color(255, 255, 255) == pixel) //Clouds

				{

					sprite_x = 8;



					if (sf::Color(255, 255, 255) == pixel_up)

					{

						sprite_y = 1;

					}


					if (sf::Color(255, 255, 255) == pixel_left)
					{
						if (sf::Color(255, 255, 255) != pixel_right)
						{
							sprite_x = 9;

						}
					}
					else if (sf::Color(255, 255, 255) == pixel_right)
					{
						sprite_x = 7;
					}
				}
				else if (sf::Color(146, 219, 0) == pixel) //Grass
				{
					sprite_x = 5;

					if (sf::Color(146, 219, 0) == pixel_left)
					{
						if (sf::Color(146, 219, 0) != pixel_right)
						{
							sprite_x = 6;
						}
					}
					else if (sf::Color(146, 219, 0) == pixel_right)
					{
						sprite_x = 4;
					}
				}
				else if (sf::Color(0, 73, 0) == pixel) //Hills outline
				{
					sprite_y = 1;

					if (sf::Color(0, 109, 0) == pixel_left)
					{
						if (sf::Color(0, 109, 0) != pixel_right)
						{
							sprite_x = 2;
						}
					}
					else if (sf::Color(0, 109, 0) == pixel_right)
					{
						sprite_x = 1;
					}
				}
				else if (sf::Color(0, 109, 0) == pixel) //Hills
				{
					sprite_x = 4;
					sprite_y = 1;

					if (sf::Color(0, 73, 0) == pixel_left)
					{
						sprite_x = 3;
					}
					else if (sf::Color(0, 73, 0) == pixel_right)
					{
						sprite_x = 5;
					}
				}
				
			     else if (sf::Color(109, 255, 85) == pixel) //Flagpole
				{
					sprite_x = 12;

					if (sf::Color(109, 255, 85) == pixel_up)
					{
						sprite_y = 1;
					}
				}

				cell_sprite.setTextureRect(sf::IntRect(CELL_DIMENSION * sprite_x, CELL_DIMENSION * sprite_y, CELL_DIMENSION, CELL_DIMENSION));

				screen.draw(cell_sprite);
			}

			//drawing the blocks separately from the background tiles.
			
			
			if (Cell::Empty != i_map[a][b])
			{
				if (Cell::Pipe == i_map[a][b]) //Pipes
				{
					if (Cell::Pipe == i_map[a][b - 1])
					{
						sprite_y = 1;
					}
					else
					{
						sprite_y = 0;
					}

					if (Cell::Pipe == i_map[a - 1][b])
					{
						sprite_x = 11;
					}
					else
					{
						sprite_x = 10;
					}
				}
				else if (Cell::QuestionBlock == i_map[a][b]) //Question blocks
				{
					sprite_x = 1;
					sprite_y = 0;
				}
				else if (Cell::Wall == i_map[a][b])
				{
					sprite_y = 0;

					if (sf::Color(0, 0, 0) == sketch.getPixel(a, b)) //Walls
					{
						sprite_x = 2;
					}
					else //Solid blocks
					{
						sprite_x = 3;
					}
				}

				cell_sprite.setTextureRect(sf::IntRect(CELL_DIMENSION * sprite_x, CELL_DIMENSION * sprite_y, CELL_DIMENSION, CELL_DIMENSION));

				screen.draw(cell_sprite);
			}
		}
	}
}
