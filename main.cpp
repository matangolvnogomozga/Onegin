/** This program sort incoming file and write the result in outgoing file **/

#include <sort.h>

const long MAXSYMB = 10e6; ///max number of symbols in file
const int MAXLINES = 10e6; ///max number of lines in file

int main()
{

    /** Open files **/
    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");

    /** File reading **/
    long symb = CountSymbols(input);
    char* array = FileToArray(input, symb);
    int lin = CountLines(array, symb);
    line* str = ArrayToStruct(array, lin, symb);

    /** Sorting of the file by rhymes **/
    qsort(str, lin, sizeof(line), ComparEnd);
    fputs("Sorting by rhymes\n", output);
    StructToFile(str, lin, output);
    fputs("\n", output);

    /** Sorting of the file by alphabet **/
    qsort(str, lin, sizeof(line), Compar);
    fputs("Sorting by alphabet\n", output);
    StructToFile(str, lin, output);
    fputs("\n", output);

    /** Origin text **/
    fputs("Origin text\n", output);
    ArrayToFile(array, symb, output);
    fputs("\n", output);

    fclose(output);
    fclose(input);
    free(array);
    free(str);
    return 0;
}
