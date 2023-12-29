#ifndef BOMBERMAN_DISPLAY_H
#define BOMBERMAN_DISPLAY_H

#include "core.h"
#include "structs.h"

void display_grid(GameField*);

void display_breakable_walls(GameField*, BreakableWall*);

void display_player(GameField*, Player*);

void display_enemies(GameField*, Enemy**, size_t);

void display_bombs(GameField*, Bomb*);

void display_door(GameField*, Door*);

void display_game_info(WINDOW*, GameInfo*, GameField*);

void display_won_window(WINDOW*, GameInfo*);

void display_lost_window(WINDOW*, GameInfo*);

#endif //BOMBERMAN_DISPLAY_H
