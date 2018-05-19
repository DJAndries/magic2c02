#include "render.h"
#include <stdlib.h>
#include "sprite.h"
#include "background.h"

void prerender(magic2c02_ctx* ctx) {
  ctx->register_info->sprite_overflow = 0;
  ctx->register_info->sprite_hit = 0;
  ctx->register_info->vblank = 0;
  ctx->scanline_count = 0;
}

void vblank_start(magic2c02_ctx* ctx) {
  ctx->register_info->vblank = 1;
  ctx->render(ctx->screen_output);
}

void visible(magic2c02_ctx* ctx) {
  unsigned char* output = ctx->screen_output + (ctx->scanline_count * 256 * 3);
  memset(ctx->scanline_buffer, 0, sizeof(unsigned char) * 256 * 3);
  fill_secondary_oam(ctx);

  /* Render sprites behind bg */
  render_sprite_scanline(ctx, 1);
  render_bg_line(ctx);
  render_sprite_scanline(ctx, 0);

  convert_buffer_to_rgb(ctx, output);
}

void render_scanline(magic2c02_ctx* ctx) {
  if (ctx->scanline_count == 261) {
    prerender(ctx);
  } else if (ctx->scanline_count == 241) {
    vblank_start(ctx);
  } else if (ctx->scanline_count < 240) {
    visible(ctx);
  }
}
