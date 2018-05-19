#include "background.h"
#include "vram.h"
#include "pattern_palette.h"

unsigned char attribute_table_palette_index(magic2c02_ctx* ctx,
unsigned short x, unsigned short y) {
  unsigned char attribute_byte;
  /* Calculate base attribute table address */
  unsigned short addr = 0x23C0 + (y >= 240 ? 0x800 : 0x00) + (x >= 256 ? 0x400 : 0x00);
  /* Calculate y offset in address
    (divide y value by 16 pixels (attribute cell size), then multiply by 8 cells (row size)) */
  addr = addr + ((y >= 240 ? y - 240 : y) / 32 * 8);
  /* Calculate x offset in address */
  addr = addr + ((x >= 256 ? x - 256 : x) / 32);

  attribute_byte = *ma(ctx, addr);
  if ((y % 32) >= 16) {
    attribute_byte = attribute_byte >> 4;
  }
  if ((x % 32) >= 16) {
    attribute_byte = attribute_byte >> 2;
  }
  return attribute_byte & 0x03;
}

unsigned char nametable_tile_number(magic2c02_ctx* ctx,
unsigned short x, unsigned short y) {
  /* Calculate base nametable address */
  unsigned short addr = 0x2000 + (y >= 240 ? 0x800 : 0x00) + (x >= 256 ? 0x400 : 0x00);
  /* Calculate y offset in address
     (divide y value by 8 pixels (tile size), then multiply by 32 tiles (row size)) */
  addr = addr + ((y >= 240 ? y - 240 : y) / 8 * 32);
  /* Calculate x offset in address */
  addr = addr + ((x >= 256 ? x - 256 : x) / 8);
  return *ma(ctx, addr);
}

void render_bg_line(magic2c02_ctx* ctx) {
  /* TODO: check for sprite color in buffer before rendering default bg color */
  unsigned short tile_bank = ctx->register_info->background_pattern_table_addr;
  unsigned short i, y, x;
  unsigned char tile_pixel_count, palette_index, tile_number, tile_y_offset, tile_x_offset;
  unsigned char* scanline_buffer = ctx->scanline_buffer;

  y = ctx->scanline_count + ctx->register_info->scroll_y;
  x = ctx->register_info->scroll_x;
  tile_y_offset = y % 8;

  for (i = 0; i < 256; i = i + tile_pixel_count) {
    tile_number = nametable_tile_number(ctx, x, y);
    palette_index = attribute_table_palette_index(ctx, x, y);
    if (i == 0) {
      /* Potential partial tile render, start of screen */
      tile_x_offset = ctx->register_info->scroll_x % 8;
      tile_pixel_count = 8 - tile_x_offset;
    } else {
      /* Potential partial tile render, end of screen */
      tile_x_offset = 0;
      tile_pixel_count = ((i + 8) > 256) ? (256 - i) : 8;
    }

    render_pattern_line(ctx, 0, tile_number, tile_bank, palette_index, tile_x_offset,
      tile_y_offset, tile_pixel_count, &scanline_buffer);

    x += tile_pixel_count;
  }
}
