#include <stdio.h>
#include <stdlib.h>

int* two_sum(const int* nums, int numsSize, int target, int* returnSize) {
    *returnSize = 2;    

    int* result = (int*)malloc(sizeof(int) * 2);
    result[0] = -1;
    result[1] = -1;

    int keys[10000];
    int values[10000];
    int last_index = 0;

    for (int i = 0; i < numsSize; ++i) {
        int n = target - nums[i];

        int is_pair_found = 0;
        for (int j = 0; j < last_index; ++j) {
            if (n == keys[j]) {
                result[0] = values[j];
                result[1] = i;
                return result;
            }
        }

        if (!is_pair_found) {
            keys[last_index] = nums[i];
            values[last_index] = i;
            ++last_index;
        }
    }

    return result;
}

int main(void) {
    int returnSize;

    int nums_1[4] = {2, 3, 5, 10};
    int target_1 = 15;
    int *result_1 = two_sum(nums_1, 4, target_1, &returnSize);
    for (int i = 0; i < 2; ++i)
        printf("%d\n", result_1[i]);

    int nums_2[4] = {2, 7, 11, 15};
    int target_2 = 9;
    int *result_2 = two_sum(nums_2, 4, target_2, &returnSize);
    for (int i = 0; i < 2; ++i)
        printf("%d\n", result_2[i]);
}