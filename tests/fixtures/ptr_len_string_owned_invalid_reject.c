#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// @lowering-fn-begin
// @rewrite-fn-begin
static int consume_bin(char *buf, int len) {
  (void)strlen(buf);
  int score = 0;
  for (int i = 0; i < len; ++i)
    score += (unsigned char)buf[i];
  free(buf);
  return score;
}
// @rewrite-fn-end
// @lowering-fn-end

static int forward_bin(char *buf, int len) { return consume_bin(buf, len); }

int main(void) {
  int   len = 2;
  char *buf = malloc(len * sizeof(char));
  memcpy(buf, "\xff", len);
  int score = forward_bin(buf, len);
  printf("%d\n", score);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn consume_bin({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut buf: *mut i8 = std::ptr::null_mut();
// LOWERING-DAG:     let mut len: i32 = 0;
// LOWERING-DAG:     let mut score: i32 = 0;
// LOWERING-DAG:     buf = {{arg[0-9]+}};
// LOWERING-DAG:     len = {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i8 = buf;
// LOWERING-DAG:     let {{_v[0-9]+}}: u64 = (unsafe { strlen({{_v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     score = {{_v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let mut i: i32 = 0;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{_v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = len;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:             if !{{_v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-DAG:             let {{_v[0-9]+}}: *mut i8 = buf;
// LOWERING-DAG:             let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-DAG:             let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-DAG:             let {{_v[0-9]+}}: u8 = {{_v[0-9]+}} as u8;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = score;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:             score = {{_v[0-9]+}};
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{_v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i8 = buf;
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = score;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn consume_bin(mut buf: *mut i8, mut len: i32) -> i32 {
// REWRITES-DAG:     let mut score: i32 = 0;
// REWRITES-DAG:     (unsafe { strlen(buf as *const core::ffi::c_char) }) as u64;
// REWRITES-DAG:     for i in 0..len {
// REWRITES-DAG:         let {{_v[0-9]+}}: *mut i8 = buf;
// REWRITES-DAG:         let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-DAG:         score += ((unsafe { *{{_v[0-9]+}} }) as u8) as i32;
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { free(buf as *mut core::ffi::c_void) };
// REWRITES-DAG:     score
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
