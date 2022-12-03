CFLAGS = -Wall -Werror -Wextra #-fsanitize=address
CC = gcc
PATCHCAT  := cat/
PATCHGREP := grep/
DEL = rm $(PATCHCAT)s21_cat.o $(PATCHGREP)s21_grep.o *.o

all : s21_cat s21_grep

s21_cat : $(PATCHCAT)s21_cat.o $(PATCHCAT)s21_cat.h
	$(CC) $(CFLAGS) -c $(PATCHCAT)s21_cat.c -o $(PATCHCAT)s21_cat.o
	$(CC) $(CFLAGS) $(PATCHCAT)s21_cat.o -o $(PATCHCAT)s21_cat
	
s21_grep : $(PATCHGREP)s21_grep.o
	$(CC) $(CFLAGS) -c $(PATCHGREP)s21_grep.c -o $(PATCHGREP)s21_grep.o
	$(CC) $(CFLAGS) $(PATCHGREP)s21_grep.o -o $(PATCHGREP)s21_grep
clean :
	$(DEL)

rebuild: clean all

lint:
	cp ../materials/linters/CPPLINT.cfg ./
	python3 ../materials/linters/cpplint.py --extensions=c */*.h */*.c
	rm -f CPPLINT.cfg