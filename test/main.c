#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL.h"

#include "magic2c02.h"
#include "test_struct.h"
#include "ppu_registers.h"

void render(void* tctx, unsigned char* screen_output) {
  int pitch = 256 * 3;
  unsigned char* pixels;
  test_ctx* ttctx = (test_ctx*)tctx;
  SDL_LockTexture(ttctx->buffer, NULL, (void**)&pixels, &pitch);
  memcpy(pixels, screen_output, 256 * 240 * 3);
  SDL_UnlockTexture(ttctx->buffer);

  SDL_RenderClear(ttctx->renderer);
  SDL_RenderCopy(ttctx->renderer, ttctx->buffer, NULL, NULL);
  SDL_RenderPresent(ttctx->renderer);
  SDL_Delay(200);
}

void cpu_interrupt(void* tctx) {
  ((test_ctx*)tctx)->scroll += 1;
  ((test_ctx*)tctx)->sprite_0_position += 1;
  adjust_scroll((test_ctx*)tctx);
}

unsigned char* cpu_memory_access(void* tctx, unsigned short addr) {
  return &((test_ctx*)tctx)->cpu_memory[addr];
}

int main(int argc, char** argv) {
  unsigned int total_scanline_count;
  SDL_Window* win;
  test_ctx* tctx = (test_ctx*)malloc(sizeof(test_ctx));
  SDL_Event e;
  tctx->ctx = magic2c02_init(cpu_memory_access, cpu_interrupt, tctx, render);
  tctx->scroll = 2;

  tctx->cpu_memory = (unsigned char*)malloc(sizeof(unsigned char) * 0x10000);
  memset(tctx->cpu_memory, 0, 0x10000);

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL init failed: %s\n", SDL_GetError());
    return 1;
  }

  win = SDL_CreateWindow("magic2c02", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    256, 240, 0);
  tctx->renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED
    | SDL_RENDERER_TARGETTEXTURE);

  tctx->buffer = SDL_CreateTexture(tctx->renderer, SDL_PIXELFORMAT_RGB24,
    SDL_TEXTUREACCESS_STREAMING, 256, 240);

  magic2c02_process_registers((void*)tctx->ctx, 0);
  init_ppu_registers(tctx);

  for(total_scanline_count = 0; total_scanline_count < 1000000; total_scanline_count++) {
    magic2c02_render_scanline(tctx->ctx);
    SDL_PollEvent( &e );
  }

  SDL_DestroyTexture(tctx->buffer);
  SDL_DestroyRenderer(tctx->renderer);
  SDL_DestroyWindow(win);

  magic2c02_free(tctx->ctx);
  free(tctx);
  return 0;
}
