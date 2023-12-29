#include <stdio.h>

void lc_bubble_sort(int data_arr[])
{
    register int i = 0;
    register int k = 0;
    int cnt = sizeof(data_arr) / sizeof(data_arr[0]);
    int tmp = 0;

    for (i = 0; i < cnt - 1; i++) {
        if (data_arr[i] < data_arr[i + 1]) {
            tmp = data_arr[i];
            data_arr[i] = data_arr[i + 1];
            data_arr[i + 1] = tmp;
        }
    }
}

int main(int argc, const char* argv[])
{

    return 0;
}
