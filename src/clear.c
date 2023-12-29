#include <stdlib.h>

#include "clear.h"

#include "player.h"
#include "enemy.h"
#include "wall.h"
#include "bomb.h"
#include "core.h"

void clear_enemies(Enemy *enemies[], size_t enemies_count) {
    for (size_t i = 0; i < enemies_count; ++i) {
        free(enemies[i]);
    }
}

void clear_bombs(Bomb* bombs) {
    Bomb* temp = NULL;
    while (bombs != NULL) {
        temp = bombs;
        bombs = bombs->next;
        free(temp);
    }
}

void clear_walls(BreakableWall *breakable_walls) {
    BreakableWall* temp = NULL;
    while (breakable_walls != NULL) {
        temp = breakable_walls;
        breakable_walls = breakable_walls->next;
        free(temp);
    }
}

void clear_local_variables(
        GameField *game_field, BreakableWall *breakable_walls, Door *door,
        Player *player, Enemy *enemies[], size_t enemies_count, Bomb *bombs
) {
    free(game_field);
    free(player);
    free(door);
    clear_enemies(enemies, enemies_count);
    clear_bombs(bombs);
    clear_walls(breakable_walls);
}
