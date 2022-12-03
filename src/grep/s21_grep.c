#include "s21_grep.h"

int main(int argc, char **argv) {
    if (argc > 1) {
        regex_t regex;
        Flags flag;
        int eMark = 0;
        int fMas[100];
        int pIndex = -1;
        int mas[100];
        int eMas[100];
        int countOfMatch = 0;
        int eOMark = 0;
        flagInitial(&flag);
        parser(argv, argc, mas, eMas, fMas, &pIndex, &flag, &eMark);
        if (flag.f) {
            flagFReader(argv, fMas, mas, &flag, &regex, &countOfMatch, eMark);
        } else if (flag.e && (!flag.o || (flag.o && flag.v) || (flag.o && flag.c))) {
            flagEReader(argv, eMas, flag, &regex);
        } else if (flag.e && flag.o && !flag.v && !flag.c) {
            flagEOWriter(argv, eMas, mas, flag, &regex, eMark);
            eOMark = 1;
        } else {
            eMark--;
            notFlagReader(argv, pIndex, &regex);
        }
        if (!flag.f && !eOMark) {
            notFFlagWriter(argv, flag, mas, &regex, &countOfMatch, eMark);
        }
        regfree(&regex);
    }
return 0;
}

void notFFlagWriter(char **argv, Flags flag, int *mas, regex_t *regex, int *countOfMatch, int eMark) {
    int index = 1;
    if (flag.e) {
        index = 0;
    }
    while (mas[index] != 0) {
        distribute(argv[mas[index]], &flag, regex, countOfMatch, eMark);
        if (flag.c && flag.l) {
            writeFromFileCL(flag, argv[mas[index]], eMark, countOfMatch);
        }
        if (flag.c && !flag.l) {
            cResultWrite(argv[mas[index]], eMark, flag, countOfMatch);
        }
        index++;
        *countOfMatch = 0;
    }
}

void notFlagReader(char **argv, int pIndex, regex_t *regex) {
    char buffer[4096];
    if (!strcmp(argv[pIndex], "^$")) {
        strcpy(buffer, "^\n$");
    }  else if (!strcmp(argv[pIndex], ".")) {
        strcpy(buffer, "[^\n]");
    } else {
        strcpy(buffer, argv[pIndex]);
    }
    regcomp(regex, buffer, REG_EXTENDED);
}

void flagEReader(char **argv, int *eMas, Flags flag, regex_t *regex) {
    char sumOfArgv[4096];
    sumOfArgv[0] = '\0';
    char buffer[4096];
    int i = 0;
    while (eMas[i]) {
        if (!strcmp(argv[eMas[i]], "^$")) {
            strcpy(buffer, "^\n$");
        }  else if (!strcmp(argv[eMas[i]], ".")) {
            strcpy(buffer, "[^\n]");
        } else {
            strcpy(buffer, argv[eMas[i]]);
        }
        strcat(sumOfArgv, buffer);
        strcat(sumOfArgv, "|");
        i++;
    }
    int sumLength = strlen(sumOfArgv);
    sumOfArgv[sumLength-1] = '\0';
    if (flag.i) {
        lowCaseConvert(sumOfArgv);
    }
    regcomp(regex, sumOfArgv, REG_EXTENDED);
}

