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

int main(void) {
  char       *end;
  // @lowering-begin
  // @rewrite-begin
  long double value = strtold("0x1.0000000000000002p0", &end);
  // @rewrite-end
  // @lowering-end
  dump80("strtold", value);
  printf("%d\n", *end == '\0');
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"0x1.0000000000000002p0\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-DAG: __slate_strtold__rf80_pc_ppc(
// LOWERING-DAG: {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-DAG: std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// LOWERING-DAG: )
// LOWERING-DAG: };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: LongDouble = unsafe {
// REWRITES-DAG: __slate_strtold__rf80_pc_ppc(
// REWRITES-DAG: c"0x1.0000000000000002p0".as_ptr(),
// REWRITES-DAG: std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// REWRITES-DAG: )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
