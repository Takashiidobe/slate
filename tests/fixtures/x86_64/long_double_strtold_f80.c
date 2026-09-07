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

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  char       *end;
  long double value = strtold("0x1.0000000000000002p0", &end);
  dump80("strtold", value);
  printf("%d\n", *end == '\0');
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-X86_64-GNU-DAG:     let mut end: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-DAG:     let mut end: *mut u8 = std::ptr::null_mut();
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"0x1.0000000000000002p0\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = unsafe {
// LOWERING-X86_64-GNU-DAG:         __slate_strtold__rf80_pc_ppc(
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"0x1.0000000000000002p0\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = unsafe {
// LOWERING-AARCH64-GNU-DAG:         strtold(
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-DAG:             std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"strtold\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"strtold\0".as_ptr() as *mut u8;
// LOWERING-DAG:     dump80({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = end;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = end;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-X86_64-GNU-DAG:     let mut end: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-DAG:     dump80(c"strtold".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-DAG:         __slate_strtold__rf80_pc_ppc(
// REWRITES-AARCH64-GNU-DAG:     let mut end: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-DAG:     dump80(c"strtold".as_ptr() as *mut u8, unsafe {
// REWRITES-AARCH64-GNU-DAG:         strtold(
// REWRITES-DAG:             c"0x1.0000000000000002p0".as_ptr(),
// REWRITES-DAG:             std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// REWRITES-DAG:         )
// REWRITES-DAG:     });
// REWRITES-DAG:     unsafe { printf(c"%d\n".as_ptr(), (((unsafe { *end }) as i32) == 0) as i32) };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
