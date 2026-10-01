#include <stdio.h>

static char greeting[] = "hello";
char padded[8] = "ab";
const char label[] = "label";
static char grid[2][4] = {"abc", "xyz"};
static const char *words[] = {"zero", "one", "two"};
static char high[] = "\x80\xff\x7f";

static int total(const char *s) {
  int sum = 0;
  while (*s) {
    sum += *s;
    s++;
  }
  return sum;
}

int main(void) {
  char local[] = "local";
  greeting[0] = 'j';
  padded[2] = 'c';
  grid[1][0] = 'w';
  local[0] = 'L';
  printf("%s %s %s %s %s %s\n", greeting, padded, label, grid[0], grid[1], local);
  printf("%d %d %d %d %d %d\n", padded[5], total(words[1]), total(words[2]), high[0], high[1], high[2]);
  for (int i = 0; i < 3; i++)
    printf("%s\n", words[i]);
  return total(greeting) % 256;
}
