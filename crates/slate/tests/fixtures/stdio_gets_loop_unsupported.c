#include <stdio.h>
#include <string.h>

// @check-fn-begin
int main(void) {
  remove("slate_stdio_gets_loop_unsupported.tmp");
  FILE *f = fopen("slate_stdio_gets_loop_unsupported.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("one\n", f);
  fputs("two\n", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_gets_loop_unsupported.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char line[64];
  int  count = 0;
  while (fgets(line, sizeof line, g) != NULL) {
    count += (int)strlen(line);
  }
  fclose(g);
  printf("%d\n", count);
  remove("slate_stdio_gets_loop_unsupported.tmp");
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { remove(c"slate_stdio_gets_loop_unsupported.tmp".as_ptr() as *const i8) };
// CHECK-NEXT:     let mut f: *mut _IO_FILE = unsafe {
// CHECK-NEXT:         fopen(
// CHECK-NEXT:             c"slate_stdio_gets_loop_unsupported.tmp".as_ptr() as *const i8,
// CHECK-NEXT:             c"w".as_ptr() as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if !(f != std::ptr::null_mut::<_IO_FILE>()) {
// CHECK-NEXT:         unsafe { puts(c"open-fail".as_ptr() as *const i8) };
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fputs(c"one\n".as_ptr() as *const i8, f) };
// CHECK-NEXT:     unsafe { fputs(c"two\n".as_ptr() as *const i8, f) };
// CHECK-NEXT:     unsafe { fclose(f) };
// CHECK-NEXT:     let mut g: *mut _IO_FILE = unsafe {
// CHECK-NEXT:         fopen(
// CHECK-NEXT:             c"slate_stdio_gets_loop_unsupported.tmp".as_ptr() as *const i8,
// CHECK-NEXT:             c"r".as_ptr() as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if !(g != std::ptr::null_mut::<_IO_FILE>()) {
// CHECK-NEXT:         unsafe { puts(c"reopen-fail".as_ptr() as *const i8) };
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut line: __SlateAlign16<[i8; 64]> = __SlateAlign16([0 as i8; 64]);
// CHECK-NEXT:     let mut count: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe {
// CHECK-NEXT:         fgets(line.0.as_mut_ptr() as *mut i8, ((64 as u64) as u32) as i32, g)
// CHECK-NEXT:     }) != std::ptr::null_mut::<i8>()
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = count;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]]
// CHECK-NEXT:             + (((unsafe { strlen(line.0.as_mut_ptr() as *const i8) }) as u32) as i32);
// CHECK-NEXT:         count = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fclose(g) };
// CHECK-NEXT:     unsafe { printf(c"%d\n".as_ptr() as *const i8, count) };
// CHECK-NEXT:     unsafe { remove(c"slate_stdio_gets_loop_unsupported.tmp".as_ptr() as *const i8) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
