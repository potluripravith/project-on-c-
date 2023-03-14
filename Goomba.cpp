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

Goomba::Goomba(const bool i_map2, const float j, const float k) :
	Enemy(j, k),
	isDyingFromCollision(0),
	map2(i_map2),
	death_duration(GOOMBA_DEATH_DURATION),
	walk_animation(CELL_DIMENSION, "Images/EnemyGoombaWalk.png", GOOMBA_WALK_ANIMATION_SPEED)
{
	side_motion = -GOOMBA_VELOCITY;

	if (0 == map2)
	{
		texture.loadFromFile("Images/EnemyGoombaDeath0.png");
	}
	else
	{
		texture.loadFromFile("Images/Map2GoombaDeath0.png");

		walk_animation.for_texture_position("Images/Map2GoombaWalk.png");
	}
}

bool Goomba::set_death(const bool m) const
{
	if (1 == m)
	{
		return death;
	}
	else
	{
		return death || isDyingFromCollision || GOOMBA_DEATH_DURATION > death_duration;
	}
}

void Goomba::die(const unsigned char i_death_type)
{
	switch (i_death_type)
	{
		case 0:
		{
			
		

			death = 1;

			break;
		}
		case 1:
		{
			
			if (0 == isDyingFromCollision)
			{
				death_duration--;
			}

			break;
		}
		case 2:
		{
			if (GOOMBA_DEATH_DURATION == death_duration)
			{
	
				isDyingFromCollision = 1;

				up_motion = 0.5f * MARIO_JUMP_VELOCITY;

				if (0 == map2)
				{
					texture.loadFromFile("Images/EnemyGoombaDeath1.png");
				}
				else
				{
					texture.loadFromFile("Images/Map2GoombaDeath1.png");
				}
			}
		}
	}
}

void Goomba::draw(const unsigned i_view_x, sf::RenderWindow& i_window)
{
	int state = 1;

	if (GOOMBA_DEATH_DURATION <= death_duration)
		state = 2;

	if (-CELL_DIMENSION >= round(y) || round(x) <= static_cast<int>(i_view_x) - CELL_DIMENSION || round(x) >= SCREEN_WIDTH + i_view_x || round(y) >= SCREEN_HEIGHT)
		state = 0;

	switch (state)
	{
		case 1:
			sprite.setPosition(round(x), round(y));
			sprite.setTexture(texture);

			i_window.draw(sprite);
			break;

		case 2:
			walk_animation.for_place(round(x), round(y));
			walk_animation.draw(i_window);
			break;

		default:
			break;
	}
}


void Goomba::update(const unsigned i_view_x, const std::vector<std::shared_ptr<Enemy>>& i_enemies, const MapManager& i_map_manager, Mario& i_mario)
{
	if (-CELL_DIMENSION < y && x >= static_cast<int>(i_view_x) - CELL_DIMENSION - ENTITY_UPDATE_AREA && x < ENTITY_UPDATE_AREA + SCREEN_WIDTH + i_view_x && y < SCREEN_HEIGHT)
{
    std::vector<unsigned char> collision;

    sf::FloatRect hit_box = hitting_box();

    up_motion = std::min(GRAVITY_ACCELERATION + up_motion, MAX_VERTICAL_VELOCITY);

    hit_box.top += up_motion;

    collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);

    if (0 == isDyingFromCollision)
    {
        bool has_collision = false;
       
	    for (const auto& value : collision)
        {
            switch (value)
            {
                case 0:
                    break;
                default:
                    has_collision = true;
                    break;
            }
        }

        if (has_collision)
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
				bool changed = false;

if (!set_death(0))
{
    for (unsigned short a = 0; a < i_enemies.size(); a++)
    {
        if (shared_from_this() != i_enemies[a] && !i_enemies[a]->set_death(0) && hit_box.intersects(i_enemies[a]->hitting_box()))
        {
            switch (up_motion >= 0)
            {
                case true:
                    y = i_enemies[a]->hitting_box().top - CELL_DIMENSION;
                    break;
                case false:
                    y = i_enemies[a]->hitting_box().height + i_enemies[a]->hitting_box().top;
                    break;
            }
            
            up_motion = 0;
            changed = true;
            break;
        }
    }
}

if (!changed)
{
    y += up_motion;
}
			}
			

			
				if (0 == set_death(0))
{
	hit_box = hitting_box();
	hit_box.left += side_motion;

	collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);

	if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
	{
		return 0 == i_value;
	}))
	{
		switch (side_motion >= 0)
		{
		case true:
			x = CELL_DIMENSION * (ceil((side_motion + x) / CELL_DIMENSION) - 1);
			break;

		case false:
			x = CELL_DIMENSION * (1 + floor((side_motion + x) / CELL_DIMENSION));
			break;
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
				changed = 1;

				side_motion *= -1;

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
	//Check Mario's vertical speed
	switch (i_mario.set_up_motion() > 0)
	{
		case true:
			//If Mario is falling...
			die(1);
			i_mario.set_up_motion(0.5f * MARIO_JUMP_VELOCITY);
			break;
		case false:
			//Otherwise, kill Mario.
			i_mario.die(0);
			break;
	}
}
				walk_animation.update();
			}
			else if (GOOMBA_DEATH_DURATION > death_duration)
			{
				if (0 < death_duration)
				{
					death_duration--;
				}
				else
				{
					die(0);
				}
			}
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

