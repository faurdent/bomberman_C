#ifndef BOMBERMAN_BOMB_H
#define BOMBERMAN_BOMB_H
#include <stdlib.h>

#include "player.h"
#include "wall.h"
#include "enemy.h"
#include "core.h"
#include "structs.h"

struct Bomb {
    int x, y;
    int till_explode;
    struct Bomb *prev;
    struct Bomb *next;
};

Bomb* find_bomb(Bomb*, int, int);

void clear_direction(GameField*, BreakableWall*, Door*, Player*, Enemy**, size_t, int, int, int, int);

void explode_single_bomb(GameField*, BreakableWall*, Door*, Player*, Enemy**, size_t, Bomb*);

void explode_bombs(GameField*, BreakableWall*, Door*, Player*, Enemy**, size_t, Bomb**);

void add_bomb(Bomb**, int, int);

void decrease_bomb_explosion_time(Bomb**);

#endif //BOMBERMAN_BOMB_H
