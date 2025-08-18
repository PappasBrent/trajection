#include "constants.h"
#include "default_levels.h"
#include "globals.h"
#include "level.h"
#include "raylib.h"
#include "turn_circle.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

Levels levels;

Level *level = &levels.data[0];

typedef enum State {
    Title,
    In_Level,
    Win,
} State;

State state = Title;
Direction ball_direction = Right;
Vector2 ball_position = { 0 };
bool ball_fired = false;
bool should_quit = false;
bool default_levels = false;

void init(void) {
    Image finish_line_image = { 0 };
    InitWindow(WIDTH, HEIGHT, "Trajection");
    HideCursor();
    SetTargetFPS(60);
    init_globals();
}

void quit(void) {
    unload_globals();
    CloseWindow();
}

void draw(void) {
    Vector2 text_metrics = { 0 };

    BeginDrawing();

    ClearBackground(BACKGROUND_COLOR);

    switch (state) {
    case Title:
        text_metrics =
            MeasureTextEx(font, "Trajection", Font_Size_Large, FONT_SPACING);
        DrawTextEx(font, "Trajection",
                   (Vector2){
                       .x = (float)WIDTH / 2 - text_metrics.x / 2,
                       .y = CELL_SIZE * 4,
                   },
                   Font_Size_Large, FONT_SPACING, BLACK);

        text_metrics = MeasureTextEx(font, "Click to start", Font_Size_Medium,
                                     FONT_SPACING);
        DrawTextEx(font, "Click to start",
                   (Vector2){
                       .x = (float)WIDTH / 2 - text_metrics.x / 2,
                       .y = CELL_SIZE * 8,
                   },
                   Font_Size_Medium, FONT_SPACING, BLACK);
        break;
    case In_Level:
        draw_level(*level, false);
        if (!ball_fired) {
            DrawTextEx(font, "Click to launch",
                       (Vector2){
                           .x = CELL_SIZE,
                           .y = HEIGHT - CELL_SIZE * 2,
                       },
                       Font_Size_Medium, FONT_SPACING, BLACK);
        }
        DrawTextEx(
            font, TextFormat("Level %d/%d", (level - levels.data + 1), NLEVELS),
            (Vector2){
                .x = (float)CELL_SIZE / 2,
                .y = (float)CELL_SIZE / 2,
            },
            Font_Size_Small, FONT_SPACING, BLACK);
        if (default_levels) {
            // Print tutorial text if we are in the default levels.
            // TODO(Brent): Add text placement to the level editor.
            if (levels.data == level) {
                DrawTextEx(font, "Reach the finish line\nto beat the level",
                           (Vector2){ .x = CELL_SIZE * 3, .y = CELL_SIZE * 4 },
                           Font_Size_Medium, FONT_SPACING, BLACK);
            }
            if (levels.data + 1 == level) {
                DrawTextEx(
                    font,
                    "When the ball is\nin a circle,\nclick to change\nthe ball's direction",
                    (Vector2){ .x = CELL_SIZE * 1, .y = CELL_SIZE * 8 },
                    Font_Size_Medium, FONT_SPACING, BLACK);
            }
            if (levels.data + 3 == level) {
                DrawTextEx(font, "Watch out\nfor walls",
                           (Vector2){ .x = CELL_SIZE * 17,
                                      .y = CELL_SIZE * 12 },
                           Font_Size_Medium, FONT_SPACING, BLACK);
            }
            if (levels.data + 4 == level) {
                DrawTextEx(font, "Good luck!",
                           (Vector2){ .x = CELL_SIZE * 17, .y = CELL_SIZE * 2 },
                           Font_Size_Medium, FONT_SPACING, BLACK);
            }
        }
        break;
    case Win:
        text_metrics =
            MeasureTextEx(font, "You win!", Font_Size_Large, FONT_SPACING);
        DrawTextEx(font, "You win!",
                   (Vector2){
                       .x = (float)WIDTH / 2 - text_metrics.x / 2,
                       .y = CELL_SIZE * 8,
                   },
                   Font_Size_Large, FONT_SPACING, BLACK);
        text_metrics = MeasureTextEx(font, "Click to return to title",
                                     Font_Size_Medium, FONT_SPACING);
        DrawTextEx(font, "Click to return to title",
                   (Vector2){
                       .x = (float)WIDTH / 2 - text_metrics.x / 2,
                       .y = CELL_SIZE * 12,
                   },
                   Font_Size_Medium, FONT_SPACING, BLACK);
        break;
    }

    if (ball_fired) {
        DrawCircle(ball_position.x, ball_position.y, BALL_RADIUS, BALL_COLOR);
    }

    EndDrawing();
}

