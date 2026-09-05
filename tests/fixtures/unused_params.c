#include <stdio.h>

static int add(int a, int unused) { return a + 1; }

static int get_used(int a, int b) { return a + b; }

static int remove_two(int a, int unused_a, int unused_b) { return a + 2; }

static int address_taken(int a, int unused) { return a + 4; }

int main(void) {
  // @lowering-begin
  // @rewrite-begin
  int (*indirect)(int, int) = address_taken;
  printf("%d %d %d %d\n", add(5, 10), get_used(1, 2), remove_two(3, 4, 5),
         indirect(8, 9));
  // @rewrite-end
  // @lowering-end
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 5;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 10;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = add({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = get_used({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 4;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 5;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = remove_two({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 8;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 9;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { Some(address_taken).unwrap()({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     printf(
// REWRITES-DAG:         c"%d %d %d %d\n".as_ptr(),
// REWRITES-DAG:         add(5, 10),
// REWRITES-DAG:         get_used(1, 2),
// REWRITES-DAG:         remove_two(3, 4, 5),
// REWRITES-DAG:         unsafe { Some(address_taken).unwrap()(8 as i32, 9 as i32) },
// REWRITES-DAG:     )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
