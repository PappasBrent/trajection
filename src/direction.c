#include "direction.h"

const char *direction_to_string(Direction direction) {
    // clang-format off
    switch (direction) {
    case Left:      return "<";
    case Right:     return ">";
    case Up:        return "^";
    case Down:      return "v";
    }
    // clang-format on
}

float direction_to_rotation(Direction direction) {
    switch (direction) {
    case Left:
        return 180;
    case Right:
        return 0;
    case Up:
        return 270;
    case Down:
        return 90;
    }
}
