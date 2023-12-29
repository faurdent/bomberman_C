#include <stdlib.h>

#include "enemy.h"
#include "core.h"

int add_enemy(Enemy** enemies, Enemy **enemy, int x, int y, char position_char) {
    int id = 0;
    while (enemies[id] != NULL) {
        Enemy* temp = enemies[id++];
        if ((temp->x == x && temp->y == y) || (position_char == '#' || position_char == '$')) {
            return 0;
        }
    }
    Enemy* new_enemy = (Enemy*) malloc(sizeof(Enemy));
    new_enemy->x = x;
    new_enemy->y = y;
    *enemy = new_enemy;
    return 1;
}

void create_enemies(GameField *game_field, Enemy *enemies[], size_t enemies_count) {
    size_t generated_enemies = 0;
    int x = 0, y = 0;
    while (generated_enemies < enemies_count) {
        x = (int) ranged_random(1 ,game_field->width);
        y = (int) ranged_random(1, game_field->height);
        char position_char = mvwinch(game_field->window, y, x) & A_CHARTEXT;
        if (position_char == '#' || position_char == '$') {
            continue;
        }
        Enemy *enemy = NULL;
        int is_added = add_enemy(enemies, &enemy, x, y, position_char);
        if(is_added) {
            enemies[generated_enemies++] = enemy;
        }
    }
}

int get_direction_priority(char character) {
    switch (character) {
        case '*': return 0;
        case '@': return 2;
        case ' ':
        default: return 1;
    }
}

enum Directions choose_direction(GameField *gameField, Enemy* enemy) {
    int x = enemy->x, y = enemy->y;
    Directions directions[4];
    int priorities[4];
    for (int i = 0; i < 4; ++i) {
        directions[i] = -1;
        priorities[i] = 0;
    }
    int size = 0;
    char upper_char, bottom_char, left_char, right_char;
    upper_char = mvwinch(gameField->window, y - 1, x) & A_CHARTEXT;
    if ((y - 1) >= 1 && upper_char != '#' && upper_char != '$' && upper_char != 'E') {
        directions[size] = NORTH;
        priorities[size] = get_direction_priority(upper_char);
        size++;
    }
    bottom_char = mvwinch(gameField->window, y + 1, x) & A_CHARTEXT;
    if ((y + 1) <= gameField->height && bottom_char != '#' && bottom_char != '$' && bottom_char != 'E') {
        directions[size] = SOUTH;
        priorities[size] = get_direction_priority(bottom_char);
        size++;
    }
    left_char = mvwinch(gameField->window, y, x - 1) & A_CHARTEXT;
    if ((x - 1) >= 1  && left_char != '#' && left_char != '$' && left_char != 'E') {
        directions[size] = WEST;
        priorities[size] = get_direction_priority(left_char);
        size++;
    }
    right_char = mvwinch(gameField->window, y, x + 1) & A_CHARTEXT;
    if ((x + 1) <= gameField->width && right_char != '#' && right_char != '$' && right_char != 'E') {
        directions[size] = EAST;
        priorities[size] = get_direction_priority(right_char);
        size++;
    }
    if (size == 0) {
        return FAILED;
    }
    int id = 0;
    bool mid_priority_exists = false;
    while (id < size && directions[id] != FAILED) {
        if (priorities[id] == 2) {
            return directions[id];
        } else if (directions[id] == 1) {
            mid_priority_exists = true;
        }
        id++;
    }
    bool can_select_lowest = false;
    if (!mid_priority_exists) {
        can_select_lowest = true;
    }
    int random_number = 0;
    bool generated = false;
    while (!generated) {
        random_number = (int) ranged_random(0, size);
        if (priorities[random_number] == 0 && can_select_lowest) {
            return directions[random_number];
        } else if (priorities[random_number] != 0) {
            generated = true;
        }
    }
    return directions[random_number];
}

void move_enemy(Enemy *enemy, Directions direction) {
    switch (direction) {
        case NORTH:
            enemy->y = enemy->y - 1;
            break;
        case EAST:
            enemy->x = enemy->x + 1;
            break;
        case SOUTH:
            enemy->y = enemy->y + 1;
            break;
        case WEST:
            enemy->x = enemy->x - 1;
            break;
        default:
            break;
    }
}

void move_enemies(GameField *game_field, Player *player, Enemy** enemies, size_t enemies_count) {
    Enemy* enemy = NULL;
    for (size_t i = 0; i < enemies_count; ++i) {
        enemy = enemies[i];
        if (enemy != NULL) {
            enum Directions direction = choose_direction(game_field, enemy);
            if (direction == FAILED) { continue; }
            mvwaddch(game_field->window, enemy->y, enemy->x, ' ');
            move_enemy(enemy, direction);
            if (enemy->x == player->x && enemy->y == player->y) {
                player->is_dead = true;
                return;
            }
        }
    }
}

void kill_enemy(Enemy** enemies, int enemies_count, int x, int y) {
    Enemy* enemy = NULL;
    for (int i = 0; i < enemies_count; ++i) {
        enemy = enemies[i];
        if (enemy == NULL) continue;
        if (enemy->x == x && enemy->y == y) {
            enemies[i] = NULL;
            free(enemy);
            break;
        }
    }
}
