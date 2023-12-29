#include <stdlib.h>
#include <ncurses.h>

#include "core.h"
#include "display.h"
#include "enemy.h"

long ranged_random(int min, int max) {
    unsigned long
    num_bins = (unsigned long) (max - min) + 1,
            num_rand = (unsigned long) RAND_MAX + 1,
            bin_size = num_rand / num_bins,
            defect   = num_rand % num_bins;
    long x;
    do {
        x = random();
    }
    while (num_rand - defect <= (unsigned long)x);

    return min + (x / bin_size);
}

void tick_game(
        GameField *game_field, BreakableWall *breakableWalls, Door *door,
        Player *player, Enemy* enemies[], size_t enemies_count, Bomb **bombs
) {
    display_bombs(game_field, *bombs);
    display_player(game_field, player);
    display_enemies(game_field, enemies, enemies_count);
    decrease_bomb_explosion_time(bombs);
    explode_bombs(game_field, breakableWalls, door, player, enemies, enemies_count, bombs);
    move_enemies(game_field, player, enemies, enemies_count);
    if (player->is_dead) {
        return;
    }
    display_bombs(game_field, *bombs);
    display_enemies(game_field, enemies, enemies_count);
}
