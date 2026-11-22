#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int total(const char *bytes, int n) {
  int sum = 0;
  for (int i = 0; i < n; i++)
    sum += bytes[i];
  return sum;
}

static void fill(char *bytes, int n, char value) {
  for (int i = 0; i < n; i++)
    bytes[i] = value;
}

// @check-fn-begin
static int owned(int n) {
  char *buf = malloc(n);
  if (!buf)
    return -1;
  memset(buf, 'a', n);
  buf[0] = 'x';
  buf[n - 1] = 0;
  int result = buf[0] + buf[1] + total(buf, n);
  free(buf);
  return result;
}
// @check-fn-end

// @check-fn-begin
static int grown(void) {
  char *grow = calloc(4, 1);
  if (grow == NULL)
    return -1;
  grow = realloc(grow, 8);
  grow[7] = 3;
  fill(grow, 4, 2);
  int result = grow[0] + grow[3] + grow[5] + grow[7];
  free(grow);
  return result;
}
// @check-fn-end

// @check-fn-begin
static int early(int n, int bail) {
  char *buf = malloc(n);
  if (bail) {
    free(buf);
    return 0;
  }
  memset(buf, 1, n);
  int result = total(buf, n);
  free(buf);
  return result;
}
// @check-fn-end

// @check-fn-begin
static int leaked(int n) {
  char *buf = calloc(n, 1);
  buf[2] = 9;
  return buf[2] + buf[1];
}
// @check-fn-end

static int peek(const char *p) { return p[0]; }

static void release(char *p) { free(p); }

// @check-fn-begin
static int offset(int n) {
  char *buf = malloc(n);
  memset(buf, 4, n);
  int result = peek(buf + 2);
  free(buf);
  return result;
}
// @check-fn-end

// @check-fn-begin
static int looped(int n) {
  int sum = 0;
  for (int i = 0; i < n; i++) {
    char *buf = calloc(4, 1);
    buf[i % 4] = (char)i;
    sum += buf[i % 4];
    free(buf);
  }
  return sum;
}
// @check-fn-end

// @check-fn-begin
static int returned_in_loop(int n) {
  char *buf = malloc(4);
  buf[0] = 5;
  int result = buf[0];
  for (int i = 0; i < n; i++) {
    if (i == 2) {
      free(buf);
      return result + i;
    }
  }
  free(buf);
  return result;
}
// @check-fn-end

// @check-fn-begin
static int freed_by_callee(void) {
  char *buf = malloc(8);
  buf[0] = 1;
  int result = buf[0];
  release(buf);
  return result;
}
// @check-fn-end

// @check-fn-begin
static char *escaped(int n) {
  char *buf = malloc(n);
  memset(buf, 'z', n);
  return buf;
}
// @check-fn-end

int main(void) {
  printf("%d\n", owned(16));
  printf("%d\n", grown());
  printf("%d %d\n", early(8, 0), early(8, 1));
  printf("%d\n", leaked(4));
  printf("%d %d\n", offset(5), looped(6));
  printf("%d %d %d\n", returned_in_loop(4), returned_in_loop(1), freed_by_callee());
  char *kept = escaped(4);
  printf("%d\n", kept[3]);
  free(kept);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn owned(n: i32) -> i32 {
// CHECK-NEXT:     let mut buf: Vec<i8> = vec![0; (n as u64) as usize];
// CHECK-NEXT:     if false {
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     buf[0usize..((n as u64) as usize) as usize].fill((97i32 as i32) as i8);
// CHECK-NEXT:     buf[0] = 120;
// CHECK-NEXT:     buf[(n - 1) as usize] = 0;
// CHECK-NEXT:     let result: i32 = (buf[0] as i32) + (buf[1] as i32) + total(&buf, n);
// CHECK-NEXT:     drop(buf);
// CHECK-NEXT:     result
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn grown() -> i32 {
// CHECK-NEXT:     let mut grow: Vec<i8> = vec![0; (4 as usize) * (1 as usize)];
// CHECK-NEXT:     if false {
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     grow.resize(8 as usize, 0);
// CHECK-NEXT:     grow[7] = 3;
// CHECK-NEXT:     fill(&mut grow, 4i32, 2i8);
// CHECK-NEXT:     let result: i32 = (grow[0] as i32) + (grow[3] as i32) + (grow[5] as i32)
// CHECK-NEXT:         + (grow[7] as i32);
// CHECK-NEXT:     drop(grow);
// CHECK-NEXT:     result
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn early(n: i32, bail: i32) -> i32 {
// CHECK-NEXT:     let mut buf: Vec<i8> = vec![0; (n as u64) as usize];
// CHECK-NEXT:     if bail != 0 {
// CHECK-NEXT:         drop(buf);
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     buf[0usize..((n as u64) as usize) as usize].fill((1i32 as i32) as i8);
// CHECK-NEXT:     let result: i32 = total(&buf, n);
// CHECK-NEXT:     drop(buf);
// CHECK-NEXT:     result
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn leaked(n: i32) -> i32 {
// CHECK-NEXT:     let mut buf: Vec<i8> = vec![0; ((n as u64) as usize) * 1];
// CHECK-NEXT:     buf[2] = 9;
// CHECK-NEXT:     (buf[2] as i32) + (buf[1] as i32)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn offset(n: i32) -> i32 {
// CHECK-NEXT:     let mut buf: Vec<i8> = vec![0; (n as u64) as usize];
// CHECK-NEXT:     buf[0usize..((n as u64) as usize) as usize].fill((4i32 as i32) as i8);
// CHECK-NEXT:     let result: i32 = peek(unsafe { buf.as_mut_ptr().add(2) as *const i8 });
// CHECK-NEXT:     drop(buf);
// CHECK-NEXT:     result
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn looped(n: i32) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let mut buf: Vec<i8> = vec![0; (4 as usize) * (1 as usize)];
// CHECK-NEXT:         buf[(i % 4) as usize] = i as i8;
// CHECK-NEXT:         sum += buf[(i % 4) as usize] as i32;
// CHECK-NEXT:         drop(buf);
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn returned_in_loop(n: i32) -> i32 {
// CHECK-NEXT:     let mut buf: Vec<i8> = vec![0; 4 as usize];
// CHECK-NEXT:     buf[0] = 5;
// CHECK-NEXT:     let result: i32 = buf[0] as i32;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         if i == 2 {
// CHECK-NEXT:             drop(buf);
// CHECK-NEXT:             return result + i;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     drop(buf);
// CHECK-NEXT:     result
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn freed_by_callee() -> i32 {
// CHECK-NEXT:     let buf: *mut i8 = unsafe { (malloc(8u64 as usize) as *mut ()) as *mut i8 };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *buf.add(0) = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let result: i32 = unsafe { *buf.add(0) as i32 };
// CHECK-NEXT:     release(buf);
// CHECK-NEXT:     result
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn escaped(n: i32) -> *mut i8 {
// CHECK-NEXT:     let buf: *mut i8 = unsafe { (malloc((n as u64) as usize) as *mut ()) as *mut i8 };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::write_bytes(
// CHECK-NEXT:             ((buf as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (122i32 as i32) as u8,
// CHECK-NEXT:             ((n as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     buf
// CHECK: {{^}}}
// SLATE-FILECHECK-END
