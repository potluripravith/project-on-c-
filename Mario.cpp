#include <array>
#include <chrono>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Headers/Animation.hpp"
#include "Headers/Global.hpp"
#include "Headers/MapManager.hpp"
#include "Headers/Mushroom.hpp"
#include "Headers/Mario.hpp"

Mario::Mario() :
	crouching(0),
	death(0),
	flipped(0),
	ground(0),
	enemy_bounce_speed(0),
	side_motion(0),
	up_motion(0),
	x(0),
	y(0),
	jump_duration(0),
	powerup_state(0),
	death_duration(MARIO_DEATH_PAUSE),
	growth_timer(0),
	invincible_timer(0),
	big_walk_animation(CELL_DIMENSION, "Images/SuperMarioGif.png", MARIO_WALK_ANIMATION_SPEED),
	walk_animation(CELL_DIMENSION, "Images/SmallMarioGif.png", MARIO_WALK_ANIMATION_SPEED)
{
	texture.loadFromFile("Images/SmallMarioStand.png");

	sprite.setTexture(texture);
}


bool Mario::set_death() const
{
	return death;
}


float Mario::set_up_motion() const
{
	return up_motion;
}


float Mario::set_x() const
{
	return x;
}


void Mario::die(bool isInstantDeath)
{
    switch (isInstantDeath)
    {
        case true:
            death = true;
            texture.loadFromFile(powerup_state == 0 ? "Images/SmallMarioDead.png" : "Images/SuperMarioDead.png");
            break;
        case false:
            if (growth_timer == 0 && invincible_timer == 0)
            {
                if (powerup_state == 0)
                {
                    death = true;
                    texture.loadFromFile("Images/SmallMarioDead.png");
                }
                else
                {
                  powerup_state = 0;
				invincible_timer = MARIO_INVINCIBLE_DURATION;
				y += (crouching == 0) ? CELL_DIMENSION : 0;
				crouching = 0;
                }
            }
            break;
    }
}
void Mario::draw(sf::RenderWindow& i_window)
{
	//When Mario is invincible, his sprite will blink.
	if (0 == invincible_timer / MARIO_BLINK_DURATION % 2)
	{
		bool draw_sprite = 1;
		//When Mario is growing, his sprite will switch between being big and small.
		bool draw_big = 0 == growth_timer / MARIO_BLINK_DURATION % 2;

		sprite.setPosition(round(x), round(y));

		if (0 == death)
		{
			if (0 < powerup_state)
			{
				if (1 == crouching)
				{
					if (0 == draw_big)
					{
						texture.loadFromFile("Images/SmallMarioStand.png");
					}
					else
					{
						texture.loadFromFile("Images/SuperMarioCrouch.png");
					}
				}
				else if (0 == ground)
				{
					if (0 == draw_big)
					{
						sprite.setPosition(round(x), CELL_DIMENSION + round(y));

						texture.loadFromFile("Images/SmallMarioJump.png");
					}
					else
					{
						texture.loadFromFile("Images/SuperMarioJump.png");
					}
				}
				else
				{
					if (0 == side_motion)
					{
						if (0 == draw_big)
						{
							sprite.setPosition(round(x), CELL_DIMENSION + round(y));

							texture.loadFromFile("Images/SmallMarioStand.png");
						}
						else
						{
							texture.loadFromFile("Images/SuperMarioStand.png");
						}
					}
					else if ((0 < side_motion && 0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
							  1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ||
							 (0 > side_motion && 0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
							  1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))

					{
						if (0 == draw_big)
						{
							sprite.setPosition(round(x), CELL_DIMENSION + round(y));

							texture.loadFromFile("Images/SuperMarioBrake.png");
						}
						else
						{
							texture.loadFromFile("Images/SuperMarioBrake.png");
						}
					}
					else
					{
						draw_sprite = 0;

						if (0 == draw_big)
						{
							walk_animation.for_flip(flipped);
							walk_animation.for_place(round(x), CELL_DIMENSION + round(y));
							walk_animation.draw(i_window);
						}
						else
						{
							big_walk_animation.for_flip(flipped);
							big_walk_animation.for_place(round(x), round(y));
							big_walk_animation.draw(i_window);
						}
					}
				}
			}
			else if (0 == ground)
			{
				texture.loadFromFile("Images/SmallMarioJump.png");
			}
			else
			{
				if (0 == side_motion)
				{
					texture.loadFromFile("Images/SmallMarioStand.png");
				}
				else if ((0 < side_motion && 0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
						  1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ||
						 (0 > side_motion && 0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
						  1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))

				{
					texture.loadFromFile("Images/SmallMarioBrake.png");
				}
				else
				{
					draw_sprite = 0;

					walk_animation.for_flip(flipped);
					walk_animation.for_place(round(x), round(y));
					walk_animation.draw(i_window);
				}
			}
		}

		if (1 == draw_sprite)
		{
			if (0 == flipped)
			{
				sprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));
			}
			else
			{
				sprite.setTextureRect(sf::IntRect(texture.getSize().x, 0, -static_cast<int>(texture.getSize().x), texture.getSize().y));
			}

			i_window.draw(sprite);
		}
	}
}

