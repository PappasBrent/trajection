#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "raylib.h"
enum Constants {
    WIDTH = 800,
    HEIGHT = 640,
    CELL_SIZE = 32,
    MAX_TURN_CIRCLES = 16,
    MAX_WALLS = 48,
    MAX_GOALS = 16,
    NLEVELS = 5,
    BALL_RADIUS = 4,
    BALL_SPEED = 6,
    FONT_SPACING = 2,
};

enum Font_Size {
    Font_Size_Small = 24,
    Font_Size_Medium = 48,
    Font_Size_Large = 72,
};

static const Color BACKGROUND_COLOR =
    (Color){ .r = 119, .g = 190, .b = 240, .a = 255 };
static const Color BALL_COLOR = WHITE;

#endif
