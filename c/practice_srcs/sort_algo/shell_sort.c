#include <stdio.h>

void lc_shell_sort(int* a, int cnt)
{
    int i = 0;
    int j = 0;
    int gap = 0;
    int tmp = 0;

    for (gap = cnt >> 1; gap > 0; gap >>= 1) {
        for (i = 0; i < cnt; i++) {
            tmp = a[i];
            for (j = i; j >= gap && tmp > a[j - gap]; j -= gap) {
                a[j] = a[j - gap];
            }
            a[j] = tmp;
        }
    }

    for (i = 0; i < cnt; i++) {
        printf("%d ", a[i]);
    }

    printf("\n");
}

int main(int argc, const char* argv[])
{
    int a[] = { 11, 22, 332, 1, 2, 3, 7, 8, 9, 4, 5, 6, 0 };

    lc_shell_sort(a, sizeof(a) / sizeof(a[0]));

    return 0;
}
