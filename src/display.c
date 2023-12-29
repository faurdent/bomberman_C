#include <ncurses.h>
#include <stdlib.h>

#include "display.h"
#include "core.h"
#include "player.h"
#include "wall.h"

void display_grid(GameField *game_field) {
    for (int i = 2; i < game_field->height; i += 2) {
        for (int j = 2; j < game_field->width; j += 2) {
            mvwaddch(game_field->window, i, j, '#');
        }
    }
}

void display_breakable_walls(GameField *game_field, BreakableWall *breakable_walls) {
    wattron(game_field->window, COLOR_PAIR(3));
    while (breakable_walls != NULL) {
        mvwaddch(game_field->window, breakable_walls->y, breakable_walls->x, '$');
        breakable_walls = breakable_walls->next;
    }
    wattroff(game_field->window, COLOR_PAIR(3));
}

void display_player(GameField *game_field, Player *player) {
    wattron(game_field->window, COLOR_PAIR(2));
    mvwprintw(game_field->window, player->y, player->x, "@");
    wattroff(game_field->window, COLOR_PAIR(2));
}

void display_enemies(GameField *game_field, Enemy* enemies[], size_t enemies_count) {
    Enemy* temp = NULL;
    wattron(game_field->window, COLOR_PAIR(1));
    for (size_t i = 0; i < enemies_count; ++i) {
        temp = enemies[i];
        if (temp == NULL) continue;
        mvwaddch(game_field->window, temp->y, temp->x, 'E');
    }
    wattroff(game_field->window, COLOR_PAIR(1));
}

void display_bombs(GameField *game_field, Bomb *bombs) {
    if (bombs == NULL) {
        return;
    }
    Bomb *temp = bombs;
    wattron(game_field->window, COLOR_PAIR(4));
    while (temp != NULL) {
        mvwaddch(game_field->window, temp->y, temp->x, '*');
        temp = temp->next;
    }
    wattroff(game_field->window, COLOR_PAIR(4));
}

void display_door(GameField *game_field, Door *door) {
    if (door->is_visible) {
        mvwaddch(game_field->window, door->y, door->x, '=');
    }
}

void display_game_info(WINDOW *info_window, GameInfo *game_info, GameField *game_field) {
    wclear(info_window);
    mvwprintw(info_window, 1, 1, "Score: %d\tLevel: %d", game_field->local_score + game_info->score, game_info->level);
}

void display_won_window(WINDOW *result_window, GameInfo* gameInfo) {
    wattron(result_window, COLOR_PAIR(2));
    mvwprintw(result_window, 1, (51 - 25) / 2, "Congratulations! You won!");
    mvwprintw(result_window, 3, (51 - 13) / 2, "Your score: %d", gameInfo->score);
    wattroff(result_window, COLOR_PAIR(2));
}

void display_lost_window(WINDOW *result_window, GameInfo* gameInfo) {
    wattron(result_window, COLOR_PAIR(1));
    mvwprintw(result_window, 1, (51 - 15) / 2, "Oh, you lost :(");
    mvwprintw(result_window, 3, (51 - 13) / 2, "Your score: %d", gameInfo->score);
    mvwprintw(result_window, 3, (51 - 13) / 2, "Your level: %d", gameInfo->level);
    wattroff(result_window, COLOR_PAIR(1));
}
