#include <array>
#include <chrono>
#include <cmath>
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

int main()
{
	unsigned char present_level = 0;

	unsigned short finish_level = 0;

	
	std::chrono::microseconds lag(0);

	std::chrono::steady_clock::time_point previous_time;
		sf::Event event;

	sf::RenderWindow window(sf::VideoMode(SCREEN_SCALE * SCREEN_WIDTH, SCREEN_SCALE * SCREEN_HEIGHT), "Super Mario", sf::Style::Close);
	

	
	std::vector<std::shared_ptr<Enemy>> enemies;

	sf::Color background_color = sf::Color(0, 219, 255);

	

	window.setPosition(sf::Vector2i(window.getPosition().x, window.getPosition().y - 90));

	sf::View view(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

	MapManager map_manager;

	Mario mario;

	convert_sketch(present_level, finish_level, enemies, background_color, map_manager, mario);

	previous_time = std::chrono::steady_clock::now();


   while (window.isOpen())
      { 
	std::chrono::microseconds delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time);

	lag += delta_time;

	previous_time += delta_time;

	while (FRAME_DURATION <= lag)
	{
		unsigned view_x;

		lag -= FRAME_DURATION;

		while (window.pollEvent(event))

		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Enter)
				{
					enemies.clear();

					mario.reset();

					convert_sketch(present_level, finish_level, enemies, background_color, map_manager, mario);
				}
			}
		}
	
			//Once Mario goes beyond the finish, we move on to the next level.
			if (CELL_DIMENSION * finish_level <= mario.set_x())
			{
				present_level++;

				enemies.clear();

				mario.reset();

				convert_sketch(present_level, finish_level, enemies, background_color, map_manager, mario);
			}

			//Keeping Mario at the center of the view.
			view_x = std::clamp<int>(round(mario.set_x()) - 0.5f * (SCREEN_WIDTH - CELL_DIMENSION), 0, CELL_DIMENSION * map_manager.get_map_width() - SCREEN_WIDTH);

			map_manager.update();

			mario.update(view_x, map_manager);

			
for (unsigned short a = 0; a < enemies.size();)

			{
				enemies[a]->update(view_x, enemies, map_manager, mario);
				if (enemies[a]->set_death(1))
				{
					//We don't have to worry about memory leaks since we're using SMART POINTERS!
					enemies.erase(a + enemies.begin());
				}
				else
				{
					a++;
				}
			}

			bool should_update_and_render = true;
            while (FRAME_DURATION <= lag)
   {
  should_update_and_render = false;
  lag -= FRAME_DURATION;
}

if (should_update_and_render)
{
  view.reset(sf::FloatRect(view_x, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
  window.setView(view);
  window.clear(background_color);

  map_manager.draw_map(1, sf::Color(0, 0, 85) == background_color, view_x, window);
  mario.draw_mushrooms(view_x, window);
  map_manager.draw_map(0, sf::Color(0, 0, 85) == background_color, view_x, window);

  for (unsigned short a = 0; a < enemies.size(); a++)
  {
    enemies[a]->draw(view_x, window);
  }

  mario.draw(window);

  window.display();
}
	}}}