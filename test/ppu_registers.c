#include "ppu_registers.h"

void write_register(test_ctx* tctx, unsigned short addr, unsigned char value) {
  tctx->cpu_memory[addr] = value;
  magic2c02_process_registers(tctx->ctx, addr);
}

void write_ppu_byte(test_ctx* tctx, unsigned short addr, unsigned char value) {
  write_register(tctx, 0x2006, (addr >> 8) & 0xFF);
  write_register(tctx, 0x2006, addr & 0xFF);
  write_register(tctx, 0x2007, value);
}

void write_oam_byte(test_ctx* tctx, unsigned char addr, unsigned char value) {
  write_register(tctx, 0x2003, addr);
  write_register(tctx, 0x2004, value);
}

void init_ppu_registers(test_ctx* tctx) {
  /* Turn on NMI */
  write_register(tctx, 0x2000, 0x80);

  /* Load universal background color */
  write_ppu_byte(tctx, 0x3F00, 0x06);

  /* Load tile */
  write_ppu_byte(tctx, 0x0000, 0x41);
  write_ppu_byte(tctx, 0x0001, 0x41);
  write_ppu_byte(tctx, 0x0002, 0x41);
  write_ppu_byte(tctx, 0x0003, 0xFF);
  write_ppu_byte(tctx, 0x0004, 0xFF);
  write_ppu_byte(tctx, 0x0005, 0x41);
  write_ppu_byte(tctx, 0x0006, 0x41);
  write_ppu_byte(tctx, 0x0007, 0x41);
  write_ppu_byte(tctx, 0x0008, 0x41);
  write_ppu_byte(tctx, 0x0009, 0x41);
  write_ppu_byte(tctx, 0x000A, 0x41);
  write_ppu_byte(tctx, 0x000B, 0x41);
  write_ppu_byte(tctx, 0x000C, 0x41);
  write_ppu_byte(tctx, 0x000D, 0x41);
  write_ppu_byte(tctx, 0x000E, 0x41);
  write_ppu_byte(tctx, 0x000F, 0x41);

  /* Load sprite tile */
  write_ppu_byte(tctx, 0x0010, 0xFF);
  write_ppu_byte(tctx, 0x0011, 0xFF);
  write_ppu_byte(tctx, 0x0012, 0xFF);
  write_ppu_byte(tctx, 0x0013, 0xFF);
  write_ppu_byte(tctx, 0x0014, 0xFF);
  write_ppu_byte(tctx, 0x0015, 0xFF);
  write_ppu_byte(tctx, 0x0016, 0xFF);
  write_ppu_byte(tctx, 0x0017, 0xFF);
  write_ppu_byte(tctx, 0x0018, 0x41);
  write_ppu_byte(tctx, 0x0019, 0x41);
  write_ppu_byte(tctx, 0x001A, 0x41);
  write_ppu_byte(tctx, 0x001B, 0xFF);
  write_ppu_byte(tctx, 0x001C, 0xFF);
  write_ppu_byte(tctx, 0x001D, 0x41);
  write_ppu_byte(tctx, 0x001E, 0x41);
  write_ppu_byte(tctx, 0x001F, 0x41);

  /* Load simple palette */
  write_ppu_byte(tctx, 0x3F01, 0x2A);
  write_ppu_byte(tctx, 0x3F02, 0x2A);
  write_ppu_byte(tctx, 0x3F03, 0x2A);

  write_ppu_byte(tctx, 0x3F11, 0x1c);
  write_ppu_byte(tctx, 0x3F12, 0x18);
  write_ppu_byte(tctx, 0x3F13, 0x15);

  /* Load sprite 0 */
  write_oam_byte(tctx, 0x00, 30);
  write_oam_byte(tctx, 0x01, 0x02);
  write_oam_byte(tctx, 0x02, 0x00);
  write_oam_byte(tctx, 0x03, 70);
}

void adjust_scroll(test_ctx* tctx) {
  write_register(tctx, 0x2005, tctx->scroll);
  write_register(tctx, 0x2005, tctx->scroll);

  write_oam_byte(tctx, 0x00, tctx->sprite_0_position);
}
