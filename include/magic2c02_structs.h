#ifndef MAGIC2C02_STRUCTS_H
#define MAGIC2C02_STRUCTS_H

#define MAGIC2C02_INT_IRQ 2
#define MAGIC2C02_INT_NMI 3

typedef struct {
  unsigned short base_nametable_addr;
  unsigned char vram_increment;
  unsigned short sprite_pattern_table_addr;
  unsigned short background_pattern_table_addr;
  unsigned char sprite_size;
  unsigned char master_slave_select;
  unsigned char nmi_enabled;

  unsigned char scroll_x;
  unsigned char scroll_y;
  unsigned char scroll_is_y_input;

  unsigned short ppuaddr;
  unsigned char ppuaddr_is_low_input;

  unsigned char sprite_hit;
  unsigned char sprite_overflow;
  unsigned char v_blank;
  unsigned char last_write_byte;
} magic2c02_register_info;

typedef struct {
  unsigned char* vm;

  unsigned char* oam;
  unsigned char* secondary_oam;
  unsigned char sec_oam_count;

  unsigned short scanline_count;

  void* cpu_ctx;
  void (*cpu_interrupt)(void*, char);
  unsigned char* (*cpu_ma)(void*, unsigned short);

  magic2c02_register_info* register_info;

  unsigned char* screen_output;
  unsigned char* scanline_buffer;
  void (*render)(unsigned char*);
} magic2c02_ctx;

#endif
