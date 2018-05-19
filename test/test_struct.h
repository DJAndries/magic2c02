#ifndef TEST_STRUCT_H
#define TEST_STRUCT_H

#include "SDL2/SDL.h"

typedef struct {
  unsigned char* cpu_memory;
  SDL_Renderer* renderer;
  SDL_Texture* buffer;
} test_ctx;

#endif
