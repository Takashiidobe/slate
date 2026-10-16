#include <stdio.h>

int main(void) {
  fputs("bye\n", stderr);
  return 0;
}

// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK: fputs(c"bye\n".as_ptr() as *const i8, unsafe { stderr })
// CHECK: {{^}}}
// X86_64-UNKNOWN-LINUX-GNU: {{^}}struct _IO_FILE {
