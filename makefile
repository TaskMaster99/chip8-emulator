
OUT = out/main.o out/architecture.o out/instruction_config.o  out/instruction.o out/graphic.o
SRC = main.c src/architecture.c src/instruction_config.c src/instruction.c src/graphic.c
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
