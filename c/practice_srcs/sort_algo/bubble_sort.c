#include <stdio.h>

void lc_bubble_sort(int data_arr[], size_t size)
{
    register int i = 0;
    register int k = 0;
    int cnt = size / sizeof(data_arr[0]);
    int tmp = 0;

    for (k = 0; k < cnt - 2; k++) {
        for (i = 0; i < cnt - k - 1; i++) {
            if (data_arr[i] < data_arr[i + 1]) {
                tmp = data_arr[i];
                data_arr[i] = data_arr[i + 1];
                data_arr[i + 1] = tmp;
            }
        }
    }

    for (i = 0; i < cnt; i++) {
        printf("%d ", data_arr[i]);
    }

    printf("\n");
}

int main(int argc, const char* argv[])
{
    int a[] = { 11, 22, 332, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

    lc_bubble_sort(a, sizeof(a));

    return 0;
}
