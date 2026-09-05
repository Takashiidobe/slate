#include <stdio.h>

struct First {
  int value;
};

struct Second {
  int value;
};

typedef union {
  struct First  *first;
  struct Second *second;
} PointerArgument __attribute__((transparent_union));

__attribute__((noinline)) static int read_value(PointerArgument argument) {
  return argument.first->value;
}

int main(void) {
  struct First  first  = {.value = 17};
  struct Second second = {.value = 29};
  // @lowering-begin
  // @rewrite-begin
  printf("%d %d\n", read_value(&first), read_value(&second));
  // @rewrite-end
  // @lowering-end
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: unsafe {
// COMMON-LOWERING-DAG:     agg_tmp0.first = std::ptr::addr_of_mut!(first);
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: PointerArgument = agg_tmp0;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = read_value({{__v[0-9]+}});
// COMMON-LOWERING-DAG: unsafe {
// COMMON-LOWERING-DAG:     agg_tmp1.second = std::ptr::addr_of_mut!(second);
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: PointerArgument = agg_tmp1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = read_value({{__v[0-9]+}});
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     agg_tmp0.first = std::ptr::addr_of_mut!(first);
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = read_value(agg_tmp0);
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     agg_tmp1.second = std::ptr::addr_of_mut!(second);
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, read_value(agg_tmp1)) };
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = c"%d %d\n".as_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = c"%d %d\n".as_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
