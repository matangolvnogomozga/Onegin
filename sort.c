#include "sort.h"

//-----------------------------------------------
//! Function "CountSymbols" count symbols in file
//!
//!@param [in] file File we are working with
//!
//!@return Number of symbols
//!
//-----------------------------------------------
long CountSymbols(FILE* file)
{
    /** Checking correctness of entry **/
    assert(file != NULL);

    /** Counting symbols **/
    fseek(file, 0, SEEK_END);
    long count = ftell(file);
    rewind(file);

    /** Checking emptiness **/
    if(count == 0)
    {
        printf("File is empty");
        exit(1);
    }
    /** Checking overfilled **/
    if(count > MAXSYMB)
    {
        printf("Too big file");
        exit(1);
    }

    return count;
}

//------------------------------------------------------------------------
//! Function "CountLines" count lines in array and change all '\n' to '\0'
//!
//!@param [in] text Array we are working with
//!@param [in] numsymb Number of symbols in array
//!
//!@return Number of lines
//!
//!@note Changing is needed for convenience in futher work
//------------------------------------------------------------------------
int CountLines(char text[], int numsymb)
{
    /** Checking correctness of entry **/
    assert(text != NULL);
    assert(numsymb != 0);
    assert(numsymb <= MAXSYMB);

    /** Counting lines and changing **/
    int count = 1;
    for(int i = 0; (i < numsymb) && (text[i] != '\0'); ++i)
        if(text[i] == '\n')
        {
            text[i] = '\0';
            ++count;
        }

    /** Checking emptiness **/
    if((count == 2) && (text[0] == '\0'))
    {
        printf("File is empty");
        exit(1);
    }
    /** Checking overfilled **/
    if(count > MAXLINES)
    {
        printf("Too many lines");
        exit(1);
    }
    return count;
}

//----------------------------------------------
//! Function "FileToArray" write file to array
//!
//!@param [in] file File we are working with
//!@param [in] numsymb Number of symbols in file
//!
//!@return Pointer to the start of the array
//!
//----------------------------------------------
char* FileToArray(FILE* file, long numsymb)
{
    /** Checking correctness of entry **/
    assert(file != NULL);
    assert(numsymb != 0);
    assert(numsymb <= MAXSYMB);

    /** Creating array of symbols **/
    char* text = (char*)calloc(numsymb, sizeof(char));
    fread(text, sizeof(char), numsymb, file);
    return text;
}

//----------------------------------------------------------------------------------
//! Function "ArrayToStruct" write data from array of symbols to array of structures
//!
//!@param [in] text Array we are working with
//!@param [in] numline Number of lines in original file
//!@param [in] numsymb Number of symbols in original file
//!
//!@return Pointer to the start of the array of structures
//!
//----------------------------------------------------------------------------------
line* ArrayToStruct(char text[], int numline, long numsymb)
{
    /** Checking correctness of entry **/
    assert(text != NULL);
    assert(numsymb != 0);
    assert(numline != 0);
    assert(numsymb <= MAXSYMB);
    assert(numline <= MAXLINES);

    /** Creating array of structures **/
    line* str = (line*)calloc(numline, sizeof(line));
    str[0].pointer = text; /// beginning of array
    int length = 0;
    long j = 0;

    /** Filling the array **/
    for(int i = 0; i < numline; ++i)
        for(j; j < numsymb; ++j)
        {
            if(text[j] != '\0')
            {
                ++length;
                continue;
            }
            str[i].len = length + 1;
            if((i + 1 < numline) && (j + 1 < numsymb)) /// the end is not reached
                str[i+1].pointer = &text[j+1];
            length = 0;
            ++j;
            break;
        }
    return str;
}

