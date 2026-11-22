#include <stdio.h>
#include <string.h>

// @check-fn-begin
static int build(void) {
  char buf[32];
  strcpy(buf, "hello");
  strcat(buf, ", world");
  printf("%s\n", buf);
  return (int)strlen(buf);
}
// @check-fn-end

// @check-fn-begin
static int initialized(const char *name) {
  char greeting[16] = "hi";
  strcat(greeting, " ");
  if (strcmp(greeting, "hi ") == 0)
    puts(greeting);
  return greeting[0] + (int)strlen(greeting) + (strcmp(greeting, name) > 0);
}
// @check-fn-end

// @check-fn-begin
static int written(void) {
  char buf[8] = "abc";
  buf[1] = 0;
  return (int)strlen(buf);
}
// @check-fn-end

// @check-fn-begin
static int copied(const char *source) {
  char buf[8];
  memcpy(buf, source, 3);
  buf[3] = 0;
  return (int)strlen(buf);
}
// @check-fn-end

int main(void) {
  printf("%d\n", build());
  printf("%d\n", initialized("hi"));
  printf("%d\n", written());
  printf("%d\n", copied("xyz"));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn build() -> i32 {
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i8; 32]> = __SlateAlign16([0i8; 32]);
// CHECK-NEXT:     slate_support::__slate_strcpy(&mut buf.0, "hello");
// CHECK-NEXT:     slate_support::__slate_strcat(&mut buf.0, ", world");
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%s\n".as_ptr(), buf.0.as_mut_ptr()) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     (slate_support::__slate_c_str(&buf.0).count_bytes() as u64) as i32
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn initialized(name: &core::ffi::CStr) -> i32 {
// CHECK-NEXT:     let mut greeting: __SlateAlign16<[i8; 16]> = __SlateAlign16(
// CHECK-NEXT:         [
// CHECK-NEXT:             104i8, 105i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8,
// CHECK-NEXT:             0i8, 0i8,
// CHECK-NEXT:         ],
// CHECK-NEXT:     );
// CHECK-NEXT:     slate_support::__slate_strcat(&mut greeting.0, " ");
// CHECK-NEXT:     if slate_support::__slate_c_str(&greeting.0) == c"hi " {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::puts(greeting.0.as_mut_ptr() as *const i8) };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     (greeting.0[0] as i32)
// CHECK-NEXT:         + ((slate_support::__slate_c_str(&greeting.0).count_bytes() as u64) as i32)
// CHECK-NEXT:         + ((slate_support::__slate_c_str(&greeting.0) > name) as i32)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn written() -> i32 {
// CHECK-NEXT:     let mut buf: [i8; 8] = [97, 98, 99, 0, 0, 0, 0, 0];
// CHECK-NEXT:     buf[1] = 0;
// CHECK-NEXT:     (slate_support::__slate_c_str(&buf).count_bytes() as u64) as i32
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn copied(source: *const i8) -> i32 {
// CHECK-NEXT:     let mut buf: [i8; 8] = [0i8; 8];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((source as *const ()) as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((buf.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (3u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     buf[3] = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (strlen((buf.as_mut_ptr() as *const i8) as *const core::ffi::c_char) as u64)
// CHECK-NEXT:             as i32
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
