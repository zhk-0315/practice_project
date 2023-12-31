#include <stdio.h>

typedef struct _Range {
    int start, end;
} Range;
Range new_Range(int s, int e)
{
    Range r;
    r.start = s;
    r.end = e;
    return r;
}
void swap(int* x, int* y)
{
    int t = *x;
    *x = *y;
    *y = t;
}
void quick_sort(int arr[], const int len)
{
    if (len <= 0)
        return; // 避免len等於負值時引發段錯誤（Segment Fault）
    // r[]模擬列表,p為數量,r[p++]為push,r[--p]為pop且取得元素
    Range r[len];
    int i = 0;
    int p = 0;
    r[p++] = new_Range(0, len - 1);
    printf("save p=%d\n", p - 1);
    while (p) {
        printf("=======================\n", p);
        Range range = r[--p];
        printf("use p=%d\n", p);
        if (range.start >= range.end)
            continue;
        int mid = arr[(range.start + range.end) / 2]; // 選取中間點為基準點
        int left = range.start, right = range.end;
        do {
            while (arr[left] < mid)
                ++left; // 檢測基準點左側是否符合要求
            while (arr[right] > mid)
                --right; // 檢測基準點右側是否符合要求

            if (left <= right) {
                swap(&arr[left], &arr[right]);
                left++;
                right--; // 移動指針以繼續
            }
        } while (left <= right);

        if (range.start < right) {
            r[p++] = new_Range(range.start, right);
            printf("save p=%d\n", p - 1);
        }
        if (range.end > left) {
            r[p++] = new_Range(left, range.end);
            printf("save p=%d\n", p - 1);
        }
    }

    // for (i = 0; i < len; i++) {
    //     printf("%d ", arr[i]);
    // }

    // printf("\n");
}

int main(int argc, const char* argv[])
{
    int a[] = { 11, 22, 332, 1, 2, 3, 7, 8, 9, 4, 5, 6, 0 };

    quick_sort(a, sizeof(a) / sizeof(a[0]));

    return 0;
}