void flagEOWriter(char **argv, int *eMas, int *mas, Flags flag, regex_t *regex, int eMark) {
    FILE *file;
    char bufferString[4096];
    bufferString[0] = '\0';
    char bufferStringTemp[4096];
    bufferStringTemp[0] = '\0';
    char eBufferString[4096];
    eBufferString[0] = '\0';
    int index = 0;
    int eIndex = 0;
    regmatch_t pm;
    size_t offset = 0;
    int mark;
    int numIndex;
    long long unsigned int i;
    long long unsigned int length;
    int countOfMatch = 0;
    int compMark = 0;
    int secondMatch;
    int endMark = 0;
    while (mas[index]) {
        numIndex = 0;
        if ((file = fopen(argv[mas[index]], "r")) != NULL) {
            while (!feof(file)) {
                fgets(bufferString, sizeof(bufferString), file);
                length = strlen(bufferString);
                if (bufferString[length-1] == '\n' && length > 1) {
                    bufferString[length-1] = '\0';
                }
                strcpy(bufferStringTemp, bufferString);
                if (flag.i) {
                    lowCaseConvert(bufferStringTemp);
                }
                numIndex++;
                mark = 0;
                while (eMas[eIndex]) {
                    compEO(argv, eBufferString, flag, eMas, eIndex, regex);
                    while (!regexec(regex, bufferStringTemp + offset, 1, &pm, 0)) {
                        countOfMatch++;
                        if (!flag.c) {
                            if (!mark) {
                                if (eMark >= 2 && !flag.l && !flag.h) {
                                    printf("%s:", argv[mas[index]]);
                                }
                                if (flag.n) {
                                    printf("%d:", numIndex);
                                }
                                mark = 1;
                            }
                            for (i = pm.rm_so+offset; i < pm.rm_eo+offset; i++) {
                                printf("%c", bufferString[i]);
                            }
                            if (strcmp(bufferString, "\n")) {
                                printf("\n");
                            }
                            compMark = eIndex+1;
                            while (eMas[compMark]) {
                                regex_t regex1;
                                regmatch_t pm1;
                                size_t offset1 = 0;
                                int markIK;
                                long long unsigned int k;
                                long long unsigned int h;
                                compEO(argv, eBufferString, flag, eMas, compMark, &regex1);
                                secondMatch = regexec(&regex1, bufferStringTemp + offset1, 1, &pm1, 0);
                                while (!secondMatch) {
                                    countOfMatch++;
                                    for (h = pm1.rm_so+offset1; h < pm1.rm_eo+offset1; h++) {
                                        markIK = 0;
                                        for (k = pm.rm_so+offset; k < pm.rm_eo+offset; k++) {
                                            if (h == k && pm.rm_eo-pm.rm_so >= pm1.rm_eo-pm1.rm_so) {
                                                printf("%c", bufferString[h]);
                                                markIK = 1;
                                            }
                                        }
                                        if (strcmp(bufferString, "\n")
                                            && markIK && h+1 == pm1.rm_eo+offset1) {
                                            printf("\n");
                                        }
                                        offset += pm.rm_eo;
                                        if (pm.rm_eo-pm.rm_so > pm1.rm_eo-pm1.rm_so
                                            && !regexec(regex, bufferStringTemp + offset, 1, &pm, 0)
                                            && markIK) {
                                            secondMatch = 1;
                                            h = pm1.rm_eo+offset1;
                                        }
                                        offset -= pm.rm_eo;
                                    }
                                    offset1 += pm1.rm_eo;
                                    if (!secondMatch) {
                                        secondMatch = regexec(&regex1,
                                            bufferStringTemp + offset1, 1, &pm1, 0);
                                    }
                                }
                                compMark++;
                            }
                            offset += pm.rm_eo;
                        }
                    }
                    eIndex++;
                }
                eIndex = 0;
                offset = 0;
            }
        } else {
            if (!flag.s) {
                printf("file not found\n");
                endMark = 1;
            }
        }
        if (flag.c && mas[index] && !endMark) {
            cResultWrite(argv[mas[index]], eMark, flag, &countOfMatch);
        }
        index++;
        countOfMatch = 0;
    }
}

void compEO(char **argv, char *eBufferString, Flags flag, int *eMas, int eIndex, regex_t *regex) {
    strcpy(eBufferString, argv[eMas[eIndex]]);
    if (flag.i) {
        lowCaseConvert(eBufferString);
    }
    if (!strcmp(eBufferString, "^$")) {
        regcomp(regex, "^\n$", REG_EXTENDED);
    } else if (!strcmp(eBufferString, ".")) {
        regcomp(regex, "[^\n]", REG_EXTENDED);
    } else {
        regcomp(regex, eBufferString, REG_EXTENDED);
    }
}

