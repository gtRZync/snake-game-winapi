#ifndef GAME_H
#define GAME_H
#pragma once

#include "snake.h"
#include "pellet.h"


void set_rand_seed();
DIRECTIONS get_reverse_direction(DIRECTIONS direction);
void move_snake(Snake* snake, int nextX, int nextY);
boolean is_collision_snake_body(Snake* snake);
boolean is_collisions_snake_pellet(Snake* snake, lpPellet pellet);
void eat_pellet(Snake* snake, lpPellet pellet);
void move_player(DIRECTIONS current_direction, Snake* snake);
void change_direction(DIRECTIONS* current_direction, Snake* snake, WPARAM wParam);
void animate_pellet(lpPellet pellet);
void stop_at_wall(DIRECTIONS direction, Snake* snake);
float interpolateScale(float start, float end, float t);
#endif