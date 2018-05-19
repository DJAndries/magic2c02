sdl_lib_dir = ../sdl

all: lib obj bin lib/libmagic2c02.so

test: bin/test

bin/test: bin/libmagic2c02.so obj/main.o obj/ppu_registers.o
	gcc -lm -o bin/test obj/ppu_registers.o obj/main.o -L./lib -L$(sdl_lib_dir)/lib \
		-lmingw32 -lSDL2main -lSDL2 -l:libmagic2c02.so

bin/libmagic2c02.so: lib/libmagic2c02.so
	cp lib/libmagic2c02.so bin/libmagic2c02.so

obj/%.o: test/%.c
	gcc -Wall -O -c $< -Iinclude -I$(sdl_lib_dir)/include -o $@

obj/%.o: src/%.c
	gcc -Wall -O -c $< -Iinclude -o $@

lib/libmagic2c02.so: obj/magic2c02.o obj/registers.o obj/vram.o obj/background.o \
	obj/render.o obj/sprite.o obj/pattern_palette.o
	gcc -fvisibility=hidden -shared -o ./lib/libmagic2c02.so obj/magic2c02.o \
		obj/registers.o obj/vram.o obj/background.o \
		obj/render.o obj/sprite.o obj/pattern_palette.o

lib:
	mkdir lib

obj:
	mkdir obj

bin:
	mkdir bin

clean:
	rm -f obj/* lib/* bin/*
