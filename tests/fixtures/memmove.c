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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn main() {
// COMMON-LOWERING-DAG:     let mut forward_buf: [u8; 8] = [0; 8];
// COMMON-LOWERING-DAG:     let mut backward_buf: [u8; 8] = [0; 8];
// COMMON-LOWERING-DAG:     let mut dyn_buf: [u8; 8] = [0; 8];
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// COMMON-LOWERING-DAG:     forward_buf = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = forward_buf.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = forward_buf.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 5;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-DAG:         memmove(
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-DAG:         )
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// COMMON-LOWERING-DAG:     backward_buf = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = backward_buf.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = backward_buf.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 5;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-DAG:         memmove(
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-DAG:         )
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// COMMON-LOWERING-DAG:     dyn_buf = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = get_count();
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_buf.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_buf.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-DAG:         memmove(
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-DAG:         )
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let mut i: i32 = 0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:         i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         loop {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                 break;
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: u8 = forward_buf[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG:             i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let mut i2: i32 = 0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:         i2 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         loop {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i2;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                 break;
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i2;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: u8 = backward_buf[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i2;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG:             i2 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let mut i3: i32 = 0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:         i3 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         loop {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i3;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                 break;
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i3;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: u8 = dyn_buf[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i3;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG:             i3 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = b"%d \0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = b"%d \0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = b"%d \0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = b"%d \0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = b"%d \0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = b"%d \0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn main() {
// COMMON-REWRITES-DAG:     let mut forward_buf: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// COMMON-REWRITES-DAG:     let mut backward_buf: [u8; 8] = [0; 8];
// COMMON-REWRITES-DAG:     let mut dyn_buf: [u8; 8] = [0; 8];
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = forward_buf.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = forward_buf.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-DAG:     unsafe { std::ptr::copy({{__v[0-9]+}} as *const u8, {{__v[0-9]+}} as *mut u8, (5 as u64) as usize) };
// COMMON-REWRITES-DAG:     backward_buf = [1, 2, 3, 4, 5, 6, 7, 8];
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = backward_buf.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = backward_buf.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-DAG:     unsafe { std::ptr::copy({{__v[0-9]+}} as *const u8, {{__v[0-9]+}} as *mut u8, (5 as u64) as usize) };
// COMMON-REWRITES-DAG:     dyn_buf = [1, 2, 3, 4, 5, 6, 7, 8];
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = get_count();
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_buf.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_buf.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         std::ptr::copy(
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *const u8,
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *mut u8,
// COMMON-REWRITES-DAG:             ({{__v[0-9]+}} as u64) as usize,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let mut i: i32 = 0;
// COMMON-REWRITES-DAG:     while i < 8 {
// COMMON-REWRITES-DAG:         unsafe { printf(c"%d ".as_ptr(), forward_buf[((i as i64) as usize)] as i32) };
// COMMON-REWRITES-DAG:         i += 1;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     let mut i2: i32 = 0;
// COMMON-REWRITES-DAG:     while i2 < 8 {
// COMMON-REWRITES-DAG:         unsafe { printf(c"%d ".as_ptr(), backward_buf[((i2 as i64) as usize)] as i32) };
// COMMON-REWRITES-DAG:         i2 += 1;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     let mut i3: i32 = 0;
// COMMON-REWRITES-DAG:     while i3 < 8 {
// COMMON-REWRITES-DAG:         unsafe { printf(c"%d ".as_ptr(), dyn_buf[((i3 as i64) as usize)] as i32) };
// COMMON-REWRITES-DAG:         i3 += 1;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     unsafe { printf(c"\n".as_ptr()) };
// COMMON-REWRITES-DAG:     std::process::exit(0 as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