void Mario::draw_mushrooms(const unsigned i_view_x, sf::RenderWindow& i_window)
{
	for (Mushroom& mushroom : mushrooms)
	{
		mushroom.draw(i_view_x, i_window);
	}
}

void Mario::reset()
{
	crouching = 0;
    death = 0;
    flipped = 0;

   ground = 0;

	enemy_bounce_speed = 0;

side_motion = 0;

up_motion = 0;

x = 0;
y = 0;

jump_duration = 0;
powerup_state = 0;

death_duration = MARIO_DEATH_PAUSE;

growth_timer = 0;

invincible_timer = 0;

mushrooms.clear();

texture.loadFromFile("Images/SmallMarioStand.png");
sprite.setTexture(texture);

walk_animation.for_flip(0);
walk_animation.for_ani_speed(MARIO_WALK_ANIMATION_SPEED);

big_walk_animation.for_flip(0);

big_walk_animation.for_ani_speed(MARIO_WALK_ANIMATION_SPEED);
;
}

void Mario::set_location(const float j, const float k)
{
	x = j;
	y = k;
}

void Mario::set_up_motion(const float i_value)
{
	enemy_bounce_speed = i_value;
}

void Mario::update(const unsigned i_view_x, MapManager& i_map_manager)
{
	
	if (0 != enemy_bounce_speed)
	{
		up_motion = enemy_bounce_speed;

		enemy_bounce_speed = 0;
	}

	for (Mushroom& mushroom : mushrooms)
	{
		mushroom.update(i_view_x, i_map_manager);
	}

	if (0 == death)
{
    bool moving = 0;

    std::vector<unsigned char> collision;

    std::vector<sf::Vector2i> cells;

    sf::FloatRect hit_box = hitting_box();

    ground = 0;

    if (0 == crouching)
    {
        int movement_direction = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            movement_direction = 1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            movement_direction = -1;
        }

        switch (movement_direction)
        {
            case 1:
                moving = 1;
                side_motion = std::min(MARIO_ACCELERATION + side_motion, MARIO_WALK_VELOCITY);
                break;
            case -1:
                moving = 1;
                side_motion = std::max(side_motion - MARIO_ACCELERATION, -MARIO_WALK_VELOCITY);
                break;
            default:
                break;
        }
    }
		if (0 == moving)
		{
			if (0 < side_motion)
			{
				side_motion = std::max<float>(0, side_motion - MARIO_ACCELERATION);
			}
			else if (0 > side_motion)
			{
				side_motion = std::min<float>(0, MARIO_ACCELERATION + side_motion);
			}
		}

		if (0 < powerup_state)
		{
			if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::C) || 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				if (0 == crouching)
				{
					crouching = 1;

					y += CELL_DIMENSION;
				}
			}
			else if (1 == crouching)
			{
				hit_box.height += CELL_DIMENSION;
				hit_box.top -= CELL_DIMENSION;

				
				collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);

				if (1 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
				{
					return 0 == i_value;
				}))
				{
					crouching = 0;

					y -= CELL_DIMENSION;
				}
				else
				{
					collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);

					
					if (1 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
					{
						return 0 == i_value;
					}))
					{
						crouching = 0;

						y -= CELL_DIMENSION;

						i_map_manager.map_collision({Cell::Brick}, cells, hit_box);

						for (const sf::Vector2i& cell : cells)
						{
							i_map_manager.set_map_cell(cell.x, cell.y, Cell::Empty);
							i_map_manager.add_brick_particles(CELL_DIMENSION * cell.x, CELL_DIMENSION * cell.y);
						}
					}
				}
			}
		}

		hit_box = hitting_box();
		hit_box.left += side_motion;
		
		collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);

		if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
		{
			return 0 == i_value;
		}))
		{
			moving = 0;

			if (0 < side_motion)
			{
				x = CELL_DIMENSION * (ceil((side_motion + x) / CELL_DIMENSION) - 1);
			}
			else if (0 > side_motion)
			{
				x = CELL_DIMENSION * (1 + floor((side_motion + x) / CELL_DIMENSION));
			}

			side_motion = 0;
		}
		else
		{
			x += side_motion;
		}

		hit_box = hitting_box();
		hit_box.top++;

		collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);
		
		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			if (0 == up_motion && 0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
			{
				return 0 == i_value;
			}))
			{
				up_motion = MARIO_JUMP_VELOCITY;

				jump_duration = MARIO_JUMP_DURATION;
			}
			else if (0 < jump_duration) //The longer we press the jump button, the higher Mario jumps.
			{
				up_motion = MARIO_JUMP_VELOCITY;

				jump_duration--;
			}
			else
			{
				up_motion = std::min(GRAVITY_ACCELERATION + up_motion, MAX_VERTICAL_VELOCITY);
			}
		}
		else
		{
			up_motion = std::min(GRAVITY_ACCELERATION + up_motion, MAX_VERTICAL_VELOCITY
			
			);

			jump_duration = 0;
		}

		hit_box = hitting_box();
		hit_box.top += up_motion;

		collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);
		
		if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
		{
			return 0 == i_value;
		}))
		{
			if (0 > up_motion)
			
			{
				//Destroying bricks!!!!
			
				if (0 == crouching && 0 < powerup_state)
			
				{
			
					i_map_manager.map_collision({Cell::Brick}, cells, hit_box);

			
					for (const sf::Vector2i& cell : cells)
			
					{
						i_map_manager.set_map_cell(cell.x, cell.y, Cell::Empty);
			
						i_map_manager.add_brick_particles(CELL_DIMENSION * cell.x, CELL_DIMENSION * cell.y);
			
					}
			
				}

				i_map_manager.map_collision({Cell::QuestionBlock}, cells, hit_box);

			
				//Activating question blocks!!!!
			
				for (const sf::Vector2i& cell : cells)
				{
			
					i_map_manager.set_map_cell(cell.x, cell.y, Cell::ActivatedQuestionBlock);


					//It can be either a mushroom or a coin, depending on the color of the pixel in the sketch.

					if (sf::Color(255, 73, 85) == i_map_manager.get_map_sketch_pixel(cell.x, cell.y))
					{

						mushrooms.push_back(Mushroom(CELL_DIMENSION * cell.x, CELL_DIMENSION * cell.y));
					}

					else

					{
						i_map_manager.add_question_block_coin(CELL_DIMENSION * cell.x, CELL_DIMENSION * cell.y);

					}

				}


				y = CELL_DIMENSION * (1 + floor((up_motion + y) / CELL_DIMENSION));
			}
			else if (0 < up_motion)
			{
				y = CELL_DIMENSION * (ceil((up_motion + y) / CELL_DIMENSION) - 1);
			}

			jump_duration = 0;

			up_motion = 0;
		}
		
		else
		{
		
			y += up_motion;
		}

		if (0 == side_motion)
		
		{
			if (1 == moving)
			{
				flipped = 1 - flipped;
			}
		}
		else if (0 < side_motion)
		
		{
			flipped = 0;
		}
		else if (0 > side_motion)
		{
			flipped = 1;
		
		}

		hit_box = hitting_box();
		hit_box.top++;

		
		
		collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);

		if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
		{
			return 0 == i_value;
		}))
		
		
		{
			ground = 1;
		
		
		}

		for (Mushroom& mushroom : mushrooms) {
        
		     if (hitting_box().intersects(mushroom.hitting_box())) {
                 mushroom.set_dead(true);


        if (!powerup_state) {
            powerup_state = true;
            growth_timer = MARIO_GROWTH_PAUSE;
            y -= CELL_DIMENSION;

        }
    }

}


		if (0 < invincible_timer)
		{
			invincible_timer--;
		}

		hit_box = hitting_box();

		i_map_manager.map_collision({Cell::Coin}, cells, hit_box);

		//Collecting coins.
		for (const sf::Vector2i& cell : cells) 
		{
  i_map_manager.set_map_cell(cell.x, cell.y, Cell::Empty);
}

   if (y >= SCREEN_HEIGHT - hitting_box().height) {
  still_alive = false;
}

