#include "turn_circle.h"
#include "constants.h"
#include "globals.h"
#include "raylib.h"

void draw_turn_circle(Turn_Circle tc, bool tentative) {
    Color color = tentative ? GRAY :
                              (Color){ .r = 234, .g = 91, .b = 111, .a = 255 };
    const float arrow_width = CELL_SIZE;
    const float arrow_height = CELL_SIZE;
    float x = tc.position.x - arrow_width / 2;
    int y = tc.position.y - arrow_height / 2;
    Vector2 arrow_position = { 0 };
    float min_x = tc.position.x - tc.radius - arrow_width;
    float max_x = tc.position.x + tc.radius;
    float min_y = tc.position.y - tc.radius - arrow_height;
    float max_y = tc.position.y + tc.radius;
    int time = GetTime() * 100;
    float offset = 0;
    Rectangle arrow_source_rect = { 0 };
    switch (tc.direction) {
    case Left:
        offset = 1 - (float)(time % 100) / 100;
        x = min_x + (max_x - min_x) * offset;
        arrow_source_rect.x = CELL_SIZE;
        arrow_source_rect.y = 0;
        arrow_source_rect.width = -CELL_SIZE;
        arrow_source_rect.height = CELL_SIZE;
        break;
    case Right:
        offset = (float)(time % 100) / 100;
        x = min_x + (max_x - min_x) * offset;
        arrow_source_rect.width = CELL_SIZE;
        arrow_source_rect.x = CELL_SIZE;
        arrow_source_rect.y = 0;
        arrow_source_rect.width = CELL_SIZE;
        arrow_source_rect.height = CELL_SIZE;
        break;
    case Up:
        offset = 1 - (float)(time % 100) / 100;
        y = min_y + (max_y - min_y) * offset;
        arrow_source_rect.x = 0;
        arrow_source_rect.y = CELL_SIZE;
        arrow_source_rect.width = CELL_SIZE;
        arrow_source_rect.height = -CELL_SIZE;
        break;
    case Down:
        offset = (float)(time % 100) / 100;
        y = min_y + (max_y - min_y) * offset;
        arrow_source_rect.x = 0;
        arrow_source_rect.y = CELL_SIZE;
        arrow_source_rect.width = CELL_SIZE;
        arrow_source_rect.height = CELL_SIZE;
        break;
    }
    arrow_position.x = x;
    arrow_position.y = y;
    DrawCircle(tc.position.x, tc.position.y, tc.radius, color);
    DrawTextureRec(spritesheet, arrow_source_rect, arrow_position, WHITE);
}
