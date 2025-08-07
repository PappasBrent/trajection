#include "cannon.h"
#include "constants.h"
#include "direction.h"
#include "globals.h"
#include "raylib.h"
#include <stdbool.h>

void draw_cannon(Cannon cannon, bool debug) {
    Vector2 dot_position = get_cannon_fire_position(cannon);
    Rectangle source_rect = { 0 };
    switch (cannon.direction) {
    case Left:
        source_rect = (Rectangle){
            .x = 0,
            .y = CELL_SIZE * 2,
            .width = -CELL_SIZE,
            .height = CELL_SIZE,
        };
        break;
    case Right:
        source_rect = (Rectangle){
            .x = 0,
            .y = CELL_SIZE * 2,
            .width = CELL_SIZE,
            .height = CELL_SIZE,
        };
        break;
    case Up:
        source_rect = (Rectangle){
            .x = CELL_SIZE,
            .y = CELL_SIZE * 2,
            .width = CELL_SIZE,
            .height = -CELL_SIZE,
        };
        break;
    case Down:
        source_rect = (Rectangle){
            .x = CELL_SIZE,
            .y = CELL_SIZE * 2,
            .width = CELL_SIZE,
            .height = CELL_SIZE,
        };
        break;
    }
    DrawTextureRec(spritesheet, source_rect, cannon.position, WHITE);
    if (debug) {
        DrawRectangle(dot_position.x, dot_position.y, CELL_SIZE / 4,
                      CELL_SIZE / 4, RED);
    }
}

Vector2 get_cannon_fire_position(Cannon cannon) {
    const float DOT_SIZE = (float)CELL_SIZE / 4;
    switch (cannon.direction) {
    case Left:
    case Right:
        return (Vector2){
            .x = cannon.position.x +
                 (CELL_SIZE - DOT_SIZE) * (cannon.direction == Right),
            .y = cannon.position.y + (float)CELL_SIZE / 2 - DOT_SIZE / 2,
        };
    case Up:
    case Down:
        return (Vector2){
            .x = cannon.position.x + (float)CELL_SIZE / 2 - DOT_SIZE / 2,
            .y = cannon.position.y +
                 (CELL_SIZE - DOT_SIZE) * (cannon.direction == Down),
        };
    }
}
