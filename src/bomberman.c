#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#include "bomberman.h"
#include "init.h"
#include "core.h"
#include "player.h"
#include "bomb.h"
#include "display.h"
#include "clear.h"

#define ENEMIES_RATIO 180

void bomberman() {
    srand(time(NULL));
    initscr();
    curs_set(FALSE);
    cbreak();
    noecho();
    init_colors();
    wbkgd(stdscr, COLOR_PAIR(5));
    bool restart_game = true;
    while (restart_game) {
        halfdelay(5);
        restart_game = false;
        GameInfo *game_info = init_game();
        do {
            game_info->level++;
            int height = (int) ranged_random(15, LINES - 6);
            int width = (int) ranged_random(49, COLS - 6);
            keypad(stdscr, TRUE);
            height = height % 2 == 1 ? height : height - 1;
            width = width % 2 == 1 ? width : width - 1;
            int start_x, start_y;
            GameField *game_field = init_field(width, height, &start_x, &start_y);
            WINDOW *info_window = newwin(2, width, start_y - 2, start_x);
            wbkgd(game_field->window, COLOR_PAIR(5));
            wbkgd(info_window, COLOR_PAIR(5));
            refresh();
            box(game_field->window, 0, 0);
            display_grid(game_field);
            int walls_count = 0;
            BreakableWall *breakableWalls = generate_breakable_walls(game_field, &walls_count);
            Door *door = init_door(breakableWalls, walls_count);
            display_door(game_field, door);
            display_breakable_walls(game_field, breakableWalls);
            int empty_tiles = (game_field->height * game_field->width) - walls_count;
            int enemies_count = empty_tiles / ENEMIES_RATIO;

            Enemy *enemies[enemies_count];
            for (int i = 0; i < enemies_count; ++i) {
                enemies[i] = NULL;
            }
            Bomb *bombs = NULL;

            create_enemies(game_field, enemies, enemies_count);
            display_enemies(game_field, enemies, enemies_count);
            Player *player = create_player(game_field);
            if (player == NULL) {
                game_info->level--;
                continue;
            }
            display_player(game_field, player);
            wrefresh(info_window);
            wrefresh(game_field->window);
            int player_input;
            enum MoveResult move_result = STAND;
            bool make_tick = true;
            while (!player->is_dead && !player->is_door_found) {
                display_game_info(info_window, game_info, game_field);
                display_bombs(game_field, bombs);
                display_player(game_field, player);
                display_enemies(game_field, enemies, enemies_count);
                display_door(game_field, door);
                display_breakable_walls(game_field, breakableWalls);
                wrefresh(game_field->window);
                wrefresh(info_window);
                player_input = getch();
                if (player_input) napms(80);
                switch (player_input) {
                    case KEY_UP:
                        move_result = move_player(game_field, player, 0, 1);
                        break;
                    case KEY_DOWN:
                        move_result = move_player(game_field, player, 0, -1);
                        break;
                    case KEY_RIGHT:
                        move_result = move_player(game_field, player, -1, 0);
                        break;
                    case KEY_LEFT:
                        move_result = move_player(game_field, player, 1, 0);
                        break;
                    case 'b':
                        make_tick = false;
                        add_bomb(&bombs, player->x, player->y);
                        break;
                    case 'q':
                        clear_local_variables(game_field, breakableWalls, door, player, enemies, enemies_count, bombs);
                        free(game_info);
                        endwin();
                        return;
                    default:
                        break;
                }
                switch (move_result) {
                    case LOST:
                        player->is_dead = true;
                        break;
                    case FAILURE:
                        move_result = STAND;
                        continue;
                    case SUCCESS:
                        move_result = STAND;
                        display_player(game_field, player);
                        display_bombs(game_field, bombs);
                        break;
                    case WIN:
                        player->is_door_found = true;
                        continue;
                    default:
                        break;
                }
                if (make_tick) {
                    tick_game(game_field, breakableWalls, door, player, enemies, enemies_count, &bombs);
                }
                wrefresh(game_field->window);
                make_tick = true;
            }
            if (player->is_dead) {
                game_info->is_lost = true;
            }
            game_info->score += game_field->local_score;
            clear();
            wclear(info_window);
            delwin(game_field->window);
            delwin(info_window);
            clear_local_variables(game_field, breakableWalls, door, player, enemies, enemies_count, bombs);
        } while (game_info->level < 5 && game_info->is_lost == false);
        refresh();
        WINDOW *results_window = newwin(8, 51, (LINES - 5) / 2, (COLS - 51) / 2);
        if (game_info->is_lost) {
            display_lost_window(results_window, game_info);
        } else {
            display_won_window(results_window, game_info);
        }
        
        mvwprintw(results_window, 7, (51 - 43) / 2, "To restart press 'r'  To quit press any key");
        refresh();
        wbkgd(results_window, COLOR_PAIR(5));
        wrefresh(results_window);
        cbreak();
        nodelay(stdscr, FALSE);
        nodelay(results_window, FALSE);
        int input = getch();
        if (input == 'r') {
            restart_game = true;
            wclear(results_window);
            wrefresh(results_window);
            delwin(results_window);
        }
        free(game_info);
        refresh();
    }
    endwin();
    return;
}

