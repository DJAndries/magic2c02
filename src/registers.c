#include "registers.h"
#include "vram.h"
#include <string.h>

void ppuctrl_write(magic2c02_ctx* ctx, magic2c02_register_info* reginfo) {
  unsigned char ppuctrl = *ctx->cpu_ma(ctx->cpu_ctx, 0x2000);

  reginfo->base_nametable_addr = 0x2000;
  switch (ppuctrl & 0x03) {
    case 1:
      reginfo->base_nametable_addr = 0x2400;
      break;
    case 2:
      reginfo->base_nametable_addr = 0x2800;
      break;
    case 3:
      reginfo->base_nametable_addr = 0x2C00;
      break;
  }

  reginfo->vram_increment = (ppuctrl & 0x04) == 0x04 ? 32 : 1;
  reginfo->sprite_pattern_table_addr = (ppuctrl & 0x08) == 0x08 ? 0x1000 : 0x00;
  reginfo->background_pattern_table_addr = (ppuctrl & 0x10) == 0x10 ? 0x1000 : 0x00;
  reginfo->sprite_size = (ppuctrl & 0x20) == 0x20 ? 16 : 8;
  reginfo->master_slave_select = (ppuctrl & 0x40) == 0x40 ? 1 : 0;
  reginfo->nmi_enabled = (ppuctrl & 0x80) == 0x80 ? 1 : 0;
}

void ppustatus_read(magic2c02_ctx* ctx, magic2c02_register_info* reginfo) {
  *ctx->cpu_ma(ctx->cpu_ctx, 0x2002) = *ctx->cpu_ma(ctx->cpu_ctx, 0x2002) & 0x7F;
  reginfo->scroll_is_y_input = 0;
  reginfo->ppuaddr_is_low_input = 0;
  reginfo->v_blank = 0;
}

void oamaddr_write(magic2c02_ctx* ctx) {
  *ctx->cpu_ma(ctx->cpu_ctx, 0x2004) = ctx->oam[*ctx->cpu_ma(ctx->cpu_ctx, 0x2003)];
}

void oamdata_access(magic2c02_ctx* ctx) {
  ctx->oam[*ctx->cpu_ma(ctx->cpu_ctx, 0x2003)] = *ctx->cpu_ma(ctx->cpu_ctx, 0x2004);
}

void scroll_write(magic2c02_ctx* ctx, magic2c02_register_info* reginfo) {
  if (reginfo->scroll_is_y_input == 1) {
    reginfo->scroll_y = *ctx->cpu_ma(ctx->cpu_ctx, 0x2005);
  } else {
    reginfo->scroll_x = *ctx->cpu_ma(ctx->cpu_ctx, 0x2005);
  }
  reginfo->scroll_is_y_input = reginfo->scroll_is_y_input ? 0 : 1;
}

void ppuaddr_write(magic2c02_ctx* ctx, magic2c02_register_info* reginfo) {
  if (reginfo->ppuaddr_is_low_input == 1) {
    reginfo->ppuaddr = (reginfo->ppuaddr & 0xFF00)
      | *ctx->cpu_ma(ctx->cpu_ctx, 0x2006);
    /* Allow ppudata read */
    *ctx->cpu_ma(ctx->cpu_ctx, 0x2007) = *ma(ctx, reginfo->ppuaddr);
  } else {
    reginfo->ppuaddr = (reginfo->ppuaddr & 0x00FF)
      | (*ctx->cpu_ma(ctx->cpu_ctx, 0x2006) << 8);
  }
  reginfo->ppuaddr_is_low_input = reginfo->ppuaddr_is_low_input ? 0 : 1;
}

void ppudata_access(magic2c02_ctx* ctx, magic2c02_register_info* reginfo) {
  *ma(ctx, reginfo->ppuaddr) = *ctx->cpu_ma(ctx->cpu_ctx, 0x2007);
  reginfo->ppuaddr = reginfo->ppuaddr + reginfo->vram_increment;
}

void oamdma_write(magic2c02_ctx* ctx, magic2c02_register_info* reginfo) {
  unsigned char oam_addr = *ctx->cpu_ma(ctx->cpu_ctx, 0x2003);
  unsigned char page_high = *ma(ctx, 0x4014);
  unsigned char* page_start = ma(ctx, page_high << 8);
  memcpy(ctx->oam + oam_addr, page_start, 256 - oam_addr);
}

void update_last_write_byte(magic2c02_ctx* ctx, magic2c02_register_info* reginfo,
unsigned char address) {
  reginfo->last_write_byte = *ctx->cpu_ma(ctx->cpu_ctx, address);
}

void update_ppustatus(magic2c02_ctx* ctx, magic2c02_register_info* reginfo) {
  unsigned char value = reginfo->v_blank << 7 | reginfo->sprite_hit << 6 |
    reginfo->sprite_overflow << 5 | (reginfo->last_write_byte & 0x1F);
  *ctx->cpu_ma(ctx->cpu_ctx, 0x2002) = value;
}

void process_registers(magic2c02_ctx* ctx, unsigned short last_memory_access_addr) {
  magic2c02_register_info* reginfo = ctx->register_info;

  /* update last write byte if writable register */
  if (last_memory_access_addr != 0x2001 && last_memory_access_addr != 0x2002) {
    update_last_write_byte(ctx, reginfo, last_memory_access_addr);
  }

  switch (last_memory_access_addr) {
    case 0x2000:
      ppuctrl_write(ctx, reginfo);
      break;
    case 0x2001:
      /* Not implemented */
      break;
    case 0x2002:
      /* Clear vertical blank flag, scroll and ppuaddr latches */
      ppustatus_read(ctx, reginfo);
      break;
    case 0x2003:
      /* Allow OAM read */
      oamaddr_write(ctx);
      break;
    case 0x2004:
      /* Allow OAM write */
      oamdata_access(ctx);
      break;
    case 0x2005:
      /* Allow scroll write */
      scroll_write(ctx, reginfo);
      break;
    case 0x2006:
      ppuaddr_write(ctx, reginfo);
      break;
    case 0x2007:
      ppudata_access(ctx, reginfo);
      break;
    case 0x4014:
      oamdma_write(ctx, reginfo);
      break;
  }

  update_ppustatus(ctx, reginfo);
}