//-------------------------------------------------------------------------------------
//! Function "Compar" compares lines using the pointer from the structure
//!                                  and ignoring signes in the start and high-low case
//!
//!@param [in] a Pointer to a struct 1
//!@param [in] b Pointer to a struct 2
//!
//!@return  0, if line 1 = line 2
//!        <0, if line 1 < line 2
//!        >0, if line 1 > line 2
//-------------------------------------------------------------------------------------
int Compar(const void* a, const void* b)
{
    /** Checking correctness of entry **/
    assert(a != NULL);
    assert(b != NULL);

    line a2 = *(line *)a; /// turning
    line b2 = *(line *)b; /// to a structure

    char* str_a = a2.pointer; /// turning
    char* str_b = b2.pointer; /// to lines

    str_a = PunctOut(a2); /// ignoring signes
    str_b = PunctOut(b2); ///  in the start
    int res = 0;
    int len_a = strlen(str_a);
    int len_b = strlen(str_b);
    int min = Min(len_a, len_b);
    for(int i = 0; i < min; ++i)
    {
        if(tolower(str_a[i]) == tolower(str_b[i])) /// ignoring high-low case
            continue;
        else
        {
            res = tolower(str_a[i]) - tolower(str_b[i]);
            break;
        }
    }
    return res;
}

//------------------------------------------------------------
//! Function "PunctOut" ignore signes in the start of the line
//!
//!@param [in] str Struct we are working with
//!
//!@return Pointer to the end of the appearance of signes
//!
//------------------------------------------------------------
char* PunctOut(line* str)
{
    int i = 0;
    for(i; i < str.len; ++i)
    {
        if(!isalpha(str.pointer[i]))
            continue;
        else
            break; /// when signes are over
    }
    return (str + i);
}

//------------------------------------------------------------
//! Function "Min" compare two integer numbers
//!
//!@param [in] a First number
//!@param [in] b Second number
//!
//!@return Least number
//!
//------------------------------------------------------------
int Min(int a, int b)
{
    return (a < b) ? a : b;
}

//---------------------------------------------------------------------
//! Function "StructToFile" write data from array of structures to file
//!
//!@param [in] text Structure we are working with
//!@param [in] number Number of elements in array of structures
//!@param [in] file Outgoing file
//!
//!
//---------------------------------------------------------------------
void StructToFile(line* text, int number, FILE* file)
{
    /** Checking correctness of entry **/
    assert(text != NULL);
    assert(number != 0);
    assert(file != NULL);

    /** Writing in file **/
    for(int i = 0; i < number; ++i)
    {
        fputs(text[i].pointer, file);
        fputs("\n", file);
    }
    return;
}

//----------------------------------------------------------------------------------------
//! Function "ComparEnd" compares by rhymes lines using the pointer from the structure
//!                                     and ignoring signes in the start and high-low case
//!
//!@param [in] a Pointer to a struct 1
//!@param [in] b Pointer to a struct 2
//!
//!@return  0, if line 1 = line 2
//!        <0, if line 1 < line 2
//!        >0, if line 1 > line 2
//----------------------------------------------------------------------------------------
int ComparEnd(const void* a, const void* b)
{
    /** Checking correctness of entry **/
    assert(a != NULL);
    assert(b != NULL);

    line a2 = *(line *)a; /// turning
    line b2 = *(line *)b; /// to a structure

    char* str_a = a2.pointer; /// turning
    char* str_b = b2.pointer; /// to lines

    int res = 0;

    /** Ignored signes in the end **/
    for(int j = 0; ; ++j)
        if(isalpha(str_a[a2.len-j]))
        {
            len_a -= j;
            break;
        }
    for(int j = 0; ; ++j)
        if(isalpha(str_b[b2.len-j]))
        {
            len_b -= j;
            break;
        }

    /** Sorting be rhymes **/
    int min = Min(a2.len, b2.len);
    for(int i = 0; i < min; ++i)
    {
        if(tolower(str_a[a2.len-i]) == tolower(str_b[b2.len-i])) /// ignoring high-low case
            continue;
        else
        {
            res = tolower(str_a[a2.len-i]) - tolower(str_b[b2.len-i]);
            break;
        }
    }
    return res;
}

//------------------------------------------------------
//! Function "ArrayToFile" write data from array to file
//!
//!@param [in] text Array we are working with
//!@param [in] numsymb Number of elements in array
//!@param [in] file Outgoing file
//!
//------------------------------------------------------
void ArrayToFile(char text[], long numsymb, FILE* file)
{
    /** Checking correctness of entry **/
    assert(text != NULL);
    assert(file != NULL);
    assert(numsymb != 0);
    assert(numsymb <= MAXSYMB);

    /** Writing in file **/
    for(int i = 0; i < numsymb; ++i)
    {
        if(text[i] == '\0')
        {
            fputs("\n", file);
            continue;
        }
        fputc(text[i], file);
    }
    return;
}
