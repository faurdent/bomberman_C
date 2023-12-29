#ifndef BOMBERMAN_PLAYER_H
#define BOMBERMAN_PLAYER_H
#include <stdbool.h>
#include <ncurses.h>
#include "core.h"
#include "structs.h"

enum MoveResult {
    LOST,
    FAILURE,
    STAND,
    SUCCESS,
    WIN
};

struct Player {
    int x, y;
    bool is_dead;
    bool is_door_found;
};

bool enemies_near_present(WINDOW*, int, int);

bool is_enough_space(GameField*, int, int);

Player* create_player(GameField*);

enum MoveResult move_player(GameField*, Player*, int, int);

#endif //BOMBERMAN_PLAYER_H
