#ifndef BOMBERMAN_WALL_H
#define BOMBERMAN_WALL_H

#include "core.h"
#include "structs.h"

struct BreakableWall {
    int x, y;
    struct BreakableWall *prev;
    struct BreakableWall *next;
};

void add_breakable_wall(BreakableWall**, int, int);

BreakableWall* generate_breakable_walls(GameField*, int*);

void break_wall(BreakableWall **, int, int);



#endif //BOMBERMAN_WALL_H
