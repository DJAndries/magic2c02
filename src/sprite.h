#ifndef SPRITE_H
#define SPRITE_H

#include "magic2c02_structs.h"

void fill_secondary_oam(magic2c02_ctx* ctx);
void render_sprite_scanline(magic2c02_ctx* ctx, unsigned char is_behind_bg);

#endif
