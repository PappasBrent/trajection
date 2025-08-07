#include "level.h"
#include "cannon.h"
#include "constants.h"
#include "globals.h"
#include "raylib.h"
#include "turn_circle.h"
#include <stdbool.h>

void draw_level(Level level, bool debug) {
    Turn_Circle *tc = 0;
    Vector2 *v = 0;

    for (tc = level.turn_circles; tc < level.turn_circles + level.nturn_circles;
         ++tc) {
        draw_turn_circle(*tc, false);
    }

    for (v = level.wall_positions; v < level.wall_positions + level.nwalls;
         ++v) {
        DrawTextureRec(
            spritesheet,
            (Rectangle){
                .x = 0, .y = 0, .width = CELL_SIZE, .height = CELL_SIZE },
            *v, WHITE);
    }

    if (level.cannon_placed) {
        draw_cannon(level.cannon, debug);
    }

    for (v = level.goal_positions; v < level.goal_positions + level.ngoals;
         ++v) {
            DrawTexture(finish_line_texture, v->x, v->y, WHITE);
    }
}
