#include <stdio.h>
#include <stdlib.h>

#define ALLOC 10

typedef struct {
    char* line;
    int size;
    int capacity;
} line;

enum {
    no_end = 0,
    end_of_line = 1,
    end_of_file = 2
};

enum {
    no_find = 0,
    find = 1
};

void init_line(line* myLine);   // this function allocates memory

void realoc_line(line* myLine);  // add more memory if it needed

void free_line(line* myLine);   // free memory

int read_line(line* myLine, FILE* f);   // reads line from file

void open_file(FILE** f, char* filename);   //opens file

int length(char* str);  // calculates length of string

int find_line(line *myLine, char* pattern);   // finds string in line

int find_b(line myLine, char*  pattern);   //   finds pattern

void close_file(FILE** f);  // closes file

char pattern_symbol(char* pattern);    // search for pattern symbol

int main(int argc, char *argv[])   // main function
{
    int ret = 0;
    line *l = malloc(sizeof(line));
    FILE* f;
    if (argc<4) // if there is no -E operator
    {
        open_file(&f, argv[2]);
        int read = no_end;
        while (read!=end_of_file)
        {
            init_line(l);
            read = read_line(l, f);
            if (find_line(l, argv[1]))
            {
                ret++;
                printf("%s\n", l->line);
            }
            free_line(l);
        }
        free(l);
        close_file(&f);
        return !ret;
    }
    else  // if there is a -E operator
    {
        open_file(&f, argv[3]);
        int read = no_end;
        while (read!=end_of_file)
        {
            init_line(l);
            read = read_line(l, f);
            if (find_b(*l, argv[2]))
            {
                ret++;
                printf("%s\n", l->line);
            }
            free_line(l);

        }
        free(l);
        close_file(&f);
        return !ret;
    }
}

void init_line(line* myLine)
{
    myLine->line = (char*) malloc(sizeof(char));
    myLine->size = 0;
    myLine->capacity = 1;
    myLine->line[0]='\0';
}

void realloc_line(line* myLine)
{
    myLine->capacity += ALLOC;
    myLine->line = realloc(myLine->line, myLine->capacity * sizeof(char));
}

void free_line(line* myLine)
{
    if (myLine->line != NULL) 
    {
        free(myLine->line);
    }
}

int read_line(line* myLine, FILE* f)
{
    int ret = no_end;
    while(!ret)
    {
        char s = fgetc(f);
        if (myLine->size + 2 >= myLine->capacity)
        {
            realloc_line(myLine);
        }
        switch (s)
        {
            case '\n':
                ret = end_of_line;
                break;
            case EOF:
                ret = end_of_file;
                break;
            default:
                myLine->line[myLine->size++] = s;
                break;
        }
    }
    myLine->line[myLine->size++]='\0';
    return ret;
}

int length(char* str)
{
    int temp = 0;
    while(str[temp]) 
    {
        temp++;
    }
    return temp;
}

int find_line(line *myLine, char* pattern)
{
    int len = length(pattern);
    if(!len) 
    {
        return find;
    }
    int i = 0;
    while (myLine->line[i]!='\0') 
    {
        if (myLine->line[i] == pattern[0]) 
        {
            int j = 1;
            while (j < len && i + j < myLine->size && myLine->line[i + j] == pattern[j]) 
            {
                j++;
            }
            if (j == len) 
            {
                return find;
            }
        }
        i++;
    }
    return no_find;
}

int find_b(line myLine, char*  pattern)
{
    char s = pattern_symbol(pattern);
    int len = length(pattern);
    if(!len) 
    {
        return find;
    }
    int pat_index=0;
    for (int i=0; i<len; i++)
    {
        if (pattern[i]==s)
        {
            pat_index=i;
            break;
        }
    }
    if (s == '?')
    {
        int i=0;
        int temp_len = length(myLine.line);
        while (i<pat_index-1 && myLine.line[i+1]==pattern[i+1])
        {
            i++;
        }
        if (myLine.line[i+1]!=pattern[i+1])
        {
            for (int n = i+1; n<len; n++)
            {
                i++;
                if (myLine.line[i]!=pattern[n]) break;
            }
            if (i==temp_len-1)
            {
                return find;
            }
        }
        else if (myLine.line[i]==pattern[pat_index-1])
        {
            i++;
            for (int n = i+1; n<len; n++)
            {
                i++;
                if (myLine.line[i]!=pattern[n]) break;
            }
            if (i==temp_len-1) 
            {
                return find;
            }
        }
    }
    else if (s == '*')
    {
        int temp_len = length(myLine.line);
        int i=0;
        while (i<pat_index-1)
        {
           
            if (i<temp_len-1)
            {
               
                if (myLine.line[i+1]==pattern[i+1]) 
                {
                    
                    i++;
                }
                else break;
            }
            else break;
        }
        if (i<temp_len-1)
        {
            if (myLine.line[i+1]!=pattern[pat_index-1] && myLine.line[i]!=pattern[pat_index-1])
            {
                for (int n = i+3; n<len; n++)
                {
                    
                    i++;
                    if (myLine.line[i]==pattern[n]) break;
                }
                if (i==temp_len-1) return find;
            }
            else
            { 
                while (myLine.line[i]==pattern[pat_index-1])
                {
                    i++;
                    
                }
                int j=0;
                while (myLine.line[i]==pattern[pat_index+j] && i<temp_len)
                {
                    i++;
                    j++;
                    
                }
                if (i==temp_len-1) return find;
            }
        }
    }
    else if (s == '+')
        {
            int temp_len = length(myLine.line);
        int i=0;
        while (i<pat_index-1 && myLine.line[i]==pattern[i])
        {
            i++;
        }
        i++;
        while (myLine.line[i]==pattern[pat_index-1])
        {
            i++;
        }
        int j=0;
        while (myLine.line[i]==pattern[pat_index+j] && i<temp_len)
        {
            i++;
            j++;
        }
        if (i==temp_len-1) return find;
        }
    return no_find;
}

void open_file(FILE** f, char* filename)
{
    if (filename)
    {
        *f = fopen(filename, "r");
        if (*f == NULL) 
        {
            exit(1);
        }
    }
    else 
    {
        *f = stdin;
    }
}

void close_file(FILE** f)
{
    if (fclose(*f) == EOF){
        exit(1);
    }
}

char pattern_symbol(char* pattern)
{
    char ret='\0';
    int size = length(pattern);
    for (int i = 0; i<size; i++)
    {
        if (pattern[i]=='?' || pattern[i]=='*' || pattern[i]=='+')
        {
            ret=pattern[i];
            break;
        }
    }
    return ret;
}
