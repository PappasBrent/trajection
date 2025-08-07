#include "cannon.h"
#include "constants.h"
#include "globals.h"
#include "level.h"
#include "turn_circle.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Levels levels;

Level *level = &levels.data[0];

typedef enum State {
    Normal,
    Erasing,
    Placing_Circle,
    Placing_Wall,
    Placing_Cannon,
    Placing_Goal,
} State;
State state;

Direction tentative_direction = Left;
Vector2 cursor_position = { 0 };
int tentative_radius = CELL_SIZE;

Rectangle get_eraser_rect(void) {
    return (Rectangle){ .x = cursor_position.x + (float)CELL_SIZE / 4,
                        .y = cursor_position.y + (float)CELL_SIZE / 4,
                        .width = (float)CELL_SIZE / 2,
                        .height = (float)CELL_SIZE / 2 };
}

void init(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Trajection");
    HideCursor();
    SetTargetFPS(60);
    init_globals();
}

void quit(void) {
    unload_globals();
    CloseWindow();
}

Vector2 snap_to_grid(Vector2 position) {
    return (Vector2){
        .x = (int)position.x / CELL_SIZE * CELL_SIZE, // NOLINT
        .y = (int)position.y / CELL_SIZE * CELL_SIZE, // NOLINT
    };
}

void update(void) {
    float mouse_wheel_move;
    Turn_Circle *tc = NULL;
    Vector2 *v = NULL;

    cursor_position = snap_to_grid(GetMousePosition());

    if (IsKeyPressed(KEY_E)) {
        state = Erasing;
    }
    if (IsKeyPressed(KEY_C)) {
        state = Placing_Circle;
    }
    if (IsKeyPressed(KEY_W)) {
        state = Placing_Wall;
    }
    if (IsKeyPressed(KEY_N)) {
        state = Placing_Cannon;
    }
    if (IsKeyPressed(KEY_F)) {
        state = Placing_Goal;
    }

    if (IsKeyPressed(KEY_RIGHT) && level != levels.data + NLEVELS - 1) {
        ++level;
    }
    if (IsKeyPressed(KEY_LEFT) && level != levels.data) {
        --level;
    }

    if (IsKeyPressed(KEY_ONE)) {
        tentative_direction = Left;
    }
    if (IsKeyPressed(KEY_TWO)) {
        tentative_direction = Right;
    }
    if (IsKeyPressed(KEY_THREE)) {
        tentative_direction = Up;
    }
    if (IsKeyPressed(KEY_FOUR)) {
        tentative_direction = Down;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        switch (state) {
        case Normal:
            break;
        case Erasing:
            for (tc = level->turn_circles;
                 tc < level->turn_circles + level->nturn_circles; ++tc) {
                if (CheckCollisionCircleRec(tc->position, tc->radius,
                                            get_eraser_rect())) {
                    *tc = level->turn_circles[--level->nturn_circles];
                    break;
                }
            }
            for (v = level->wall_positions;
                 v < level->wall_positions + level->nwalls; ++v) {
                if (CheckCollisionRecs(get_eraser_rect(),
                                       (Rectangle){
                                           .x = v->x,
                                           .y = v->y,
                                           .width = CELL_SIZE,
                                           .height = CELL_SIZE,
                                       })) {
                    *v = level->wall_positions[--level->nwalls];
                    break;
                }
            }
            for (v = level->goal_positions;
                 v < level->goal_positions + level->ngoals; ++v) {
                if (CheckCollisionRecs(get_eraser_rect(),
                                       (Rectangle){
                                           .x = v->x,
                                           .y = v->y,
                                           .width = CELL_SIZE,
                                           .height = CELL_SIZE,
                                       })) {
                    *v = level->goal_positions[--level->ngoals];
                    break;
                }
            }
            if (level->cannon_placed &&
                CheckCollisionRecs(get_eraser_rect(),
                                   (Rectangle){ .x = level->cannon.position.x,
                                                level->cannon.position.y,
                                                CELL_SIZE,
                                                CELL_SIZE })) {
                level->cannon_placed = false;
            }
            break;
        case Placing_Circle:
            if (level->nturn_circles < MAX_TURN_CIRCLES) {
                level->turn_circles[level->nturn_circles++] = (Turn_Circle){
                    .position =
                        (Vector2){
                            .x = cursor_position.x + (float)CELL_SIZE / 2,
                            .y = cursor_position.y + (float)CELL_SIZE / 2,
                        },
                    .radius = tentative_radius,
                    .direction = tentative_direction,
                };
            }
            break;
        case Placing_Cannon:
            level->cannon.position = cursor_position;
            level->cannon.direction = tentative_direction;
            level->cannon_placed = true;
            break;
        case Placing_Goal:
            if (level->nturn_circles < MAX_GOALS) {
                level->goal_positions[level->ngoals++] = cursor_position;
            }
            break;
        case Placing_Wall:
            if (level->nwalls < MAX_WALLS) {
                level->wall_positions[level->nwalls++] = cursor_position;
            }
            break;
        }
    }

    if (state == Placing_Circle) {
        mouse_wheel_move = GetMouseWheelMove();
        if (mouse_wheel_move > 0) {
            tentative_radius += 8;
        } else if (mouse_wheel_move < 0) {
            tentative_radius -= 8;
        }
    }
}

