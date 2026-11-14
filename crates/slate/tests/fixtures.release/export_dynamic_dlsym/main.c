// translate-args: --export-dynamic
// link-args: -rdynamic
#include <dlfcn.h>
#include <stdio.h>

int api_sum(const int *values, int count);
int api_scale(int value);

int main(void) {
  int values[] = {3, 5, 7};
  printf("%d %d\n", api_sum(values, 3), api_scale(4));
  int (*sum)(const int *, int) = (int (*)(const int *, int))dlsym(RTLD_DEFAULT, "api_sum");
  int (*scale)(int) = (int (*)(int))dlsym(RTLD_DEFAULT, "api_scale");
  if (!sum || !scale) {
    puts("missing export");
    return 1;
  }
  printf("%d %d\n", sum(values, 3), scale(5));
  return 0;
}
