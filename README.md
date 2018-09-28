# magic2c02
#### Partially working Ricoh 2C02 PPU emulation library

Kind of working, kind of broken

Not supported:
- Sprite flipping
- Nametable mirroring (except for horizontal mirroring)
- PPU master/slave select
- Color emphasis/PPUMASK register
- PAL (NTSC only)

## Building

Should build with gcc/make.
I used MinGW (32-bit) and MSYS to build this on Windows.

```
make
```

## API

### Initialization

```
magic2c02_ctx* magic2c02_init(unsigned char* (*cpu_ma) (void*, unsigned short),
  void (*cpu_interrupt)(void*),
  void* cpu_ctx,
  void (*render)(void*, unsigned char*));
```

Parameters:
- `cpu_ma`: Callback for main memory access. Parameters are the application context and memory address, respectively.
Should return pointer to the requested memory.
- `cpu_interrupt`: Callback for invoking on NMI on the CPU.
- `cpu_ctx`: Pointer to an application context. Used in `cpu_ma`, `cpu_interrupt` and `render` callbacks. Optional.
- `render`: Callback for displaying rendered image to screen. First argument is application context.
Second argument is an RGB array.

### PPU register processing

Should probably execute this before you render a scanline. Uses the NES CPU/PPU register mappings ($2000-2007 / $4014).

```
void magic2c02_process_registers(magic2c02_ctx* ctx, unsigned short last_memory_access_addr);
```

Parameters:
- `ctx`: Pointer to the context returned from initialization.
- `last_memory_access_addr`: Memory access address from last CPU instruction. 0 if no memory access in last instruction.

### Execution

```
void magic2c02_render_scanline(magic2c02_ctx* ctx);
```

Parameters:
- `ctx`: Pointer to the context returned from initialization.

### Cleanup

```
void magic2c02_free(magic2c02_ctx* ctx);
```

## Tests

### Test build/execution

"Test" program depends on SDL 2. The Makefile assumes you have an SDL build in the parent directory.

```
make test
```

Run `test` in the bin folder after building.
