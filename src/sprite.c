#include "sprite.h"
#include "vram.h"
#include "pattern_palette.h"

void fill_secondary_oam(magic2c02_ctx* ctx) {
  unsigned char i, y;
  ctx->sec_oam_count = 0;
  for (i = 0; i < 64; i += 1) {
    y = ctx->oam[i * 4];

    /* If in range... */
    if (ctx->scanline_count < (y + ctx->register_info->sprite_size) &&
      ctx->scanline_count >= y) {

      /* ... and if oam not full */
      if (ctx->sec_oam_count < 8) {

        ctx->secondary_oam[ctx->sec_oam_count] = i;
        ctx->sec_oam_count++;

      } else {
        ctx->register_info->sprite_overflow = 1;
      }
    }
  }
}

void render_sprite_scanline(magic2c02_ctx* ctx, unsigned char is_behind_bg) {
  /* TODO: implement sprite flipping */
  unsigned short tile_bank;
  unsigned char* oam_sprite;
  unsigned char i, palette_index, behind_bg, y, x, tile_index, tile_y, pixel_count, sprite_indicator;
  unsigned char* scanline_buffer;
  for (i = 0; i < ctx->sec_oam_count; i += 1) {
    oam_sprite = ctx->oam + (ctx->secondary_oam[i] * 4);
    behind_bg = (*(oam_sprite + 2) & 0x20) ? 1 : 0;
    if (behind_bg != is_behind_bg) {
      continue;
    }
    y = *oam_sprite;
    tile_y = ctx->scanline_count - y;
    tile_index = *(oam_sprite + 1);
    if (tile_y >= 8) {
      tile_index++;
      tile_y -= 8;
    }
    if (ctx->register_info->sprite_size == 16) {
      tile_index = tile_index & 0xFE;
      tile_bank = (*(oam_sprite + 1) & 0x01) ? 0x1000 : 0x00;
    } else {
      tile_bank = ctx->register_info->sprite_pattern_table_addr;
    }
    palette_index = *(oam_sprite + 2) & 0x03;
    x = *(oam_sprite + 3);
    pixel_count = (x + 8) > 256 ? 256 - x : 8;
    scanline_buffer = ctx->scanline_buffer + (x * 3);
    /* Indicate that this is sprite 0 (for sprint hit detection) */
    sprite_indicator = i == 0 ? 0x03 : 0x01;

    render_pattern_line(ctx, sprite_indicator, tile_index, tile_bank, palette_index, 0,
      tile_y, pixel_count, &scanline_buffer);
  }
}
