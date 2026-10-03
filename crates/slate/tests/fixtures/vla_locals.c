#include <stdio.h>
#include <string.h>

typedef struct {
  int  kind;
  long payload;
} Object;

static long wrap_objects(int argc) {
  Object  objects[argc];
  Object *argv[argc];
  for (int j = 0; j < argc; j++) {
    objects[j].kind    = j;
    objects[j].payload = (long)j * 100;
    argv[j]            = objects + j;
  }
  long total = 0;
  for (int j = 0; j < argc; j++) {
    total += argv[j]->kind + argv[j]->payload;
  }
  return total + (long)sizeof(objects) + (long)sizeof(argv);
}

static int format_port(int port) {
  int  length = snprintf(NULL, 0, "%d", port);
  char buf[length + 1];
  snprintf(buf, sizeof(buf), "%d", port);
  return (int)strlen(buf) * 1000 + buf[0];
}

static int const_bound(void) {
  const int bufflen = 16;
  char      buf[bufflen];
  memset(buf, 'x', sizeof(buf) - 1);
  buf[bufflen - 1] = '\0';
  return (int)strlen(buf);
}

static int matrix(int rows, int cols) {
  int grid[rows][cols];
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      grid[r][c] = r * 10 + c;
    }
  }
  int (*row)[cols] = grid + 1;
  int sum          = 0;
  for (int c = 0; c < cols; c++) {
    sum += row[0][c] + row[1][c];
  }
  long distance = (grid + rows - 1) - grid;
  return sum * 100 + (int)distance * 10 + (int)(sizeof(grid) / sizeof(grid[0]));
}

static int per_iteration(int rounds) {
  int total = 0;
  for (int i = 1; i <= rounds; i++) {
    int scratch[i];
    for (int k = 0; k < i; k++) {
      scratch[k] = k + i;
    }
    total += scratch[i - 1];
  }
  return total;
}

static int sum_row(int n, int (*values)[n]) {
  int total = 0;
  for (int i = 0; i < n; i++) {
    total += (*values)[i];
  }
  return total;
}

int main(void) {
  int length = 5;
  int values[length];
  for (int i = 0; i < length; i++) {
    values[i] = i * i;
  }
  printf("%ld\n", wrap_objects(4));
  printf("%d\n", format_port(6379));
  printf("%d\n", const_bound());
  printf("%d\n", matrix(3, 4));
  printf("%d\n", per_iteration(6));
  printf("%d\n", sum_row(length, &values));
  return 0;
}
