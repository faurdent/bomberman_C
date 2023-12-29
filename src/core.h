#ifndef BOMBERMAN_CORE_H
#define BOMBERMAN_CORE_H
#include <stdbool.h>
#include <stdlib.h>
#include <ncurses.h>

#include "wall.h"
#include "bomb.h"
#include "player.h"
#include "enemy.h"
#include "structs.h"

struct GameInfo {
    int score;
    int level;
    bool is_lost;
};

struct GameField {
    WINDOW *window;
    int height;
    int width;
    int bomb_range;
    int local_score;
};

struct Door {
    int x, y;
    bool is_visible;
};

long ranged_random(int, int);

void tick_game(
        GameField*, BreakableWall*, Door*,
        Player*, Enemy**, size_t, Bomb**
);

#endif //BOMBERMAN_CORE_H
