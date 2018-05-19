#ifndef PATTERN_PALETTE_H
#define PATTERN_PALETTE_H

#include "magic2c02_structs.h"

void convert_buffer_to_rgb(magic2c02_ctx* ctx, unsigned char* output);

void render_pattern_line(magic2c02_ctx* ctx, unsigned char is_sprite, unsigned char tile_index,
  unsigned short tile_bank, unsigned char palette_index, unsigned char offset_x,
  unsigned char offset_y, unsigned char pixel_count, unsigned char** scanline_buffer);

#endif
