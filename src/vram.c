#include "vram.h"

unsigned char* ma(magic2c02_ctx* ctx, unsigned short addr) {
  unsigned short final_addr = addr;
  if (addr >= 0x3000 && addr <= 0x3EFF) {
    final_addr -= 0x1000;
  } else if (addr >= 0x3F20 && addr <= 0x3FFF) {
    final_addr = ((addr - 0x3F20) % 0x20) + 0x3F00;
  }

  /* TODO: Support other modes of nametable mirroring */
  if ((final_addr >= 0x2800 && final_addr < (0x2800 + 960)) ||
    (final_addr >= 0x2C00 && final_addr < (0x2C00 + 960))) {
    final_addr -= 0x800;
  }

  return &ctx->vm[final_addr % 0x4000];
}
