
OUT = out/main.o out/chip8.o out/instruction_config.o  out/instruction.o 
SRC = main.c src/chip8.c src/instruction_config.c src/instruction.c
LIB = -lSDL2

test : ${OUT}
	gcc -o test ${OUT} ${LIB}

out/%.o : %.c
	mkdir -p out
	gcc -std=c99 -Wall -Wextra -pedantic -ggdb -o $@ -O1 -c $< 

out/%.o : src/%.c
	mkdir -p out
	gcc -std=c99 -Wall -Wextra -pedantic -ggdb -o $@ -O1 -c $< 

run:
	./test

clean:
	rm -f ${OUT}
	rm -f test
