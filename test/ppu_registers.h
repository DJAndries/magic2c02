#ifndef PPU_REGISTERS_H
#define PPU_REGISTERS_H

#include "magic2c02.h"
#include "test_struct.h"

void init_ppu_registers(test_ctx* tctx);
void adjust_scroll(test_ctx* tctx);

#endif
