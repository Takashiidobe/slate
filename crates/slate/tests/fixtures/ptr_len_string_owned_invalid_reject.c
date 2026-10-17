#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// @check-fn-begin
static int consume_bin(char *buf, int len) {
  (void)strlen(buf);
  int score = 0;
  for (int i = 0; i < len; ++i)
    score += (unsigned char)buf[i];
  free(buf);
  return score;
}
// @check-fn-end

static int forward_bin(char *buf, int len) { return consume_bin(buf, len); }

int main(void) {
  int   len = 2;
  char *buf = malloc(len * sizeof(char));
  memcpy(buf, "\xff", len);
  int score = forward_bin(buf, len);
  printf("%d\n", score);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn consume_bin(buf: *mut i8, len: i32) -> i32 {
// CHECK-NEXT:     unsafe { strlen(buf as *const i8) };
// CHECK-NEXT:     let mut score: i32 = 0;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         score += (((unsafe { *unsafe { buf.offset(i as isize) } }) as u8) as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { free(buf as *mut ()) };
// CHECK-NEXT:     score
// CHECK: {{^}}}
// SLATE-FILECHECK-END
