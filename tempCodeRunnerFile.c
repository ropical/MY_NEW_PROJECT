#include <stdio.h>
#include <math.h>

int a(int x, int y)
{
    if (x >= y) return x - y;
    else return y - x;
}

int main()
{
    int sum = 0;
    for (int i1 = 0; i1 < 10; i1++)
    {
        for (int j1 = 0; j1 < 10; j1++)
        {
            for (int k1 = 0; k1 < 10; k1++)
            {
                for (int t1 = 0; t1 < 10; t1++)
                {
                    sum += a(i1,k1) + a(j1,t1);
                }
            }
        }
    }
    printf("%d", sum / 10000);
}