void flagFReader(char **argv, int *fMas, int *mas,
    Flags *flag, regex_t *regex, int *countOfMatch, int eMark) {
    flag->e = 0;
    FILE *file;
    int i = 0;
    char bufferString[4096];
    bufferString[0] = '\0';
    char fBuffer[4096];
    fBuffer[0] = '\0';
    int x = 0;
    int endMark = 0;
    while (fMas[x]) {
        if ((file = fopen(argv[fMas[x]], "r")) != NULL) {
            while (feof(file) == 0) {
                fgets(bufferString, sizeof(bufferString), file);
                if (!feof(file)) {
                    if (bufferString[0] == '\n' && strlen(bufferString) < 2) {
                        flag->fcmp = 1;
                        flag->fcompare = 0;
                    }
                }
            }
            fseek(file, 0, SEEK_SET);
            while (feof(file) == 0) {
                fgets(bufferString, sizeof(bufferString), file);
                if (!strcmp(bufferString, "^$")) {
                    strcpy(bufferString, "^\n$");
                } else if (!strcmp(bufferString, ".")) {
                    strcpy(bufferString, "[^\n]");
                }
                if (flag->i) {
                    lowCaseConvert(bufferString);
                }
                strcat(fBuffer, bufferString);
                int length = strlen(fBuffer);
                if (fBuffer[length-1] == '\n') {
                    fBuffer[length-1] = '|';
                } else {
                    fBuffer[length] = '|';
                }
            }
            fclose(file);
        } else {
            if (!flag->s) {
                printf("file not found\n");
            }
            endMark = 1;
        }
        x++;
    }
    if (!endMark) {
        int length = strlen(fBuffer);
        if (fBuffer[length-1] == '|') {
            fBuffer[length-1] = '\0';
        }
        regcomp(regex, fBuffer, REG_EXTENDED);
        x = 0;
        int fCount = 0;
        int hMark = 0;
        if (flag->o) {
            while (fMas[x]) {
                fCount++;
                x++;
            }
            if (fCount > 1) {
                for  (int k = 0; k < fCount; k++) {
                    for (int h = 1; h < fCount; h++) {
                        if (k != h && !strcmp(argv[fMas[k]], argv[fMas[h]]) && flag->o && flag->fcompare) {
                            flag->fcompare = 1;
                        }
                        if (strcmp(argv[fMas[k]], argv[fMas[h]])) {
                            flag->fcompare = 0;
                            hMark = h;
                        }
                    }
                }
                flag->o = 0;
            } else {
                flag->fcompare = 0;
            }
        }
        if (fCount < 2) {
            flag->fcompare = 0;
        }
        while (mas[i]) {
            if (!strcmp(argv[mas[i]], argv[fMas[hMark]]) && fCount < 2) {
                flag->fcompare = 0;
            }
            distribute(argv[mas[i]], flag, regex, countOfMatch, eMark);
            if (flag->c && flag->l) {
                writeFromFileCL(*flag, argv[mas[i]], eMark, countOfMatch);
            }
            if (flag->c && !flag->l) {
                cResultWrite(argv[mas[i]], eMark, *flag, countOfMatch);
            }
            flag->ff = 0;
            *countOfMatch = 0;
            i++;
        }
        flag->fcmp = 0;
    }
}

void parser(char **argv, int argc, int *mas, int *eMas, int *fMas, int *pIndex, Flags *flag, int *eMark) {
    int eMasIndex = 0;
    int masIndex = 0;
    int fMasIndex = 0;
    for (int i = 0; i < 100; i++) {
        mas[i] = 0;
        eMas[i] = 0;
        fMas[i] = 0;
    }
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            flagRead(argv[i], flag);
            if (flag->f) {
                flag->fcompare = 1;
            }
            if (flag->l) {
                flag->n = 0;
                flag->o = 0;
            }
            flag->notEmpty = 1;
            int length = strlen(argv[i]);
            for (int j = 1; j < length; j++) {
                if (argv[i][j] == 'e') {
                    i++;
                    eMas[eMasIndex] = i;
                    eMasIndex++;
                }
                if (argv[i][j] == 'f') {
                    i++;
                    fMas[fMasIndex] = i;
                    fMasIndex++;
                }
            }
        } else {
            mas[masIndex] = i;
            masIndex++;
            *eMark += 1;
        }
    }
    *pIndex = mas[0];
    if (flag->i) {
        lowCaseConvert(argv[*pIndex]);
    }
}

