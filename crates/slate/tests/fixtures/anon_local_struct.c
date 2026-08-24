#include <stdio.h>

typedef struct {
  int *start;
  int *end;
  int *pointer;
} buffer_t;

int main(void) {
  struct {
    int x;
    int y;
  } point = {3, 4};

  int      storage[4];
  buffer_t buf = {0, 0, 0};
  buf.start    = storage;
  buf.pointer  = storage;
  buf.end      = storage + 4;

  *buf.pointer = point.x + point.y;
  buf.pointer++;
  *buf.pointer = point.x * point.y;
  buf.pointer++;

  printf("%d %d\n", storage[0], storage[1]);
  printf("%ld\n", (long)(buf.pointer - buf.start));
  printf("%ld\n", (long)(buf.end - buf.start));
  return 0;
}
// REWRITES-DAG: struct __slate_anonymous_struct_0(i32, i32);
// REWRITES-DAG: let point: __slate_anonymous_struct_0 = __slate_anonymous_struct_0(3, 4);
// REWRITES-DAG: point.0
// REWRITES-DAG: point.1
// REWRITES-DAG: storage[0] =
// REWRITES-DAG: storage[1] =
// REWRITES-DAG: println!("{}", 2 as i64);
// REWRITES-DAG: println!("{}", 4 as i64);
// REWRITES-NOT: let buf:
// REWRITES-NOT: buf.pointer
// REWRITES-NOT: buf.start
// REWRITES-NOT: buf.end
// REWRITES-NOT: offset_from
// REWRITES-NOT: struct anon_0
// REWRITES-NOT: point.x
// REWRITES-NOT: point.y
