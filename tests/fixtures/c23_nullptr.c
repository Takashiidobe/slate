// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: let mut null_value: *mut core::ffi::c_void
// REWRITES-DAG: std::ptr::null_mut() as *mut core::ffi::c_void
// REWRITES-NOT: std::ptr::null_mut() == std::ptr::null_mut()
// REWRITES: {{^}}}

#include <stddef.h>
#include <stdio.h>

int main(void) {
  volatile nullptr_t null_value   = nullptr;
  int               *null_pointer = nullptr;

  printf("%d %d\n", null_value == nullptr, null_pointer == nullptr);
  return 0;
}
