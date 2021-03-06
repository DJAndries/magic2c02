#ifndef MAGIC2C02_H
#define MAGIC2C02_H

#include "magic2c02_structs.h"

magic2c02_ctx* magic2c02_init(unsigned char* (*cpu_ma) (void*, unsigned short),
  void (*cpu_interrupt)(void*),
  void* cpu_ctx,
  void (*render)(void*, unsigned char*));
void magic2c02_render_scanline(magic2c02_ctx* ctx);
void magic2c02_process_registers(magic2c02_ctx* ctx, unsigned short last_memory_access_addr);
void magic2c02_free(magic2c02_ctx* ctx);

#endif
