#include "render.h"
#include <string.h>
#include "sprite.h"
#include "background.h"
#include "pattern_palette.h"

void prerender(magic2c02_ctx* ctx) {
  ctx->register_info->sprite_overflow = 0;
  ctx->register_info->sprite_hit = 0;
  ctx->register_info->v_blank = 0;
  ctx->interrupt_sent = 0;
}

void vblank(magic2c02_ctx* ctx) {
  if (ctx->scanline_count == 241) {
    ctx->register_info->v_blank = 1;
    ctx->render(ctx->cpu_ctx, ctx->screen_output);
  }
  if (ctx->register_info->nmi_enabled && !ctx->interrupt_sent) {
    ctx->cpu_interrupt(ctx->cpu_ctx);
    ctx->interrupt_sent = 1;
  }
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
  } else if (ctx->scanline_count > 240 && ctx->scanline_count <= 260) {
    vblank(ctx);
  } else if (ctx->scanline_count < 240) {
    visible(ctx);
  }
  ctx->scanline_count = ctx->scanline_count == 261 ? 0 : ctx->scanline_count + 1;
}
