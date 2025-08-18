
OUT = main.o

test : ${OUT}
	gcc -o test ${OUT}

%.o : %.c
	gcc -std=c99 -Wall -Wextra -pedantic -ggdb -o $@ -O1 -c $<


run:
	./test

clean:
	rm -f ${OUT}
	rm -f test
