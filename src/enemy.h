#ifndef BOMBERMAN_ENEMY_H
#define BOMBERMAN_ENEMY_H
#include <stdbool.h>
#include <stdlib.h>

#include "core.h"
#include "structs.h"

enum Directions {
    NORTH,
    EAST,
    SOUTH,
    WEST,
    FAILED
};

struct Enemy{
    int x, y;
};

int add_enemy(Enemy**, Enemy**, int, int, char);

void create_enemies(GameField*, Enemy**, size_t);

int get_direction_priority(char);

Directions choose_direction(GameField*, Enemy*);

void move_enemy(Enemy*, Directions);

void move_enemies(GameField*, Player*, Enemy**, size_t);

void kill_enemy(Enemy**, int, int, int);

#endif //BOMBERMAN_ENEMY_H
