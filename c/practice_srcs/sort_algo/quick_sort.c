#include <stdio.h>
#include <unistd.h>

typedef struct range_t {
    int start;
    int end;
} range_t;

range_t new_ranger_t(int start, int end)
{
    range_t range = {
        .start = start,
        .end = end
    };

    return range;
}

void swap_int(int* x, int* y)
{
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

void lc_quick_sort(int* a, int cnt)
{
    int i = 0;
    int left = 0;
    int right = 0;
    int* key = NULL;
    int stack_top = 0;
    range_t range[cnt];
    range_t cur_r = { 0 };

    // 使用 range[]模拟ranger_t变量栈,range[stack_top++]入栈,
    // range[--stack_top]出栈并取得该变量的值
    range[stack_top++] = new_ranger_t(0, cnt - 1);
    while (stack_top) { // 栈内非空
        cur_r = range[--stack_top]; // 出栈

        if (cur_r.start >= cur_r.end) {
            continue; // 丢弃
        }

        key = &a[cur_r.start];
        left = cur_r.start + 1;
        right = cur_r.end;

        while (left < right) {
            while (a[left] < *key && left < right) {
                left++;
            }

            while (a[right] >= *key && left < right) {
                right--;
            }

            if (left < right) {
                swap_int(&a[left], &a[right]);
            }
        }

        if (a[left] < *key) {
            swap_int(&a[left], key);
        } else {
            swap_int(&a[--left], key);
        }

        if (cur_r.start < left - 1)
            range[stack_top++] = new_ranger_t(cur_r.start, left - 1);
        if (left + 1 < cur_r.end)
            range[stack_top++] = new_ranger_t(left + 1, cur_r.end);
    }

    for (i = 0; i < cnt; i++) {
        printf("%d ", a[i]);
    }

    printf("\n");
}

int main(int argc, const char* argv[])
{
    int a[] = { 11, 22, 332, 1, 7, 3, 2, 8, 9, 4, 5, 6, 0, 55, 66, 999, 343, 546, 222 };

    lc_quick_sort(a, sizeof(a) / sizeof(a[0]));

    return 0;
}
