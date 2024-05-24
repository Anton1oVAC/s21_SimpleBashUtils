CC=gcc
FLAGS=-Wall -Werror -Wextra -std=c11
MAKE = make

s21_grep: s21_grep.c
	$(CC) $(FLAGS) -o ./s21_grep s21_grep.c

clean: s21_grep.c
	rm -rf  ./s21_grep

rebuild: 
	$(MAKE) clean
