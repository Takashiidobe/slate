#include <stdio.h>

// @check-fn-begin
int main(void) {
  remove("slate_stdio_file_write.tmp");
  FILE *f = fopen("slate_stdio_file_write.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("owned\n", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_file_write.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char buf[16] = {0};
  fgets(buf, sizeof buf, g);
  fclose(g);
  fputs(buf, stdout);
  remove("slate_stdio_file_write.tmp");
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { remove(c"slate_stdio_file_write.tmp".as_ptr() as *const i8) };
// CHECK-NEXT:     let mut f: *mut _IO_FILE = unsafe {
// CHECK-NEXT:         fopen(
// CHECK-NEXT:             c"slate_stdio_file_write.tmp".as_ptr() as *const i8,
// CHECK-NEXT:             c"w".as_ptr() as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if !(f != std::ptr::null_mut::<_IO_FILE>()) {
// CHECK-NEXT:         unsafe { puts(c"open-fail".as_ptr() as *const i8) };
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fputs(c"owned\n".as_ptr() as *const i8, f) };
// CHECK-NEXT:     unsafe { fclose(f) };
// CHECK-NEXT:     let mut g: *mut _IO_FILE = unsafe {
// CHECK-NEXT:         fopen(
// CHECK-NEXT:             c"slate_stdio_file_write.tmp".as_ptr() as *const i8,
// CHECK-NEXT:             c"r".as_ptr() as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if !(g != std::ptr::null_mut::<_IO_FILE>()) {
// CHECK-NEXT:         unsafe { puts(c"reopen-fail".as_ptr() as *const i8) };
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i8; 16]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = (0 as i32) as i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe { fgets(buf.0.as_mut_ptr() as *mut i8, ((16 as u64) as u32) as i32, g) };
// CHECK-NEXT:     unsafe { fclose(g) };
// CHECK-NEXT:     unsafe { fputs(buf.0.as_mut_ptr() as *const i8, unsafe { stdout }) };
// CHECK-NEXT:     unsafe { remove(c"slate_stdio_file_write.tmp".as_ptr() as *const i8) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
