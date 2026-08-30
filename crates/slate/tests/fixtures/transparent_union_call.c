#include <stdio.h>

struct First {
  int value;
};

struct Second {
  int value;
};

typedef union {
  struct First *first;
  struct Second *second;
} PointerArgument __attribute__((transparent_union));

__attribute__((noinline)) static int read_value(PointerArgument argument) {
  return argument.first->value;
}

int main(void) {
  struct First first = {.value = 17};
  struct Second second = {.value = 29};
  // @lowering-begin
  // @rewrite-begin
  printf("%d %d\n", read_value(&first), read_value(&second));
  // @rewrite-end
  // @lowering-end
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: unsafe {
// LOWERING-DAG: agg_tmp0.first = std::ptr::addr_of_mut!(first);
// LOWERING-DAG: }
// LOWERING-DAG: let {{_v[0-9]+}}: PointerArgument = agg_tmp0;
// LOWERING-DAG: coerce2 = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut *mut core::ffi::c_void = std::ptr::addr_of_mut!(coerce2) as *mut *mut core::ffi::c_void;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { *{{_v[0-9]+}} };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = read_value({{_v[0-9]+}} as *mut core::ffi::c_void);
// LOWERING-DAG: unsafe {
// LOWERING-DAG: agg_tmp1.second = std::ptr::addr_of_mut!(second);
// LOWERING-DAG: }
// LOWERING-DAG: let {{_v[0-9]+}}: PointerArgument = agg_tmp1;
// LOWERING-DAG: coerce = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut *mut core::ffi::c_void = std::ptr::addr_of_mut!(coerce) as *mut *mut core::ffi::c_void;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { *{{_v[0-9]+}} };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = read_value({{_v[0-9]+}} as *mut core::ffi::c_void);
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: unsafe {
// REWRITES-DAG: agg_tmp0.first = std::ptr::addr_of_mut!(first);
// REWRITES-DAG: }
// REWRITES-DAG: coerce2 = agg_tmp0;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut *mut core::ffi::c_void = std::ptr::addr_of_mut!(coerce2) as *mut *mut core::ffi::c_void;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = read_value((unsafe { *{{_v[0-9]+}} }) as *mut core::ffi::c_void);
// REWRITES-DAG: unsafe {
// REWRITES-DAG: agg_tmp1.second = std::ptr::addr_of_mut!(second);
// REWRITES-DAG: }
// REWRITES-DAG: coerce = agg_tmp1;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut *mut core::ffi::c_void = std::ptr::addr_of_mut!(coerce) as *mut *mut core::ffi::c_void;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = read_value((unsafe { *{{_v[0-9]+}} }) as *mut core::ffi::c_void);
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END rewrites
