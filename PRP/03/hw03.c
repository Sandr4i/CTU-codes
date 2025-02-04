#include <stdio.h>

#define ERROR_INPUT 100
#define ERROR_INTERVAL 101
#define ERROR_WIDTH 102
#define ERROR_FENSE 103
#define SUCCESS 0
#define maximum 69
#define minimum 3

void print_roof(int n, int m);

void print_simple_house(int n, int m);

void print_faigls_house(int n, int m, int k);

int main()
{
    int n,m,k; // n - width; m - height; k - height of fence
    int rep1=scanf("%d %d", &n, &m);
    if (rep1 != 2)
    {
        fprintf(stderr, "Error: Chybny vstup!\n");
        return ERROR_INPUT;
    }
    if ((n<3 || n>69) || (m<3 || m>69))
    {
        fprintf(stderr, "Error: Vstup mimo interval!\n");
        return ERROR_INTERVAL;
    }
    if (n%2==0)
    {
        fprintf(stderr, "Error: Sirka neni liche cislo!\n");
        return ERROR_WIDTH;
    }
    if (n==m)
    {
        int rep2=(scanf("%d",&k)==1);
        if (rep2 && ((k<1) || (k>=m)))
        {
            fprintf(stderr, "Error: Neplatna velikost plotu!\n");
            return ERROR_FENSE;
        }
        else if (!rep2)
        {
            fprintf(stderr, "Error: Chybny vstup!\n");
            return ERROR_INPUT;
        }
    }
    print_roof(n,m);
    if (n==m) print_faigls_house(n,m,k);
    else print_simple_house(n,m);
    return SUCCESS;
}

void print_roof(int n, int m)
{
    int count;
    for(int i = 0; i<n/2; i++)
    {
        count=n/2-i;
        while(count>0)
        {
            printf(" ");
            count--;
        }
        if (i!=0 && i!=n/2) 
        {
            printf("X");
            for (int j = i*1.99; j>0; j--)
            {
                printf(" ");
            }
            printf("X\n");
        }
        else printf("X\n");
    }
}

void print_simple_house(int n, int m)
{
    for (int i = 0; i<m; i++)
    {
        for (int j = 0; j<n-1; j++)
        {
            if ((i!=0 && i!=m-1) && (j!=0 && j!=n-1))
            {
                printf(" ");
            }
            else printf("X"); 
        }
        printf("X\n");
    }
}

void print_faigls_house(int n, int m, int k)
{
    // i - height; j - width
    for (int i = 0; i<m; i++)
    {
        for (int j = 0; j<n; j++)
        {

            if ((i!=0 && i!=m-1) && (j!=0 && j!=n-1))
            {
                if (i%2==0 && j%2==0) printf("o");
                else if (i%2==0 && j%2!=0) printf("*");
                else if (i%2!=0 && j%2==0) printf("*");
                else if (i%2!=0 && j%2!=0) printf("o");
            }
            else printf("X");

        }
        if (i==n-k || i == m-1)
        {
            if (k%2==0)
            {
                for (int y = 0; y<k; y++)
                {
                    if (y%2==0) printf("-");
                    else printf("|");
                }
            }
            else
            {
                for (int y = 0; y<k; y++)
                {
                    if (y%2==0) printf("|");
                    else printf("-");
                }
            }
        }
        else if (i>n-k)
        {
            if (k%2==0)
            {
                for (int y = 0; y<k; y++)
                {
                    if (y%2==0) printf(" ");
                    else printf("|");
                }
            }
            else
            {
                for (int y = 0; y<k; y++)
                {
                    if (y%2==0) printf("|");
                    else printf(" ");
                }
            }
        }
        printf("\n");
    }
}