void update(void) {
    Turn_Circle *tc = 0;
    Vector2 *v = 0;

    if (Title == state) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            level = &levels.data[0];
            state = In_Level;
        }
        return;
    } else if (Win == state) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state = Title;
        }
        return;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (!ball_fired) {
            ball_fired = true;
            ball_direction = level->cannon.direction;
            ball_position = get_cannon_fire_position(level->cannon);
            /* Make sure ball is centered. */
            ball_position.x += (float)BALL_RADIUS;
            ball_position.y += (float)BALL_RADIUS;
        } else {
            for (tc = level->turn_circles;
                 tc < level->turn_circles + level->nturn_circles; ++tc) {
                if (CheckCollisionPointCircle(ball_position, tc->position,
                                              tc->radius)) {
                    ball_direction = tc->direction;
                }
            }
        }
    }

    if (ball_fired) {
        switch (ball_direction) {
        case Left:
            ball_position.x -= BALL_SPEED;
            if (ball_position.x < -BALL_RADIUS) {
                ball_fired = false;
            }
            break;
        case Right:
            ball_position.x += BALL_SPEED;
            if (ball_position.x > WIDTH + BALL_RADIUS) {
                ball_fired = false;
            }
            break;
        case Up:
            ball_position.y -= BALL_SPEED;
            if (ball_position.y < -BALL_RADIUS) {
                ball_fired = false;
            }
            break;
        case Down:
            ball_position.y += BALL_SPEED;
            if (ball_position.y > HEIGHT + BALL_RADIUS) {
                ball_fired = false;
            }
            break;
        }

        for (v = level->wall_positions;
             v < level->wall_positions + level->nwalls; ++v) {
            if (CheckCollisionCircleRec(ball_position, BALL_RADIUS,
                                        (Rectangle){
                                            .x = v->x,
                                            .y = v->y,
                                            .width = CELL_SIZE,
                                            .height = CELL_SIZE,
                                        })) {
                ball_fired = false;
            }
        }

        for (v = level->goal_positions;
             v < level->goal_positions + level->ngoals; ++v) {
            if (CheckCollisionCircleRec(ball_position, BALL_RADIUS,
                                        (Rectangle){
                                            .x = v->x,
                                            .y = v->y,
                                            .width = CELL_SIZE,
                                            .height = CELL_SIZE,
                                        })) {
                ball_fired = false;
                if (level < levels.data + NLEVELS - 1) {
                    ++level;
                    printf("Incrementing level");
                    break;
                } else {
                    state = Win;
                }
            }
        }
    }
}

void update_and_draw_frame(void) {
    update();
    draw();
}

int main(int argc, char **argv) {
    unsigned char *filedata = 0;
    char *filename = 0;
    int levels_size = sizeof(levels);

    if (argc > 2) {
        fprintf(stderr, "USAGE: trajection FILENAME\n");
        return EXIT_FAILURE;
    }

    if (1 == argc) {
        levels = load_default_levels();
        level = levels.data;
        default_levels = true;
    } else {
        filename = argv[1];
        if (!FileExists(filename)) {
            fprintf(stderr, "Could not load level file %s\n", filename);
            return EXIT_FAILURE;
        }

        filedata = LoadFileData(filename, &levels_size);
        levels = *(Levels *)filedata;
        UnloadFileData(filedata);
        default_levels = false;
    }

    init();

    draw();

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(update_and_draw_frame, 0, 1);
#else
    do {
        update_and_draw_frame();
    } while (!(WindowShouldClose() || should_quit));
#endif

    quit();

    return EXIT_SUCCESS;
}
