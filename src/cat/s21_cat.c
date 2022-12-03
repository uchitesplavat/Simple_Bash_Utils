#include "s21_cat.h"

int main(int argc, char **argv) {
    Flags flag;
    flagInitial(&flag);
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            flagRead(argv[i], &flag);
            flag.notEmpty = 1;
        }
    }
    for (int i = 1; i < argc; i++) {
         if (argv[i][0] != '-') {
            for (; i < argc; i++) {
                distribute(argv[i], flag);
            }
        }
    }
return 0;
}

void flagInitial(Flags *flag) {
    flag->b = 0;
    flag->e = 0;
    flag->n = 0;
    flag->s = 0;
    flag->t = 0;
    flag->v = 0;
    flag->ee = 0;
    flag->tt = 0;
    flag->prev = '0';
}

void flagRead(char *flagString, Flags *flag) {
    int length = strlen(flagString);
    for (int i = 0; i < length; i++) {
        if (flagString[i] == 'b') {
            flag->b = 1;
        }
        if (flagString[i] == 'n') {
            flag->n = 1;
        }
        if (flagString[i] == 's') {
            flag->s = 1;
        }
        if (flagString[i] == 'e') {
            flag->e = 1;
        }
        if (flagString[i] == 't') {
            flag->t = 1;
        }
        if (flagString[i] == 'v') {
            flag->v = 1;
        }
        if (flagString[i] == 'E') {
            flag->ee = 1;
        }
        if (flagString[i] == 'T') {
            flag->tt = 1;
        }
    }
}

void distribute(char *argv, Flags flag) {
    FILE *file;
    int numIndex = 1;
    int countOfEmptyes = 0;
    if ((file = fopen(argv, "r")) != NULL) {
        if (flag.notEmpty) {
            writeFromFileWithFlags(file, &numIndex, flag, &countOfEmptyes);
            numIndex++;
        }
        if (!flag.notEmpty) {
            writeFromFile(file);
        }
    } else {
        printf("file not found\n");
    }
    fclose(file);
}

void writeFromFile(FILE *file) {
    char ch;
    while (feof(file) == 0) {
        ch = fgetc(file);
        if (ch != EOF) {
            printf("%c", ch);
        }
    }
}

void FlagS(char *ppch, char *pch, char ch, int *countOfEmptyes) {
    if (*pch =='\n' && ch == '\n') {
        *countOfEmptyes = *countOfEmptyes + 1;
    } else {
        *countOfEmptyes = 0;
    }
    *ppch = *pch;
    *pch = ch;
}

void writeFromFileWithFlags(FILE *file, int *numIndex, Flags flag, int *countOfEmptyes) {
    char ppch = '\n';
    char pch = '\n';
    char ch;
    while (feof(file) == 0) {
        ch = fgetc(file);
        FlagS(&ppch, &pch, ch, countOfEmptyes);
        if (((*countOfEmptyes == 0 && flag.b) || (flag.n && !flag.b)) && ppch == '\n') {
            if (!flag.s || (flag.s && *countOfEmptyes <= 1)) {
                printf("%6d\t", *numIndex);
                *numIndex += 1;
            }
        }
        if (flag.e || flag.v || flag.t || flag.tt || flag.ee) {
            if (!flag.s || (flag.s && *countOfEmptyes < 2)) {
                writeFromFileWithV(&flag, ch, numIndex);
            }
        }
        if ((flag.n || flag.b) && !flag.t && !flag.e && !flag.v && !flag.tt && !flag.ee && !flag.s) {
            if (ch != EOF) {
                printf("%c", ch);
            }
        }
        if (*countOfEmptyes < 2 && flag.s && !flag.t && !flag.e && !flag.v && !flag.tt && !flag.ee) {
            if (ch != EOF) {
                printf("%c", ch);
            }
        }
    }
}

void writeFromFileWithV(Flags *flag, unsigned char ch, int *numIndex) {
    if (flag->e || flag->t || flag->v) {
        if (flag->prev == 138 && flag->n && !flag->b) {
            printf("%6d\t", *numIndex);
            *numIndex += 1;
            flag->prev = '0';
        }
        if ((ch > 31 && ch < 127 && ch != '\0') || ch == 10 || ch == 9) {
            if (ch == '\n' && flag->e) {
                printf("%s", "$\n");
            } else if (ch == '\t' && flag->t) {
                printf("%s", "^I");
            } else {
                printf("%c", ch);
            }
        } else {
            if (ch <= 31) {
                printf("^%c", (unsigned char)(ch+64));
            }
            if (ch >= 128 && ch < 160) {
                printf("M-^%c", (unsigned char)(ch-64));
            }
            if (ch == 127) {
                printf("^?");
            }
            if (ch == 138) {
                flag->prev = ch;
            }
        }
    } else {
        if (ch == '\0' && flag->ee) {
            printf("$");
            printf("\n");
            }
        if (ch == '\t' && flag->tt) {
            printf("%s", "^I");
        } else if (ch == '\n' && flag->ee) {
            printf("%s", "$\n");
        } else {
            printf("%c", ch);
        }
    }
}
