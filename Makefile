2048: main.c
	gcc -Wall -Wextra -Werror main.c -o 2048 -lraylib -lm

run: 2048
	./2048
