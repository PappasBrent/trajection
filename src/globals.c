#include "globals.h"
#include "constants.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

Font font;
Texture finish_line_texture;
Texture spritesheet;

void init_globals(void) {
    Image finish_line_image = { 0 };
    finish_line_image =
        GenImageChecked(CELL_SIZE, CELL_SIZE, 4, 4, WHITE, BLACK);
    finish_line_texture = LoadTextureFromImage(finish_line_image);
    UnloadImage(finish_line_image);
    font = LoadFontEx("./resources/game-font.ttf", Font_Size_Large, 0, 0);
    /* SetTextureFilter(font.texture, TEXTURE_FILTER_TRILINEAR); */
    spritesheet = LoadTexture("./resources/spritesheet.png");
}

void unload_globals(void) {
    UnloadFont(font);
    UnloadTexture(finish_line_texture);
    UnloadTexture(spritesheet);
}
