#ifndef BOMBERMAN_CLEAR_H
#define BOMBERMAN_CLEAR_H
#include <stdlib.h>

#include "player.h"
#include "enemy.h"
#include "wall.h"
#include "bomb.h"
#include "core.h"

void clear_enemies(Enemy**, size_t);

void clear_bombs(Bomb*);

void clear_walls(BreakableWall*);

void clear_local_variables(GameField*, BreakableWall*, Door*, Player*, Enemy **enemies, size_t, Bomb*);

#endif //BOMBERMAN_CLEAR_H
