#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SUCCESS 0
#define ERROR_INPUT 100
#define ERROR_LENGTH 101


// a-z --> 97-122    A-Z --> 65-90

char *read_message(int *len);

int levenshtein_distance(char *msg1, int len1, char *msg2, int len2);

void compare(char *msg, int len, char *msg2);

void shift(char *msg, int len);

void rotate(char *msg, int offset, int len);

void print_message(char *msg, int len);

int main(int argc, char *argv[])
{
    _Bool arg = argc > 1 ? strcmp(argv[1], "-prp-optional") == 0 : false;
    char *msg_input, *msg;
    int msg_input_len, msg_len;

    msg_input = msg = NULL;
    msg_input_len = msg_len = 0;

    msg_input = read_message(&msg_input_len);
    if ((msg_input))
    {
        msg = read_message(&msg_len);
        if (!(msg))
        {
            fprintf(stderr, "Error: Chybny vstup!\n");
            free(msg_input);
            free(msg);
            return ERROR_INPUT;
        }
    }
    else 
    {
        fprintf(stderr, "Error: Chybny vstup!\n");
        free(msg_input);
        return ERROR_INPUT;
    }
    if (arg==0 && (msg_input_len != msg_len))
    {
        fprintf(stderr, "Error: Chybna delka vstupu!\n");
        free(msg_input);
        free(msg);
        return ERROR_LENGTH;
    }
    if (arg)
    {
        levenshtein_distance(msg, msg_len, msg_input, msg_input_len);
    }
    else
    {   
        compare(msg_input, msg_len, msg);
    }
    print_message(msg_input, msg_input_len);
    free(msg_input);
    free(msg);
    return SUCCESS;
}

char *read_message(int *len)
{
    int max_capacity=1;
    int leng=0;
    char *msg = malloc(max_capacity*sizeof(char)); 
    char c=getchar();
    while (c!=EOF && c!='\n')
    {
        if (!((c>='a' && c<='z')||(c>='A' && c<='Z')))
        {
            free(msg);
            msg=NULL;
            leng = 0;
            break;
        }
        else
        {
            if (leng==max_capacity)
            {
                msg = realloc(msg, (leng+1) * sizeof(char));
                if (msg == NULL)
                {
                    free(msg);
                    msg = NULL;
                    leng = 0;
                    break;
                }
                max_capacity+=1;
            }
            msg[leng++]=c;
            c=getchar(); 
        }
    }
    *len=leng;
    return msg;
}

void rotate(char *msg, int offset, int len)
{
    while(offset>0)
    {
        shift(msg, len);
        offset--;   
    }
}

void shift(char *msg, int len)
{
    for (int i=0; i<len; i++)
    {
        if (msg[i]==90 || (msg[i]>90 && msg[i]<97)) msg[i]=97;
        else if (msg[i]>=122) msg[i]=65;
        else msg[i]+=1;
    }
}

void compare(char *msg, int len, char *msg2)
{
    int max_similar=0, max_similar_offset=0, similar=0, offset=0;
    int j=0;  //number of max offsets
    while(j<52)
    {
        shift(msg, len);
        offset++;
        for (int i=0; i<len; i++)
        {
            if (msg[i]==msg2[i]) similar++;
        }
        if (similar>max_similar)
        {
            max_similar=similar;    
            max_similar_offset=offset;
        }
        j++;
        similar=0;
    }
    rotate(msg, max_similar_offset, len);
}

int levenshtein_distance(char *msg1, int len1, char *msg2, int len2)
{
    return 0;
}

void print_message(char *msg, int len)
{
    if (msg)
    {
        for (int i = 0; i < len; i++)
        {
            printf("%c", msg[i]);
        }
        printf("\n");
    }
}
