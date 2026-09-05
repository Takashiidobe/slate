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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG:     let mut len: i32 = 0;
// COMMON-LOWERING-DAG:     let mut score: i32 = 0;
// COMMON-LOWERING-DAG:     buf = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     len = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = (unsafe { strlen({{__v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     score = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let mut i: i32 = 0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:         i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         loop {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = len;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                 break;
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = score;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             score = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG:             i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = score;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: fn consume_bin({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-X86_64-GNU-DAG:     let mut buf: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = buf;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = buf;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = buf;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: fn consume_bin({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-AARCH64-GNU-DAG:     let mut buf: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = buf;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = buf;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = buf;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG:     let mut score: i32 = 0;
// COMMON-REWRITES-DAG:     (unsafe { strlen(buf as *const core::ffi::c_char) }) as u64;
// COMMON-REWRITES-DAG:     for i in 0..len {
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     unsafe { free(buf as *mut core::ffi::c_void) };
// COMMON-REWRITES-DAG:     score
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: fn consume_bin(mut buf: *mut i8, mut len: i32) -> i32 {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = buf;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-X86_64-GNU-DAG:         score += ((unsafe { *{{__v[0-9]+}} }) as u8) as i32;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: fn consume_bin(mut buf: *mut u8, mut len: i32) -> i32 {
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = buf;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-AARCH64-GNU-DAG:         score += (unsafe { *{{__v[0-9]+}} }) as i32;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
