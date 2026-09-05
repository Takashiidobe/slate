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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn read_array_element_through_call() -> i32 {
// LOWERING-DAG:     let mut values: [i32; 1] = [0; 1];
// LOWERING-DAG:     let {{__v[0-9]+}}: [i32; 1] = [10];
// LOWERING-DAG:     values = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { read_pointer(std::ptr::addr_of_mut!(values[({{__v[0-9]+}} as usize)])) };
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: let {{__v[0-9]+}}: [i32; 4] = [2, 4, 6, 8];
// LOWERING-X86_64-GNU-DAG: *values = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG: values = {{__v[0-9]+}};
// LOWERING-DAG: let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG: let {{__v[0-9]+}}: i64 = 3;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d %ld %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d %ld %d\n\0".as_ptr() as *mut u8;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = values[({{__v[0-9]+}} as usize)];
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG: let {{__v[0-9]+}}: i64 = unsafe {
// LOWERING-DAG:     std::ptr::addr_of_mut!(values[({{__v[0-9]+}} as usize)])
// LOWERING-DAG:         .offset_from(std::ptr::addr_of_mut!(values[({{__v[0-9]+}} as usize)])) as i64
// LOWERING-DAG: };
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = read_array_element_through_call();
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn read_array_element_through_call() -> i32 {
// REWRITES-DAG:     let mut values: [i32; 1] = [10];
// REWRITES-DAG:     unsafe { read_pointer(std::ptr::addr_of_mut!(values[0])) }
// REWRITES-DAG: }
// REWRITES-X86_64-GNU-DAG: *values = [2, 4, 6, 8];
// REWRITES-AARCH64-GNU-DAG: values = [2, 4, 6, 8];
// REWRITES-DAG: let {{__v[0-9]+}}: i64 = 1;
// REWRITES-DAG: let {{__v[0-9]+}}: i64 = 3;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     printf(
// REWRITES-DAG:         c"%d %ld %d\n".as_ptr(),
// REWRITES-DAG:         values[({{__v[0-9]+}} as usize)] + values[({{__v[0-9]+}} as usize)],
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             std::ptr::addr_of_mut!(values[({{__v[0-9]+}} as usize)])
// REWRITES-DAG:                 .offset_from(std::ptr::addr_of_mut!(values[({{__v[0-9]+}} as usize)]))
// REWRITES-DAG:                 as i64
// REWRITES-DAG:         },
// REWRITES-DAG:         read_array_element_through_call(),
// REWRITES-DAG:     )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
