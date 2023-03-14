#pragma once


constexpr unsigned short SCREEN_WIDTH = 320;

constexpr unsigned short SCREEN_HEIGHT = 240;

constexpr float BRICK_PARTICLE_VELOCITY = 4;

constexpr float COIN_JUMP_VELOCITY = -6;

constexpr float GOOMBA_VELOCITY = 1;

constexpr float GRAVITY_ACCELERATION = 0.25f;

constexpr float KOOPA_SHELL_VELOCITY = 4;

constexpr float KOOPA_WALK_VELOCITY = 1;

constexpr float MARIO_ACCELERATION = 0.25f;

constexpr float MARIO_JUMP_VELOCITY = -4;

constexpr float MARIO_WALK_VELOCITY = 2;

constexpr float MAX_VERTICAL_VELOCITY = 8;

constexpr float MUSHROOM_VELOCITY = 1;

constexpr unsigned char CELL_DIMENSION = 16;

constexpr unsigned char SCREEN_SCALE = 2;

constexpr unsigned char MARIO_BLINK_DURATION = 4;

constexpr unsigned char MARIO_JUMP_DURATION = 8;

constexpr unsigned char MARIO_GROWTH_PAUSE = 64;

constexpr unsigned char MARIO_DEATH_PAUSE = 32;

constexpr unsigned char MARIO_INVINCIBLE_DURATION = 128;


constexpr unsigned short ENTITY_UPDATE_AREA = 64;

constexpr unsigned short COIN_ANIMATION_SPEED = 4;

constexpr unsigned short MARIO_WALK_ANIMATION_SPEED = 4;

constexpr unsigned short QUESTION_BLOCK_ANIMATION_SPEED = 8;

constexpr unsigned short GOOMBA_WALK_ANIMATION_SPEED = 32;

constexpr unsigned short GOOMBA_DEATH_DURATION = 32;

constexpr unsigned short KOOPA_WALK_ANIMATION_SPEED = 32;

constexpr unsigned short KOOPA_GET_OUT_ANIMATION_SPEED = 4;

constexpr unsigned short KOOPA_EXIT_PAUSE = 512;




constexpr std::chrono::microseconds FRAME_DURATION(16667);

enum Cell
{
	ActivatedQuestionBlock,
	Brick,
	Coin,
	Empty,
	Pipe,
	QuestionBlock,
	Wall
};


struct Object
{
	float side_motion;
	float up_motion;
	float x;
	float y;

	Object(const float j, const float k, const float mside_motion = 0, const float mup_motion = 0) :
		side_motion(mside_motion),
		up_motion(mup_motion),
		x(j),
		y(k)
	{

	}
};


typedef std::vector<std::array<Cell, SCREEN_HEIGHT / CELL_DIMENSION>> Map;