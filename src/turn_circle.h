#ifndef TURN_CIRCLE_H
#define TURN_CIRCLE_H

#include "direction.h"
#include "raylib.h"

typedef struct Turn_Circle {
    Direction direction;
    Vector2 position;
    int radius;
} Turn_Circle;

void draw_turn_circle(Turn_Circle tc, bool tentative);

#endif
