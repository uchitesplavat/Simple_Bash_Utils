#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

struct Flags {
    int notEmpty, b, e, n, s, t, v, ee, tt;
    unsigned char prev;
};
typedef struct Flags Flags;

void flagRead(char *flagString, Flags *flag);
void flagInitial(Flags *flag);
void distribute(char *argv, Flags flag);
void writeFromFile(FILE *file);
void writeFromFileWithFlags(FILE *file, int *numIndex, Flags flag, int *countOfEmptyes);
void writeFromFileWithV(Flags *flag, unsigned char ch, int *numIndex);

#endif  // SRC_CAT_S21_CAT_H_
