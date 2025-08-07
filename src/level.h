#ifndef LEVEL_H
#define LEVEL_H

#include "cannon.h"
#include "constants.h"
#include "turn_circle.h"
#include <raylib.h>
#include <stdbool.h>

typedef struct Level {
    Turn_Circle turn_circles[MAX_TURN_CIRCLES];
    Vector2 wall_positions[MAX_WALLS];
    Vector2 goal_positions[MAX_GOALS];
    Cannon cannon;
    int nturn_circles;
    int nwalls;
    int ngoals;
    bool cannon_placed;
} Level;

typedef struct Levels {
    Level data[NLEVELS];
} Levels;

void draw_level(Level level, bool debug);

#endif