void draw(void) {
    Turn_Circle tentative_circle = { 0 };
    Vector2 *v = NULL;

    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);

    draw_level(*level, true);

    switch (state) {
    case Normal:
        DrawRectangle(cursor_position.x, cursor_position.y, 5, 5, BLACK);
        break;
    case Erasing:
        DrawRectangleRec(get_eraser_rect(), PINK);
        break;
    case Placing_Circle:
        tentative_circle = (Turn_Circle){
            .direction = tentative_direction,
            (Vector2){
                .x = cursor_position.x + (float)CELL_SIZE / 2,
                .y = cursor_position.y + (float)CELL_SIZE / 2,
            },
            tentative_radius,
        };
        draw_turn_circle(tentative_circle, true);
        break;
    case Placing_Cannon:
        draw_cannon(
            (Cannon){
                .position = cursor_position,
                .direction = tentative_direction,
            },
            true);
        break;
    case Placing_Goal:
        DrawTexture(finish_line_texture, cursor_position.x, cursor_position.y,
                    WHITE);
        break;
    case Placing_Wall:
        DrawRectangle(cursor_position.x, cursor_position.y, CELL_SIZE,
                      CELL_SIZE, GRAY);
        break;
    }

    DrawTextEx(font,
               TextFormat("Level %d/%d", (level - levels.data + 1), NLEVELS),
               (Vector2){
                   .x = (float)CELL_SIZE / 2,
                   .y = (float)CELL_SIZE / 2,
               },
               Font_Size_Small, 2, BLACK);

    EndDrawing();
}

int print_level_loader(const char *filename, Levels levels) {
    Level *level;
    Turn_Circle *c;
    Vector2 *v;
    FILE *file = fopen(filename, "w");
    if (!file) {
        return 1;
    }
    fprintf(file, "#include \"level.h\"\n\n");
    fprintf(file, "static const Levels load_default_levels(void) {");
    fprintf(file, "Levels levels = {.data={\n");
    for (level = levels.data; level < levels.data + NLEVELS; ++level) {
        fprintf(file, "    {\n");

        fprintf(file, "    .turn_circles = {\n");
        for (c = level->turn_circles;
             c < level->turn_circles + level->nturn_circles; ++c) {
            fprintf(file, "        { %d, { %f, %f }, %d},\n", c->direction,
                    c->position.x, c->position.y, c->radius);
        }
        fprintf(file, "    },\n");

        fprintf(file, "    .wall_positions = {\n");
        for (v = level->wall_positions;
             v < level->wall_positions + level->nwalls; ++v) {
            fprintf(file, "{ %f, %f },\n", v->x, v->y);
        }
        fprintf(file, "    },\n");

        fprintf(file, "    .goal_positions = {\n");
        for (v = level->goal_positions;
             v < level->goal_positions + level->ngoals; ++v) {
            fprintf(file, "{ %f, %f },\n", v->x, v->y);
        }
        fprintf(file, "    },\n");

        fprintf(file, "    .cannon = (Cannon){ { %f, %f }, %d},\n",
                level->cannon.position.x, level->cannon.position.y,
                level->cannon.direction);

        fprintf(file, "    .nturn_circles = %d\n,", level->nturn_circles);
        fprintf(file, "    .nwalls = %d\n,", level->nwalls);
        fprintf(file, "    .ngoals = %d\n,", level->ngoals);
        fprintf(file, "    .cannon_placed = %d\n", level->cannon_placed);

        fprintf(file, "    },\n");
    }
    fprintf(file, "}};\n");
    fprintf(file, "return levels;\n");
    fprintf(file, "}\n");

    return 0;
}

int main(int argc, char **argv) {
    unsigned char *filedata = 0;
    char *filename;
    int levels_size = sizeof(levels_size);

    if (argc != 2) {
        fprintf(stderr, "USAGE: trajection-editor FILE\n");
        return EXIT_FAILURE;
    }

    filename = argv[1];

    if (FileExists(filename)) {
        filedata = LoadFileData(filename, &levels_size);
        levels = *(Levels *)filedata;
        UnloadFileData(filedata);
    }

    init();

    draw();
    do {
        update();
        draw();
    } while (!WindowShouldClose());

    quit();

    SaveFileData(filename, &levels, sizeof(levels));
    print_level_loader("src/default_levels.h", levels);

    return EXIT_SUCCESS;
}
