#include "sprite.h"
#include <string.h>

void fill_secondary_oam(magic2c02_ctx* ctx) {
  unsigned char i, y, sec_oam_count;
  ctx->sec_oam_count = 0;
  memset(ctx->secondary_oam, 0xFF, sizeof(unsigned char) * 8 * 4);
  for (i = 0; i < 64; i += 1) {
    y = ctx->oam[i * 4];
    if (ctx->scanline_count < (y + ctx->register_info->sprite_size) &&
      ctx->scanline_count >= y) {
      if (sec_oam_count < 8) {
        /* copy this one, it's in the scanline */
        memcpy(ctx->secondary_oam + (sec_oam_count * 4), ctx->oam + (i * 4), 4);
        sec_oam_count = sec_oam_count + 1;
      } else {
        /* TODO: set sprite overflow flag */
      }
    }
  }
}

void render_sprite_scanline(magic2c02_ctx* ctx, unsigned char is_behind_bg) {
  /* TODO: implement sprite flipping */
  unsigned char i, palette_index, behind_bg, y, x, tile_index, tile_bank;
  for (i = 0; i < ctx->sec_oam_count; i += 1) {
    behind_bg = (ctx->secondary_oam[i * 4 + 2] & 0x20) ? 1 : 0
    if (behind_bg != is_behind_bg) {
      continue;
    }
    y = ctx->secondary_oam[i * 4];
    tile_index = ctx->secondary_oam[i * 4 + 1] >> 1;
    tile_bank = (ctx->secondary_oam[i * 4 + 1] & 0x01) ? 0x1000 : 0x00;
    palette_index = ctx->secondary_oam[i * 4 + 2] & 0x03;
  }
}
