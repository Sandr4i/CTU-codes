#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR 100

int read_matrix(int *rep);

int test();

void print(int r, int c, int m[r][c]);

int main()
{
    int rep;
    read_matrix(&rep);
    return rep;
}

int read_matrix(int *rep, int r, int c, int *m[r][c])
{
    rep=SUCCESS;
    int r,c;
    if (scanf("%d %d", &r, &c)==2 && r>=0 && c>=0)
    {
        int m1[r][c];
        for (int i=0; i<r; i++)
        {
            for (int j=0; j<c; j++)
            {
                if (scanf("%d", &m1[i][j])!=1)
                {
                    rep=ERROR;
                    return rep;
                }
            }
        }
    }
    return rep;
}

void print(int r, int c, int m[r][c])
{
    for (int i = 0; i<r; i++)
    {
        for (int j = 0; j<c; j++)
        {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}