#include <stdlib.h>
#include <ncurses.h>

#include "core.h"
#include "bomb.h"
#include "player.h"
#include "wall.h"
#include "enemy.h"


Bomb* find_bomb(Bomb *bombs, int x, int y) {
    Bomb *temp = bombs;
    while (temp != NULL) {
        if (temp->x == x && temp->y == y) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void clear_direction(
        GameField *game_field, BreakableWall *breakableWalls, Door *door,
        Player *player, Enemy* enemies[], size_t enemies_count,
        int x, int y, int x_direction, int y_direction
) {
    int bomb_range = game_field->bomb_range;
    char position_char;
    for (int i = 1; i < bomb_range + 1; ++i) {
        position_char = mvwinch(game_field->window, y - i * y_direction, x + i * x_direction) & A_CHARTEXT;
        switch (position_char) {
            case '@':
                player->is_dead=true;
                return;
            case 'E':
                kill_enemy(enemies, enemies_count, x + i * x_direction, y - i * y_direction);
                door->is_visible = true;
                game_field->local_score += 20;
                mvwaddch(game_field->window, y - i * y_direction, x + i * x_direction, ' ');
                break;
            case '$':
                break_wall(&breakableWalls, x + i * x_direction, y - i * y_direction);
                mvwaddch(game_field->window, y - i * y_direction, x + i * x_direction, ' ');
                break;
            case '#':
                return;
            default:
                break;
        }
    }
}

void explode_single_bomb(
        GameField *game_field, BreakableWall *breakableWalls, Door *door,
        Player *player, Enemy* enemies[], size_t enemies_count, Bomb *bomb
){
    int x = bomb->x, y = bomb->y;
    char position_char = mvwinch(game_field->window, y, x) & A_CHARTEXT;
    switch (position_char) {
        case '@':
            player->is_dead = true;
            return;
        case 'E':
            kill_enemy(enemies, enemies_count, x, y);
            door->is_visible = true;
            game_field->local_score += 20;
            break;
        default:
            break;
    }
    clear_direction(game_field, breakableWalls, door, player, enemies, enemies_count, x, y,0, 1);
    clear_direction(game_field, breakableWalls, door, player, enemies, enemies_count, x, y,0, -1);
    clear_direction(game_field, breakableWalls, door, player, enemies, enemies_count, x, y,1, 0);
    clear_direction(game_field, breakableWalls, door, player, enemies, enemies_count, x, y,-1, 0);
}

void explode_bombs(
        GameField *game_field, BreakableWall *breakableWalls, Door *door,
        Player *player, Enemy* enemies[], size_t enemies_count, Bomb **bombs
) {
    if (*bombs == NULL) {
        return;
    }
    Bomb *bomb = *bombs;
    Bomb *temp = NULL;
    if(bomb->next == NULL) {
        if (bomb->till_explode <= 0) {
            explode_single_bomb(game_field, breakableWalls, door, player, enemies, enemies_count, bomb);
            mvwaddch(game_field->window, bomb->y, bomb->x, ' ');
            *bombs = NULL;
            free(bomb);
        }
        return;
    }
    while(bomb->next != NULL) {
        temp = bomb->next;
        refresh();
        if(bomb->till_explode <= 0) {
            explode_single_bomb(game_field, breakableWalls, door, player, enemies, enemies_count, bomb);
            mvwaddch(game_field->window, bomb->y, bomb->x, ' ');
            if (bomb->prev != NULL) {
                bomb->next->prev = bomb->next;
            } else {
                *bombs = bomb->next;
            }
            if (bomb->next != NULL) {
                bomb->next->prev = bomb->prev;
            }
            free(bomb);
        }
        bomb = temp;
    }
}

void add_bomb(Bomb **bombs, int x, int y) {
    Bomb *new_bomb = (Bomb*) malloc(sizeof(Bomb));
    new_bomb->x = x; new_bomb->y = y;
    new_bomb->till_explode = 5;
    new_bomb->next = NULL;
    if (find_bomb(*bombs, x, y) != NULL) {
        return;
    }
    if (*bombs == NULL) {
        new_bomb->prev = NULL;
        *bombs = new_bomb;
        return;
    }
    Bomb *temp = *bombs;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_bomb;
    new_bomb->prev = temp;
}

void decrease_bomb_explosion_time(Bomb **bombs) {
    Bomb *temp = *bombs;
    while (temp != NULL) {
        temp->till_explode--;
        temp = temp->next;
    }
}
