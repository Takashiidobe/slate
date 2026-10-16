#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void dump80(const char *name, long double value) {
  unsigned char bytes[10];
  memcpy(bytes, &value, 10);
  printf("%s", name);
  for (int i = 0; i < 10; ++i)
    printf("%02x", bytes[i]);
  printf("\n");
}

// @check-fn-begin
int main(void) {
  char       *end;
  long double value = strtold("0x1.0000000000000002p0", &end);
  dump80("strtold", value);
  printf("%d\n", *end == '\0');
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut end: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let value: LongDouble = unsafe {
// CHECK-NEXT:         __slate_strtold__rf80_pi8_ppi8(
// CHECK-NEXT:             c"0x1.0000000000000002p0".as_ptr() as *const i8,
// CHECK-NEXT:             std::ptr::addr_of_mut!(end),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     dump80(c"strtold".as_ptr() as *const i8, value);
// CHECK-NEXT:     println!("{}", (((unsafe { * end }) as i32) == (0 as i32)) as i32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