if (still_alive) {
  if (0 < growth_timer) {
    growth_timer--;
  }

  if (0 == powerup_state) {
    walk_animation.for_ani_speed(MARIO_WALK_ANIMATION_SPEED * MARIO_WALK_VELOCITY / abs(side_motion));
    walk_animation.update();
  } else {
    big_walk_animation.for_ani_speed(MARIO_WALK_ANIMATION_SPEED * MARIO_WALK_VELOCITY / abs(side_motion));
    big_walk_animation.update();
  }
} else {
  if (death_duration > 0) {
    death_duration--;
    up_motion = MARIO_JUMP_VELOCITY;
  } else {
    up_motion = std::min(GRAVITY_ACCELERATION + up_motion, MAX_VERTICAL_VELOCITY);
    y += up_motion;
  }
}
	}
//Deleting mushrooms from the vector.
std::vector<Mushroom>::iterator it = mushrooms.begin();
    while (it != mushrooms.end()) {
       if (1 == it->get_dead()) {
        it = mushrooms.erase(it);
            } else {
            it++;
                 }
                }
             }

sf::FloatRect Mario::hitting_box() const
{
	//The hitbox will be small if Mario is small or crouching.
	if (1 == crouching || 0 == powerup_state)
	{
		return sf::FloatRect(x, y, CELL_DIMENSION, CELL_DIMENSION);
	}
	else
	{
		return sf::FloatRect(x, y, CELL_DIMENSION, 2 * CELL_DIMENSION);
	}
}