#include <stdlib.h>

#include "wall.h"
#include "core.h"

void add_breakable_wall(BreakableWall** breakable_walls, int x, int y) {
    BreakableWall *new_wall = (BreakableWall*) malloc(sizeof(BreakableWall));
    new_wall->x = x;
    new_wall->y = y;
    new_wall->next = NULL;
    BreakableWall *temp = *breakable_walls;
    if (*breakable_walls == NULL) {
        new_wall->prev = NULL;
        *breakable_walls = new_wall;
        return;
    }
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_wall;
    new_wall->prev = temp;
}

BreakableWall* generate_breakable_walls(GameField *game_field, int *walls_count) {
    BreakableWall *breakable_walls;
    int counter = 0;
    for (int i = 1; i < game_field->height + 1; i += 2) {
        for (int j = 1; j < game_field->width + 1; ++j) {
            if (ranged_random(1, 3) == 2) {
                add_breakable_wall(&breakable_walls, j, i);
                counter++;
            }
        }
    }
    for (int i = 2; i < game_field->height; i += 2) {
        for (int j = 1; j < game_field->width + 1; j += 2) {
            if (ranged_random(1, 3) == 2) {
                add_breakable_wall(&breakable_walls, j, i);
                counter++;
            }
        }
    }
    *walls_count = counter;
    return breakable_walls;
}

void break_wall(BreakableWall **breakableWalls, int x, int y) {
    BreakableWall *wall = *breakableWalls;
    BreakableWall *temp = NULL;
    while (wall != NULL) {
        temp = wall->next;
        if (wall->x == x && wall->y == y) {
            if (wall->prev != NULL) {
                wall->prev->next = wall->next;
            } else {
                *breakableWalls = wall->next;
            }
            if (wall->next != NULL) {
                wall->next->prev = wall->prev;
            }
            free(wall);
            return;
        }
        wall = temp;
    }
}
