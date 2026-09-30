#include <stdio.h>

#define STEP_1(value) ((value) + 1)
#define STEP_2(value) STEP_1(STEP_1(value))
#define STEP_3(value) STEP_2(STEP_2(value))
#define STEP_4(value) STEP_3(STEP_3(value))
#define STEP_5(value) STEP_4(STEP_4(value))
#define STEP_6(value) STEP_5(STEP_5(value))
#define STEP_7(value) STEP_6(STEP_6(value))
#define STEP_8(value) STEP_7(STEP_7(value))

int main(void) {
  printf("%d\n", STEP_8(0));
  return 0;
}