void flagInitial(Flags *flag) {
    flag->e = 0;
    flag->i = 0;
    flag->v = 0;
    flag->c = 0;
    flag->l = 0;
    flag->n = 0;
    flag->h = 0;
    flag->s = 0;
    flag->f = 0;
    flag->o = 0;
    flag->notEmpty = 0;
    flag->ff = 0;
    flag->fcmp = 0;
    flag->fcompare = 0;
}

void flagRead(char *flagString, Flags *flag) {
    int length = strlen(flagString);
    for (int i = 0; i < length; i++) {
        if (flagString[i] == 'e') {
            flag->e = 1;
        }
        if (flagString[i] == 'i') {
            flag->i = 1;
        }
        if (flagString[i] == 'v') {
            flag->v = 1;
        }
        if (flagString[i] == 'c') {
            flag->c = 1;
        }
        if (flagString[i] == 'l') {
            flag->l = 1;
        }
        if (flagString[i] == 'n') {
            flag->n = 1;
        }
        if (flagString[i] == 'h') {
            flag->h = 1;
        }
        if (flagString[i] == 's') {
            flag->s = 1;
        }
        if (flagString[i] == 'f') {
            flag->f = 1;
        }
        if (flagString[i] == 'o') {
            flag->o = 1;
        }
    }
}

void lowCaseConvert(char *argv) {
    int length = strlen(argv);
    for (int i = 0; i < length; i++) {
        if (argv[i] > 64 && argv[i] < 91) {
            argv[i] += 32;
        }
    }
}

void distribute(char *argv, Flags *flag, regex_t *regex, int *countOfMatch, int eMark) {
    FILE *file;
    int numIndex = 0;
    char bufferString[4096];
    bufferString[0] = '\0';
    char bufferStringTemp[4096];
    bufferStringTemp[0] = '\0';
    int matchMark = 1;
    char markArgv[4096];
    markArgv[0] = '\0';
    int length;
    if ((file = fopen(argv, "r")) != NULL) {
        while (!feof(file)) {
            fgets(bufferString, sizeof(bufferString), file);
            if (flag->fcmp) {
                matchMark = 0;
            }
            numIndex++;
            strcpy(bufferStringTemp, bufferString);
            if (flag->fcompare && !flag->v) {
                length = strlen(bufferStringTemp);
                if (flag->f && bufferStringTemp[length-1] != '\n') {
                    strcat(bufferStringTemp, "\n");
                }
                strcat(bufferStringTemp, bufferString);
                strcpy(bufferString, bufferStringTemp);
            }
            if (!flag->notEmpty) {
                matchMark = regexec(regex, bufferString, 0, NULL, 0);
                if (!matchMark) {
                    writeFromFile(matchMark, eMark, argv, *flag, bufferString, numIndex);
                }
            } else {
                if (flag->i) {
                    lowCaseConvert(bufferStringTemp);
                }
                if (!flag->fcmp) {
                    matchMark = regexec(regex, bufferStringTemp, 0, NULL, 0);
                }
                if ((!matchMark && !flag->v) || (matchMark && flag->v)) {
                    *countOfMatch += 1;
                }
                if (flag->c && flag->l && *countOfMatch > 0) {
                        *countOfMatch = 1;
                }
                if (!flag->c && !flag->o) {
                    if (strcmp(markArgv, argv) && (!flag->ff || !flag->f)
                        && ((flag->l && !matchMark && !flag->v)
                        || (matchMark && flag->v && flag->l))) {
                        writeFromFile(matchMark, eMark, argv, *flag, bufferString, numIndex);
                        if ((matchMark && flag->v) || (!matchMark && !flag->v)) {
                            strcpy(markArgv, argv);
                        }
                        flag->ff = 1;
                    }
                    if (!flag->l) {
                        writeFromFile(matchMark, eMark, argv, *flag, bufferString, numIndex);
                    }
                }
                if (flag->o && !flag->c) {
                    oResultWrite(bufferString, argv, eMark, regex, *flag, numIndex);
                }
            }
            bufferString[0] = '\n';
        }
        flag->ff = 1;
        fclose(file);
    } else {
        if (!flag->s) {
            printf("file not found\n");
        }
    }
}

