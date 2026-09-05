#include <stdio.h>
#include <string.h>

static int get_count(void) { return 4; }

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  unsigned char forward_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memmove(forward_buf + 1, forward_buf, 5);

  unsigned char backward_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memmove(backward_buf, backward_buf + 1, 5);

  unsigned char dyn_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  int           n          = get_count();
  memmove(dyn_buf, dyn_buf + 1, n);

  for (int i = 0; i < 8; i++)
    printf("%d ", forward_buf[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", backward_buf[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dyn_buf[i]);
  printf("\n");
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut forward_buf: [u8; 8] = [0; 8];
// LOWERING-DAG:     let mut backward_buf: [u8; 8] = [0; 8];
// LOWERING-DAG:     let mut dyn_buf: [u8; 8] = [0; 8];
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// LOWERING-DAG:     forward_buf = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = forward_buf.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = forward_buf.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 5;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-DAG:         memmove(
// LOWERING-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as usize,
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// LOWERING-DAG:     backward_buf = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = backward_buf.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = backward_buf.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 5;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-DAG:         memmove(
// LOWERING-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as usize,
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// LOWERING-DAG:     dyn_buf = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = get_count();
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_buf.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_buf.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-DAG:         memmove(
// LOWERING-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as usize,
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-DAG:     {
// LOWERING-DAG:         let mut i: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 8;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = b"%d \0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = b"%d \0".as_ptr() as *mut u8;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:             let {{__v[0-9]+}}: u8 = forward_buf[({{__v[0-9]+}} as usize)];
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let mut i2: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i2 = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i2;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 8;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = b"%d \0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = b"%d \0".as_ptr() as *mut u8;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i2;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:             let {{__v[0-9]+}}: u8 = backward_buf[({{__v[0-9]+}} as usize)];
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i2;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i2 = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let mut i3: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i3 = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i3;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 8;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = b"%d \0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = b"%d \0".as_ptr() as *mut u8;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i3;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:             let {{__v[0-9]+}}: u8 = dyn_buf[({{__v[0-9]+}} as usize)];
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i3;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i3 = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"\n\0".as_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut forward_buf: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// REWRITES-DAG:     let mut backward_buf: [u8; 8] = [0; 8];
// REWRITES-DAG:     let mut dyn_buf: [u8; 8] = [0; 8];
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = forward_buf.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = forward_buf.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     unsafe { std::ptr::copy({{__v[0-9]+}} as *const u8, {{__v[0-9]+}} as *mut u8, (5 as u64) as usize) };
// REWRITES-DAG:     backward_buf = [1, 2, 3, 4, 5, 6, 7, 8];
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = backward_buf.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = backward_buf.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:     unsafe { std::ptr::copy({{__v[0-9]+}} as *const u8, {{__v[0-9]+}} as *mut u8, (5 as u64) as usize) };
// REWRITES-DAG:     dyn_buf = [1, 2, 3, 4, 5, 6, 7, 8];
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = get_count();
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_buf.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_buf.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         std::ptr::copy(
// REWRITES-DAG:             {{__v[0-9]+}} as *const u8,
// REWRITES-DAG:             {{__v[0-9]+}} as *mut u8,
// REWRITES-DAG:             ({{__v[0-9]+}} as u64) as usize,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let mut i: i32 = 0;
// REWRITES-DAG:     while i < 8 {
// REWRITES-DAG:         unsafe { printf(c"%d ".as_ptr(), forward_buf[((i as i64) as usize)] as i32) };
// REWRITES-DAG:         i += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     let mut i2: i32 = 0;
// REWRITES-DAG:     while i2 < 8 {
// REWRITES-DAG:         unsafe { printf(c"%d ".as_ptr(), backward_buf[((i2 as i64) as usize)] as i32) };
// REWRITES-DAG:         i2 += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     let mut i3: i32 = 0;
// REWRITES-DAG:     while i3 < 8 {
// REWRITES-DAG:         unsafe { printf(c"%d ".as_ptr(), dyn_buf[((i3 as i64) as usize)] as i32) };
// REWRITES-DAG:         i3 += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { printf(c"\n".as_ptr()) };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
