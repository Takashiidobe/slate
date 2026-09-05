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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(preserved)) };
// LOWERING-DAG: let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-DAG: let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_u128() as i32;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     printf(
// REWRITES-DAG:         c"%d %d %d %d %d\n".as_ptr(),
// REWRITES-DAG:         {{__v[0-9]+}},
// REWRITES-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(preserved)) },
// REWRITES-DAG:         ({{__v[0-9]+}} == std::ptr::null_mut()) as i32,
// REWRITES-DAG:         {{__v[0-9]+}}.to_u128() as i32,
// REWRITES-DAG:         (1 as i32) + (1 as i32),
// REWRITES-DAG:     )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
