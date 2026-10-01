#include <stdio.h>

struct Sample {
  int     id;
  double  scale;
  int    *target;
  int     data[4];
};

struct Cell {
  char tag;
  long value;
};

static int checksum(struct Sample s) {
  int total = s.id + (int)(s.scale * 10.0);
  if (s.id != 0) {
    total += *s.target;
  }
  for (int i = 0; i < 4; i++) {
    total = total * 3 + s.data[i];
  }
  return total;
}

int main(void) {
  int partial[5] = {1, 2};
  int sum        = 0;
  for (int i = 0; i < 5; i++) {
    sum = sum * 7 + partial[i];
  }
  printf("%d\n", sum);

  int           seed = 9;
  struct Sample zero = {0};
  struct Sample some = {.id = 3, .target = &seed, .data = {[2] = 5}};
  struct Sample full = {4, 1.5, &seed, {1, 2, 3, 4}};
  printf("%d %d %d\n", checksum(zero), checksum(some), checksum(full));

  struct Cell cells[4] = {[1] = {.value = 7}, [3] = {'z'}};
  for (int i = 0; i < 4; i++) {
    printf("%d %ld\n", cells[i].tag, cells[i].value);
  }
  return 0;
}
