#include "barry.h"
#include "game_sprites.h"

#include <gui/gui.h>
#include <furi.h>

void barry_tick(BARRY* const barry) {
    // Do jetpack things
    barry->gravity += GRAVITY_TICK;
    barry->point.y += barry->gravity;

    // Constrain barry's height within sprite_height and 64 - sprite_height
    if(barry->point.y > (64 - BARRY_HEIGHT)) {
        barry->point.y = 64 - BARRY_HEIGHT;
        barry->gravity = 0; // stop upward momentum
    } else if(barry->point.y < 0) {
        barry->point.y = 0;
        barry->gravity = 0; // stop downward momentum
    }

    if(barry->isBoosting) {
        barry->gravity += GRAVITY_BOOST;
    }
}

void draw_barry(const BARRY* barry, Canvas* const canvas, const GameSprites* sprites) {
    canvas_draw_icon_animation(canvas, barry->point.x, barry->point.y, sprites->barry);
}