#include <stdio.h>

int main(void) {
  const volatile int qualified = 9;
  typeof_unqual(qualified) copy = qualified;
  typeof(qualified) preserved = 12;
  int *pointer = nullptr;
  constexpr int width = 7;
  unsigned _BitInt(width) narrow = 100;
  int unqualified = __builtin_types_compatible_p(typeof(copy), int);
  int still_qualified =
      __builtin_types_compatible_p(typeof(preserved), const volatile int);
  // @lowering-begin
  // @rewrite-begin
  printf("%d %d %d %d %d\n", copy, preserved, pointer == nullptr,
         (int)narrow, unqualified + still_qualified);
  // @rewrite-end
  // @lowering-end
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = copy;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(preserved)) };
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i32 = pointer;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-DAG: let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: bitint::BUint<7, 1, 1> = narrow;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.to_u128() as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unqualified;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = still_qualified;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = copy;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(preserved)) };
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = (pointer == {{_v[0-9]+}}) as i32;
// REWRITES-DAG: let {{_v[0-9]+}}: bitint::BUint<7, 1, 1> = narrow;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.to_u128() as i32;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, unqualified + still_qualified) };
// SLATE-FILECHECK-END rewrites
