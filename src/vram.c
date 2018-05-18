#include "vram.h"

unsigned char* ma(magic2c02_ctx* ctx, unsigned short addr) {
  unsigned short final_addr = addr;
  if (addr >= 0x3000 && addr <= 0x3EFF) {
    /* mirror of nametables */
    final_addr = addr - 0x1000;
  } else if (addr >= 0x3F20 && addr <= 0x3FFF) {
    final_addr = ((addr - 0x3F20) % 0x20) + 0x3F00;
  }
  return &ctx->vm[final_addr];
}
