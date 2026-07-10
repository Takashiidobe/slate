#include <stdio.h>

static int sum_array(void) {
    int values[3];
    values[0] = 4;
    values[1] = 5;
    values[2] = values[0] + values[1];
    return values[2];
}

static int pick_index(int index) {
    int values[3];
    values[0] = 10;
    values[1] = 20;
    values[2] = 30;
    return values[index];
}

int main(void) {
    printf("%d\n", sum_array());
    printf("%d\n", pick_index(1));
    return 0;
}
