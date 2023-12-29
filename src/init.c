#include <ncurses.h>

#include "init.h"

#include "core.h"
#include "wall.h"

Door* init_door(BreakableWall *breakable_walls, int walls_count) {
    Door *door = (Door*) malloc(sizeof(Door));
    door->is_visible = false;
    int random_wall_number = (int)ranged_random(0, walls_count);
    BreakableWall *temp = breakable_walls;
    while (random_wall_number > 0) {
        temp = temp->next;
        random_wall_number--;
    }
    door->x = temp->x; door->y = temp->y;
    return door;
}

GameInfo* init_game() {
    GameInfo* gameInfo = (GameInfo*) malloc(sizeof(GameInfo));
    gameInfo->level = 0;
    gameInfo->score = 0;
    gameInfo->is_lost = false;
    return gameInfo;
}

GameField* init_field(int width, int height, int *start_x, int *start_y) {
    *start_x = (COLS - width) / 2;
    *start_y = (LINES - height) / 2;
    GameField *game_field = (GameField*) malloc(sizeof(GameField));
    game_field->window = newwin(height, width, *start_y, *start_x);
    game_field->height = height - 2;
    game_field->width = width - 2;
    game_field->bomb_range = 2;
    game_field->local_score = 0;
    return game_field;
}

void init_colors() {
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_BLACK);
}
