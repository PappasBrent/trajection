#ifndef CANNON_H
#define CANNON_H

#include "direction.h"
#include "raylib.h"

typedef struct Cannon {
    Vector2 position;
    Direction direction;
} Cannon;

void draw_cannon(Cannon cannon, bool debug);
Vector2 get_cannon_fire_position(Cannon cannon);

#endif
