#include <stdio.h>
 
// This is one of my first programs ever, so it's logic is not good 

int main()
{
    int n;
    int count = 0; // pocet vsech cisel
    int sud = 0; //pocet sudych
    int lich = 0; // pocet lichych
    int klad = 0; // pocet kladnych
    int zap = 0; // pocet zapornych
    int sum = 0; //suma vsech cisel
    int rep = scanf("%d",&n);
    int max=n; 
    int min=n;
    int next; // zapisuje nasledujici cislo
    if (n<-10000||n>10000)
        {   
            printf("Error: vstup je mimo interval!\n");
            return 100;
        }
    while (rep!=EOF)
    {
        count+=1;
        if (n>max) max=n;
        else if (n<min) min=n;
        if (n%2==0) sud+=1;
        else lich+=1;
        if (n<0 && n!=0) zap+=1;
        else if (n>0 && n!=0)klad+=1;
        sum+=n;
        if ((rep=(scanf("%d", &next))!=EOF && (next>=-10000&&next<=10000))) 
        {
            printf("%d, ", n);
            rep=1;
        }
        else
        {
            printf("%d\n",n);
            rep=EOF;
        }
        n=next;
        if (n<-10000||n>10000)
        {   
            printf("Error: Vstup je mimo interval!\n");
            return 100;
        }
    }
    printf("Pocet cisel: %d\n", count);
    if (count !=0)
    {
        printf("Pocet kladnych: %d\n", klad);
        printf("Pocet zapornych: %d\n", zap);
        printf("Procento kladnych: %.2f\n", ((100 * (klad*1.0))/count));
        printf("Procento zapornych: %.2f\n", ((100 * zap)/(count*1.0)));
        printf("Pocet sudych: %d\n", sud);
        printf("Pocet lichych: %d\n", lich);  
        printf("Procento sudych: %.2f\n", ((100 * sud)/(count*1.0)));
        printf("Procento lichych: %.2f\n", ((100 * lich)/(count*1.0)));
        printf("Prumer: %.2f\n", ((sum*1.0)/count));
    }
    else
    {
        printf("Pocet kladnych: 0\n");
        printf("Pocet zapornych: 0\n");
        printf("Procento kladnych: 0.00\n");
        printf("Procento zapornych: 0.00\n");
        printf("Pocet sudych: 0\n");
        printf("Pocet lichych: 0\n");  
        printf("Procento sudych: 0.00\n");
        printf("Procento lichych: 0.00\n");
        printf("Prumer: 0.00\n");
    }
    printf("Maximum: %d\n", max);
    printf("Minimum: %d\n", min);
    return 0;
}

