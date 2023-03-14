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
#include "Headers/Koopa.hpp"

Koopa::Koopa(const bool mmap2, const float j, const float k) :
	Enemy(j, k),
	collision_for(1),
	flipped(1),
	hitting_x(0),
	map2(mmap2),
	state(0),
	get_out_timer(KOOPA_EXIT_PAUSE),
	get_out_animation(CELL_DIMENSION, "Images/EnemyKoopaGetOut.png", KOOPA_GET_OUT_ANIMATION_SPEED),
	walk_animation(CELL_DIMENSION, "Images/EnemyKoopaWalk.png", KOOPA_WALK_ANIMATION_SPEED)
{
	side_motion = -KOOPA_WALK_VELOCITY;

	if (0 == map2)
	{
		texture.loadFromFile("Images/EnemyKoopaShell.png");
	}
	else
	{
		texture.loadFromFile("Images/Map2KoopaShell.png");

		get_out_animation.for_texture_position("Images/Map2KoopaGetOut.png");
		walk_animation.for_texture_position("Images/Map2KoopaWalk.png");
	}
}

bool Koopa::set_death(const bool m) const
{
	if (1 == m)
	{
		return death;
	}
	else
	{
		return death || hitting_x;
	}
}

void Koopa::die(const unsigned char n)
{
	switch (n)
	{
		case 0:
		{
			// Instant death.
			death = 1;

			break;
		}
		case 2:
		{
			// Dying from Koopa's shell.
			hitting_x = 1;

			up_motion = 0.5f * MARIO_JUMP_VELOCITY;

			if (0 == map2)
			{
				texture.loadFromFile("Images/KoopaDeath.png");
			}
			else
			{
				texture.loadFromFile("Images/Map2KoopaDeath.png");
			}
		}
	}
}

void Koopa::draw(const unsigned view, sf::RenderWindow& window)
{
	if (-CELL_DIMENSION < round(y) && round(x) > static_cast<int>(view) - CELL_DIMENSION && round(x) < SCREEN_WIDTH + view && round(y) < SCREEN_HEIGHT)
	{
		if (0 < state || 1 == hitting_x)
		{
			if (0 == set_death(0) && 1 == state && get_out_timer <= 0.25f * KOOPA_EXIT_PAUSE)
			{
				get_out_animation.for_place(round(x), round(y));
				get_out_animation.draw(window);
			}
			else
			{
				sprite.setPosition(round(x), round(y));
				sprite.setTexture(texture);

				window.draw(sprite);
			}
		}
		else
		{
			walk_animation.for_flip(flipped);
			walk_animation.for_place(round(x), round(y));
			walk_animation.draw(window);
		}
	}
}

