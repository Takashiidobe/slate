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
// LOWERING-X86_64-GNU-DAG: fn main() -> std::process::ExitCode {
// LOWERING-X86_64-GNU-DAG:     let mut end: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"0x1.0000000000000002p0\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = unsafe {
// LOWERING-X86_64-GNU-DAG:         __slate_strtold__rf80_pc_ppc(
// LOWERING-X86_64-GNU-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-DAG:             std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// LOWERING-X86_64-GNU-DAG:         )
// LOWERING-X86_64-GNU-DAG:     };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"strtold\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     dump80({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = end;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     return std::process::ExitCode::SUCCESS;
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU-DAG: fn main() -> std::process::ExitCode {
// REWRITES-X86_64-GNU-DAG:     let mut end: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-DAG:     dump80(c"strtold".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-DAG:         __slate_strtold__rf80_pc_ppc(
// REWRITES-X86_64-GNU-DAG:             c"0x1.0000000000000002p0".as_ptr(),
// REWRITES-X86_64-GNU-DAG:             std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// REWRITES-X86_64-GNU-DAG:         )
// REWRITES-X86_64-GNU-DAG:     });
// REWRITES-X86_64-GNU-DAG:     println!("{}", (((unsafe { *end }) as i32) == 0) as i32);
// REWRITES-X86_64-GNU-DAG:     let _ = std::io::Write::flush(&mut std::io::stdout());
// REWRITES-X86_64-GNU-DAG:     return std::process::ExitCode::SUCCESS;
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
