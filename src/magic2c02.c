#include "magic2c02.h"
#include <stdlib.h>
#include <string.h>

#include "render.h"
#include "registers.h"

__attribute__((visibility("default"))) \
magic2c02_ctx* magic2c02_init(unsigned char* (*cpu_ma)(void*, unsigned short),
void (*cpu_interrupt)(void*), void* cpu_ctx, void (*render)(void*, unsigned char*)) {
  magic2c02_ctx* ctx = (magic2c02_ctx*)malloc(sizeof(magic2c02_ctx));
  if (ctx == 0) {
    return 0;
  }

  memset(ctx, 0, sizeof(magic2c02_ctx));

  ctx->vm = (unsigned char*)malloc(sizeof(unsigned char) * 16384);
  ctx->oam = (unsigned char*)malloc(sizeof(unsigned char) * 256);
  ctx->secondary_oam = (unsigned char*)malloc(sizeof(unsigned char) * 8);
  ctx->screen_output = (unsigned char*)malloc(sizeof(unsigned char) * 256 * 240 * 3);
  ctx->scanline_buffer = (unsigned char*)malloc(sizeof(unsigned char) * 256 * 3);
  ctx->register_info = (magic2c02_register_info*)malloc(sizeof(magic2c02_register_info));

  memset(ctx->vm, 0, sizeof(unsigned char) * 16384);
  memset(ctx->oam, 0, sizeof(unsigned char) * 256);
  memset(ctx->screen_output, 0, sizeof(unsigned char) * 256 * 240 * 3);
  memset(ctx->scanline_buffer, 0, sizeof(unsigned char) * 256 * 3);
  memset(ctx->register_info, 0, sizeof(magic2c02_register_info));

  ctx->cpu_ma = cpu_ma;
  ctx->cpu_interrupt = cpu_interrupt;
  ctx->cpu_ctx = cpu_ctx;
  ctx->render = render;
  /* Start at pre-render scanline */
  ctx->scanline_count = 261;
  ctx->register_info->sprite_size = 8;

  return ctx;
}

__attribute__((visibility("default"))) void magic2c02_render_scanline(magic2c02_ctx* ctx) {
  render_scanline(ctx);
}

__attribute__((visibility("default"))) void magic2c02_process_registers(magic2c02_ctx* ctx,
unsigned short last_memory_access_addr) {
  process_registers(ctx, last_memory_access_addr);
}

__attribute__((visibility("default"))) void magic2c02_free(magic2c02_ctx* ctx) {
  free(ctx->vm);
  free(ctx->oam);
  free(ctx->secondary_oam);
  free(ctx->register_info);
  free(ctx->screen_output);
  free(ctx->scanline_buffer);
  free(ctx);
}