void writeFromFileCL(Flags flag, char *argv, int eMark, int *countOfMatch) {
    cResultWrite(argv, eMark, flag, countOfMatch);
    if (flag.l) {
        if (*countOfMatch > 0) {
            printf("%s\n", argv);
        }
    }
}

void writeFromFile(int matchMark, int eMark, char *argv, Flags flag, char *bufferString, int numIndex) {
    if ((!matchMark && !flag.v) || (matchMark && flag.v) || (flag.l && flag.c)) {
        if (eMark >= 2 && !flag.l && !flag.h) {
            printf("%s:", argv);
        }
        if (flag.n) {
            printf("%d:", numIndex);
        }
        if (flag.l) {
            printf("%s\n", argv);
        } else {
            printf("%s", bufferString);
            int length = strlen(bufferString);
            if (bufferString[length-1] != '\n') {
                printf("\n");
            }
        }
    }
}

void oResultWrite(char *bufferString, char *argv, int eMark, regex_t *regex, Flags flag, int numIndex) {
    regmatch_t pm;
    size_t offset = 0;
    long long unsigned int i;
    long long unsigned int length = strlen(bufferString);
    int mark = 0;
    char bufferStringTemp[4096];
    strcpy(bufferStringTemp, bufferString);
    if (flag.i) {
        lowCaseConvert(bufferStringTemp);
    }
    if (regexec(regex, bufferStringTemp, 0, NULL, 0) && flag.v && !flag.fcmp) {
        if (eMark >= 2 && !flag.l && !flag.h) {
            printf("%s:", argv);
        }
        if (flag.n) {
            printf("%d:", numIndex);
        }
        if (flag.l) {
            printf("%s\n", argv);
        } else {
            printf("%s", bufferString);
            int length = strlen(bufferString);
            if (bufferString[length-1] != '\n') {
                printf("\n");
            }
        }
    } else {
        if (flag.fcmp && !flag.v && flag.f) {
            if (eMark >= 2 && !flag.l && !flag.h) {
                    printf("%s:", argv);
                }
                if (flag.n) {
                    printf("%d:", numIndex);
                }
                printf("%s", bufferString);
                if (bufferString[length-1] != '\n') {
                printf("\n");
            }
        }
        if (bufferString[length-1] == '\n' && length > 1) {
            bufferString[length-1] = '\0';
        }
        while (!regexec(regex, bufferString + offset, 1, &pm, 0) && !flag.v && (!flag.fcmp || !flag.f)) {
            if (!mark) {
                if (eMark >= 2 && !flag.l && !flag.h) {
                    printf("%s:", argv);
                }
                if (flag.n) {
                    printf("%d:", numIndex);
                }
                mark = 1;
            }
            for (i = pm.rm_so+offset; i < pm.rm_eo+offset; i++) {
                printf("%c", bufferString[i]);
            }
            offset += pm.rm_eo;
            if (strcmp(bufferString, "\n")) {
                printf("\n");
            }
            if (pm.rm_so == pm.rm_eo) {
                offset += 1;
            }
        }
    }
}

void cResultWrite(char *argv, int eMark, Flags flag, int *countOfMatch) {
    if (eMark >= 2 && (!flag.l || (flag.l && flag.c)) && !flag.h) {
        printf("%s:", argv);
    }
    printf("%d\n", *countOfMatch);
}
