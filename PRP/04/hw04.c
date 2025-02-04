#include <stdio.h>
#include <math.h>

#define ERROR 100
#define SUCCESS 0

void find_prime_numbers(long long int *n);  // function that finds all prime numbers before the entered number

void prime_decomposition(long long int **n, int *k); // function that do prime decomposition of entered number

int main()
{
    long long int n;
    int rep=scanf("%lld", &n); // for checking if value is a number
    while(n!=0)
    {
        if (n<0 || rep!=1)
        {
            fprintf(stderr, "Error: Chybny vstup!\n");
            return ERROR;
        }   
        find_prime_numbers(&n);
        printf("\n");
        rep=scanf("%lld", &n);
    }
    return SUCCESS;
}

void find_prime_numbers(long long int *n)
{
    // if number is less than 4 (1,2,3) then it is a simple number
    if (*n<4) printf("Prvociselny rozklad cisla %lld je:\n%lld", *n, *n);
    else
    {
        // else we create a array with 0; indexes will be used to divide a number by indexes; 0 under index means that this index is a simple number
        long long int a[*n];
        for (int i=0; i<*n; i++)
        {
            a[i]=0;
        }
        //Sieve of Eratosthenes
        for (int i = 2; i*i<=*n; i++)
        {
            if (a[i]==1) continue;
            for (int j = i*2; j<*n ;j+=i)
            {
                if (j%i==0) a[j]=1;
            }
        }
        int c=0;
        printf("Prvociselny rozklad cisla %lld je:\n", *n);
        for (int i = 2; i<=(int)(sqrt(*n)); i++) 
        {
            if (i==2) // check if entered not divided number is a simple number
            {
                for(int j=2; j<*n; j++)
                {
                    if (*n%j==0)
                    {
                        c+=1;
                        break;
                    }
                }
                if(c==0) 
                {
                    printf("%lld", *n);
                    break;
                }
                else
                {
                    if (a[i]==0) prime_decomposition(&n, &i);
                    else continue;
                }
            }
            else
            {
                if (a[i]==0) prime_decomposition(&n, &i);
                else continue;
            }
        }
    }
}

void prime_decomposition(long long int **n, int *k)
{
    int c=0;
    while(**n%*k==0)
    {
        **n/=*k;
        c++;
    }
    if(c!=0)
    {
        if (c>1 && **n!=1) printf("%d^%d x ", *k, c);
        else if (c==1 && **n!=1) printf("%d x ", *k);
        else if (c>1 && **n==1) printf("%d^%d", *k, c);
        else if (c==1 && **n==1) printf("%d", *k);
    }
    if (**n!=1) //check if entered divided number is a simple number
    {
        c=0;
        for(int i=2; i<**n; i++)
        {
            if (**n%i==0)
            {
                c+=1;
                break;
            }
        }
        if(c==0 && **n!=1)
        {
            printf("%lld", **n);
            **n=1;
        }
    }      
}