void Koopa::update(const unsigned i_view_x, const std::vector<std::shared_ptr<Enemy>>& i_enemies, const MapManager& i_map_manager, Mario& i_mario)
{
	

	if (-CELL_DIMENSION < y && x >= static_cast<int>(i_view_x) - CELL_DIMENSION - ENTITY_UPDATE_AREA && x < ENTITY_UPDATE_AREA + SCREEN_WIDTH + i_view_x && y < SCREEN_HEIGHT)
	{
		up_motion = std::min(GRAVITY_ACCELERATION + up_motion, MAX_VERTICAL_VELOCITY
		
		
		);

		if (0 == set_death(0))
		{
			std::vector<unsigned char> collision;

			sf::FloatRect hit_box = hitting_box();

			hit_box.top += up_motion;

			collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);

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
				bool changed = 0;

				for (unsigned short a = 0; a < i_enemies.size(); a++)
				{
					if (shared_from_this() != i_enemies[a] && 0 == i_enemies[a]->set_death(0) && 1 == hit_box.intersects(i_enemies[a]->hitting_box()))
					{
						changed = 1;

						if (0 > up_motion)
						{
							y = i_enemies[a]->hitting_box().top + i_enemies[a]->hitting_box().height;
						}
						else
						{
							y = i_enemies[a]->hitting_box().top - CELL_DIMENSION;
						}

						up_motion = 0;

						break;
					}
				}

				if (0 == changed)
				{
					y += up_motion;
				}
			}

			hit_box = hitting_box();
			hit_box.left += side_motion;

			collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);

			if (0 == hitting_x && 0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
			{
				return 0 == i_value;
			}))
			{
				if (0 < side_motion)
				{
					x = CELL_DIMENSION * (ceil((side_motion + x) / CELL_DIMENSION) - 1);
				}
				else
				{
					x = CELL_DIMENSION * (1 + floor((side_motion + x) / CELL_DIMENSION));
				}

				side_motion *= -1;
			}
			else
			{
				bool changed = 0;

				for (unsigned short a = 0; a < i_enemies.size(); a++)
				{
					if (shared_from_this() != i_enemies[a] && 0 == i_enemies[a]->set_death(0) && 1 == hit_box.intersects(i_enemies[a]->hitting_box()))
					{
						if (0 == state)
						{
							changed = 1;

							side_motion *= -1;
						}
						else if (2 == state)
						{
							i_enemies[a]->die(2);
						}

						break;
					}
				}

				if (0 == changed)
				{
					x += side_motion;
				}
			}

			if (0 == i_mario.set_death() && 1 == hitting_box().intersects(i_mario.hitting_box()))
			{
				if (1 == collision_for)
				{
					if (0 == state)
					{
						// Turning into a shell.
						if (0 < i_mario.set_up_motion())
						{
							// We check the collision only once after we collide with Mario.
							collision_for = 0;

							side_motion = 0;

							state = 1;

							get_out_timer = KOOPA_EXIT_PAUSE;

							i_mario.set_up_motion(0.5f * MARIO_JUMP_VELOCITY);
						}
						else
						{
							i_mario.die(0);
						}
					}

					// Start sliding.
					else if (1 == state)
					{
						collision_for = 0;

						state = 2;

						// The direction is based on where the Koopa is kicked by Mario.
						if (x < i_mario.set_x())
						{
							side_motion = -KOOPA_SHELL_VELOCITY;
						}
						else
						{
							side_motion = KOOPA_SHELL_VELOCITY;
						}

						if (0 < i_mario.set_up_motion())
						{
							i_mario.set_up_motion(0.5f * MARIO_JUMP_VELOCITY);
						}
					}
					else
					{
						//Stop sliding.
						if (0 < i_mario.set_up_motion())
						{
							collision_for = 0;

							side_motion = 0;

							state = 1;

							get_out_timer = KOOPA_EXIT_PAUSE;

							i_mario.set_up_motion(0.5f * MARIO_JUMP_VELOCITY);
						}
						// Don't stand in front of a sliding shell.
						else if ((0 < side_motion && x < i_mario.set_x()) ||
								 (0 > side_motion && x > i_mario.set_x()))
						{
							i_mario.die(0);
						}
					}
				}
			}
			else
			
			{
				// Once Mario stops crossing paths with Bowser, start looking for collisions.
				collision_for = 1;
			}

			if (0 < side_motion)
			{
				flipped = 0;
			}
			else if (0 > side_motion)
			{
				flipped = 1;
			}

			// Koopa can get out of his shell here.
			if (1 == state)
			{
				get_out_timer--;
				
				if (0 == get_out_timer)
				{
					state = 0;

					if (0 == flipped)
					{
						side_motion = KOOPA_WALK_VELOCITY;
					}
					else
					{
						side_motion = -KOOPA_WALK_VELOCITY;
					}
				}
				else
				{
					get_out_animation.update();
				}
			}

			walk_animation.update();
		}
		else
		{
			y += up_motion;
		}
	}

	if (SCREEN_HEIGHT <= y)
	{
		die(0);
	}
}

	

