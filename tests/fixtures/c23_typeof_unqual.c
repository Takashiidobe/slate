// { dg-options "-std=gnu23" }
#include <stdio.h>

int main(void) {
  const volatile int qualified   = 9;
  typeof_unqual(qualified) copy  = qualified;
  typeof(qualified) preserved    = 12;
  int              *pointer      = nullptr;
  constexpr int     width        = 7;
  unsigned _BitInt(width) narrow = 100;
  int unqualified = __builtin_types_compatible_p(typeof(copy), int);
  int still_qualified =
      __builtin_types_compatible_p(typeof(preserved), const volatile int);
  // @lowering-begin
  // @rewrite-begin
  printf("%d %d %d %d %d\n", copy, preserved, pointer == nullptr, (int)narrow,
         unqualified + still_qualified);
  // @rewrite-end
  // @lowering-end
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(preserved)) };
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_u128() as i32;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:     printf(
// COMMON-LOWERING-DAG:         {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:     )
// COMMON-LOWERING-DAG: };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     printf(
// COMMON-REWRITES-DAG:         c"%d %d %d %d %d\n".as_ptr(),
// COMMON-REWRITES-DAG:         {{__v[0-9]+}},
// COMMON-REWRITES-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(preserved)) },
// COMMON-REWRITES-DAG:         ({{__v[0-9]+}} == std::ptr::null_mut()) as i32,
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}.to_u128() as i32,
// COMMON-REWRITES-DAG:         (1 as i32) + (1 as i32),
// COMMON-REWRITES-DAG:     )
// COMMON-REWRITES-DAG: };
// SLATE-FILECHECK-END common-rewrites
