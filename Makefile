all: lib obj bin lib/libmagic2c02.so bin/test

bin/test: bin/magic2c02.so obj/main.o
	gcc -lm -o bin/test obj/main.o -L./lib -l:libmagic2c02.so

bin/libmagic2c02.so: lib/libmagic2c02.so
	cp lib/libmagic2c02.so bin/libmagic2c02.so

obj/%.o: test/%.c
	gcc -Wall -O -c $< -Iinclude -o $@

obj/%.o: src/%.c
	gcc -Wall -O -c $< -Iinclude -o $@

lib/libmagic2c02.so: obj/magic2c02.o obj/registers.o obj/vram.o
	gcc -fvisibility=hidden -shared -o ./lib/libmagic2c02.so obj/registers.o obj/vram.o

lib:
	mkdir lib

obj:
	mkdir obj

bin:
	mkdir bin

clean:
	rm -f obj/* lib/* bin/*
