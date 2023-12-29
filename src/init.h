#ifndef BOMBERMAN_INIT_H
#define BOMBERMAN_INIT_H

#include "core.h"
#include "wall.h"

Door* init_door(BreakableWall*, int);

GameInfo* init_game();

GameField* init_field(int, int, int*, int*);

void init_colors();

#endif //BOMBERMAN_INIT_H
