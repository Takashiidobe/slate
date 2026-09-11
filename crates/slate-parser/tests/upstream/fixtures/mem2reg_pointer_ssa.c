#include <stdio.h>

int  value         = 7;
int *value_pointer = &value;
int  values[]      = {11, 13, 17};

int *return_global(void) { return &value; }

int **return_global_pointer(void) { return &value_pointer; }

int *return_element(void) { return &values[1]; }

int main(void) {
  printf("%d\n",
         *return_global() + **return_global_pointer() + *return_element());
  return 0;
}


