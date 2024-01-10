#include <stdio.h>
#include <stdlib.h>

void foo1(int* x)
{
    printf("inside foo: x = %d\n", *x);
    printf("inside foo: address of x = %n\n", x);
    x = (int *)malloc(sizeof(int));
    *x = 5;
    printf("inside foo after allocation: x = %d\n", *x);
}


int main1()
{
    int y = 99;
    foo1(&y);
    printf("Main: %d\n", y);

    return 0;
}



void foo2(int** x)
{
    *x = (int *)malloc(sizeof(int));
    **x = 5;
}

int main2()
{
    int y = 99;
    int* ptr = &y;
    foo2(&ptr);
    printf("Main: %d\n", *ptr);
    return 0;
}

int main()
{
    main1();
    return 0;
}