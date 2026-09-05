#include <stdio.h>

int read_pointer(int *pointer) { return *pointer; }

// @rewrite-fn-begin
// @lowering-fn-begin
int read_array_element_through_call(void) {
  int  values[1] = {10};
  int *pointer   = &values[0];
  return read_pointer(pointer);
}
// @lowering-fn-end
// @rewrite-fn-end

int main(void) {
  // @lowering-begin
  // @rewrite-begin
  int  values[4] = {2, 4, 6, 8};
  int *p         = &values[1];
  int *q         = &values[3];
  printf("%d %ld %d\n", *p + *q, q - p, read_array_element_through_call());
  // @rewrite-end
  // @lowering-end
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn read_array_element_through_call() -> i32 {
// COMMON-LOWERING-DAG:     let mut values: [i32; 1] = [0; 1];
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: [i32; 1] = [10];
// COMMON-LOWERING-DAG:     values = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { read_pointer(std::ptr::addr_of_mut!(values[({{__v[0-9]+}} as usize)])) };
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: [i32; 4] = [2, 4, 6, 8];
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-LOWERING-DAG:     std::ptr::addr_of_mut!(values[({{__v[0-9]+}} as usize)])
// COMMON-LOWERING-DAG:         .offset_from(std::ptr::addr_of_mut!(values[({{__v[0-9]+}} as usize)])) as i64
// COMMON-LOWERING-DAG: };
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = read_array_element_through_call();
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: *values = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d %ld %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: values = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d %ld %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn read_array_element_through_call() -> i32 {
// COMMON-REWRITES-DAG:     let mut values: [i32; 1] = [10];
// COMMON-REWRITES-DAG:     unsafe { read_pointer(std::ptr::addr_of_mut!(values[0])) }
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i64 = 1;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i64 = 3;
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     printf(
// COMMON-REWRITES-DAG:         c"%d %ld %d\n".as_ptr(),
// COMMON-REWRITES-DAG:         values[({{__v[0-9]+}} as usize)] + values[({{__v[0-9]+}} as usize)],
// COMMON-REWRITES-DAG:         unsafe {
// COMMON-REWRITES-DAG:             std::ptr::addr_of_mut!(values[({{__v[0-9]+}} as usize)])
// COMMON-REWRITES-DAG:                 .offset_from(std::ptr::addr_of_mut!(values[({{__v[0-9]+}} as usize)]))
// COMMON-REWRITES-DAG:                 as i64
// COMMON-REWRITES-DAG:         },
// COMMON-REWRITES-DAG:         read_array_element_through_call(),
// COMMON-REWRITES-DAG:     )
// COMMON-REWRITES-DAG: };
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: *values = [2, 4, 6, 8];
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: values = [2, 4, 6, 8];
// SLATE-FILECHECK-END rewrites-aarch64-gnu
