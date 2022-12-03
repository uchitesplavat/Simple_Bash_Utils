#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

struct Flags {
    int fcmp, fcompare, ff, notEmpty, e, i, v, c, l, n, h, s, f, o;
};
typedef struct Flags Flags;

void flagRead(char *flagString, Flags *flag);
void distribute(char *argv, Flags *flag, regex_t *regex, int *countOfMatch, int eMark);
void writeFromFile(int matchMark, int eMark, char *argv, Flags flag, char *bufferString, int numIndex);
void lowCaseConvert(char *argv);
void oResultWrite(char *bufferString, char *argv, int eMark, regex_t *regex, Flags flag, int numIndex);
void flagInitial(Flags *flag);
void cResultWrite(char *argv, int eMark, Flags flag, int *countOfMatch);
void parser(char **argv, int argc, int *mas, int *eMas, int *fMas, int *pIndex, Flags *flag, int *eMark);
void writeFromFileCL(Flags flag, char *argv, int eMark, int *countOfMatch);
void flagFReader(char **argv, int *fMas, int *mas, Flags *flag, regex_t *regex, int *countOfMatch, int eMark);
void flagEReader(char **argv, int *eMas, Flags flag, regex_t *regex);
void notFlagReader(char **argv, int pIndex, regex_t *regex);
void notFFlagWriter(char **argv, Flags flag, int *mas, regex_t *regex, int *countOfMatch, int eMark);
void flagEOWriter(char **argv, int *eMas, int *mas, Flags flag, regex_t *regex, int eMark);
void compEO(char **argv, char *eBufferString, Flags flag, int *eMas, int eIndex, regex_t *regex);

#endif  // SRC_GREP_S21_GREP_H_
