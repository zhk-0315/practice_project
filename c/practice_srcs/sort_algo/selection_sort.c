#include <stdio.h>

void lc_selection_sort(int a[], size_t asize)
{
    int i = 0;
    int k = 0;
    int cnt = asize / sizeof(a[0]);
    int tmp_i = 0;
    int tmp = 0;

    for (k = 0; k < cnt - 1; k++) {
        tmp_i = k;
        for (i = k; i < cnt; i++) {
            if (a[tmp_i] < a[i]) {
                tmp_i = i;
            }
        }
        if (a[k] != a[tmp_i]) {
            tmp = a[k];
            a[k] = a[tmp_i];
            a[tmp_i] = tmp;
        }
    }

    for (i = 0; i < cnt; i++) {
        printf("%d ", a[i]);
    }

    printf("\n");
}


int main(int argc, const char* argv[])
{
    int a[] = { 11, 22, 332, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

    lc_selection_sort(a, sizeof(a));

    return 0;
}
