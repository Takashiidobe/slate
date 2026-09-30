#include <stdio.h>

void walk(int *position) {
  static void *labels[] = {&&again, &&done};
  goto        *labels[*position];
again:
  position++;
  goto *labels[*position];
done:
  return;
}

int main(void) {
  int path[] = {0, 1};
  walk(path);
  printf("done\n");
  return 0;
}
