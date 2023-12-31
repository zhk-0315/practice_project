#include <stdio.h>

void lc_insert_sort(int a[], int cnt)
{
    int i = 0;
    int j = 0;
    int tmp = 0;

    for (i = 1; i < cnt; i++) {
        tmp = a[i];
        for (j = i; j > 0 && tmp > a[j - 1]; j--) {
            a[j] = a[j - 1];
        }
        a[j] = tmp;
    }

    for (i = 0; i < cnt; i++) {
        printf("%d ", a[i]);
    }

    printf("\n");
}

int main(int argc, const char* argv[])
{
    int a[] = { 11, 22, 332, 1, 2, 3, 7, 8, 9, 4, 5, 6, 0 };

    // int a[] = { 33, 22, 11, 00 };

    lc_insert_sort(a, sizeof(a) / sizeof(a[0]));

    return 0;
}