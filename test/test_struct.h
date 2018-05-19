#ifndef TEST_STRUCT_H
#define TEST_STRUCT_H

#include "SDL2/SDL.h"
#include "magic2c02.h"

typedef struct {
  unsigned char* cpu_memory;
  SDL_Renderer* renderer;
  SDL_Texture* buffer;
  magic2c02_ctx* ctx;
  unsigned char scroll;
  unsigned char sprite_0_position;
} test_ctx;

#endif
