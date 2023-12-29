#include <stdlib.h>
#include <ncurses.h>

#include "core.h"
#include "player.h"

bool enemies_near_present(WINDOW *window, int x, int y) {
    for (int i = x - 6; i < x + 6; ++i) {
        if (i < 0) {
            continue;
        }
        if ((mvwinch(window, y, i) & A_CHARTEXT) == 'E') {
            return true;
        }
    }
    for (int i = y - 3; i < y + 3; ++i) {
        if (i < 0) {
            continue;
        }
        if ((mvwinch(window, i, x) & A_CHARTEXT) == 'E') {
            return true;
        }
    }
    return false;
}

bool is_enough_space(GameField *game_field, int x, int y) {
    char position_char;
    bool is_suitable = true;
    for (int i = x - 2; i < x + 3; ++i) {
        if (i < 1 || i > game_field->width) continue;
        position_char = mvwinch(game_field->window, y, i) & A_CHARTEXT;
        if(position_char == '$' || position_char == '#') {
            is_suitable = false;
            break;
        }
    }
    if (is_suitable) {
        return true;
    }
    is_suitable = true;
    for (int i = y - 2; i < y + 3; ++i) {
        if (i < 1 || i > game_field->height) continue;
        position_char = mvwinch(game_field->window, i, x) & A_CHARTEXT;
        if(position_char == '$' || position_char == '#') {
            is_suitable = false;
            break;
        }
    }
    return is_suitable;
}

Player* create_player(GameField *game_field) {
    Player* player = (Player*) malloc(sizeof(Player));
    bool generated = false;
    int x, y;
    int counter = 0;
    while (!generated && counter < 1000) {
        x = (int) ranged_random(1, game_field->width);
        y = (int) ranged_random(1, game_field->height);
        if (!enemies_near_present(game_field->window, x, y) && is_enough_space(game_field, x, y)) {
            generated = true;
        }
        counter++;
    }
    if (counter >= 1000) {
        return NULL;
    }
    player->x = x;
    player->y = y;
    player->is_dead = false;
    player->is_door_found = false;
    return player;
}

enum MoveResult move_player(GameField *game_field, Player *player, int x_change, int y_change) {
    int new_x = player->x - x_change, new_y = player->y - y_change;
    char to_move_char = mvwinch(game_field->window, new_y, new_x) & A_CHARTEXT;
    if (to_move_char == 'E') {
        mvprintw(7, 1, "Stepped on enemy");
        refresh();
        return LOST;
    }
    if (to_move_char == '=') {
        return WIN;
    }
    bool in_borders = ((new_x <= game_field->width && new_x > 0) && (new_y <= game_field->height && new_y > 0));
    if ((to_move_char == '$' || to_move_char == '#') || !in_borders) {
        return FAILURE;
    }
    mvwaddch(game_field->window, player->y, player->x, ' ');
    player->x = new_x; player->y = new_y;
//    display_player(game_field, player); TODO: MOVE IT AND TEST
    wrefresh(game_field->window);
    return SUCCESS;
}
