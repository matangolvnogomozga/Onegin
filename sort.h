#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

const long MAXSYMB = 10e6; ///max number of symbols in file
const int MAXLINES = 10e6; ///max number of lines in file

/** Struct "line" contains pointer to the start of the line end it's length **/
struct line
{
    char* pointer;
    int len;
};

long CountSymbols(FILE *f);
int CountLines(char text[], int max);
char* FileToArray(FILE* file, long numsymb);
line* ArrayToStruct(char text[], int numline, long numsymb);
int Compar(const void* a, const void* b);
char* PunctOut(line str);
int Min(int a, int b);
void StructToFile(line* text, int number, FILE* file);
int ComparEnd(const void* a, const void* b);
void ArrayToFile(char text[], long numsymb, FILE* file);
