#ifndef DIRECTION_H
#define DIRECTION_H

typedef enum Direction { Left, Right, Up, Down } Direction;

const char *direction_to_string(Direction direction);
float direction_to_rotation(Direction direction);

#endif
