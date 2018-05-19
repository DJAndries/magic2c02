#include "pattern_palette.h"
#include "vram.h"
#include "colors.h"

#define UNI_BG_COLOR_ADDR 0x3F00

const unsigned short bg_palette_bases[4] = {
  0x3F01, 0x3F05, 0x3F09, 0x3F0D
};

const unsigned short sprite_palette_bases[4] = {
  0x3F11, 0x3F15, 0x3F19, 0x3F1D
};

void detect_sprite_hit(magic2c02_ctx* ctx, unsigned char* scanline_pixel,
unsigned char is_sprite, unsigned char color_index) {
  /* If not rendering bg, and existing pixel is from sprite 0.
     If not rendering sprite 0 and existing pixel is from bg. */
  if ((!is_sprite && *scanline_pixel == 0x03) || (is_sprite == 0x03 && *scanline_pixel == 0x00)) {
    /* if existing and new pixels are non-zero */
    if (*(scanline_pixel + 2) != 0x00 && color_index != 0x00) {
      ctx->register_info->sprite_hit = 1;
    }
  }
}

void render_pattern_line(magic2c02_ctx* ctx, unsigned char is_sprite, unsigned char tile_index,
unsigned short tile_bank, unsigned char palette_index, unsigned char offset_x,
unsigned char offset_y, unsigned char pixel_count, unsigned char** scanline_buffer) {

  unsigned char i = 0;
  unsigned char color_index;
  char first_bit_shift, second_bit_shift;

  unsigned char* first_plane = ma(ctx, (tile_bank | (tile_index * 0x10)) + offset_y);
  unsigned char* second_plane = first_plane + 0x08;

  for (i = offset_x; i < (offset_x + pixel_count); i += 1) {

    first_bit_shift = pixel_count - offset_x - 1;
    second_bit_shift = pixel_count - offset_x - 2;

    color_index = ((*first_plane >> first_bit_shift) & 0x01) |
      ((*second_plane >> second_bit_shift) & 0x02);

    /* Render bg if no existing pixel, or render sprite if non-zero pixel */
    if ((is_sprite == 0 && *(*scanline_buffer + 2) != 0x00)
      || (is_sprite && color_index != 0)) {
      detect_sprite_hit(ctx, *scanline_buffer, is_sprite, color_index);

      **scanline_buffer = is_sprite;
      *(*scanline_buffer + 1) = palette_index;
      *(*scanline_buffer + 2) = color_index;
    }

    *scanline_buffer = *scanline_buffer + 3;
  }

}

void convert_buffer_to_rgb(magic2c02_ctx* ctx, unsigned char* output) {
  int i;
  unsigned char* buffer = ctx->scanline_buffer;
  unsigned char* color_rgb;
  unsigned char is_sprite, palette_index, color_index;
  unsigned short palette_base, color_address;
  for (i = 0; i < 256; i += 1) {
    is_sprite = *buffer;
    palette_index = *(buffer + 1);
    color_index = *(buffer + 2);

    palette_base = is_sprite ? sprite_palette_bases[palette_index] :
      bg_palette_bases[palette_index];

    if (is_sprite == 0 && color_index == 0) {
      color_address = UNI_BG_COLOR_ADDR;
    } else {
      color_address = palette_base + (color_index - 1);
    }

    color_rgb = colors[*ma(ctx, color_address) % 0x40];

    *output = color_rgb[0];
    *(output + 1) = color_rgb[1];
    *(output + 2) = color_rgb[2];

    buffer = buffer + 3;
    output = output + 3;
  }
}
