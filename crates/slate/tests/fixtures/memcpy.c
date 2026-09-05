#include <stdio.h>
#include <string.h>

static int get_count(void) { return 4; }

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  unsigned char full_src[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  unsigned char full_dst[8] = {0};
  memcpy(full_dst, full_src, 8);

  unsigned char partial_src[8] = {9, 8, 7, 6, 5, 4, 3, 2};
  unsigned char partial_dst[8] = {0};
  memcpy(partial_dst, partial_src, 4);

  unsigned char alias_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memcpy(alias_buf, alias_buf + 4, 4);

  unsigned char dyn_src[8] = {1, 1, 1, 1, 1, 1, 1, 1};
  unsigned char dyn_dst[8] = {0};
  int           n          = get_count();
  memcpy(dyn_dst, dyn_src, n);

  for (int i = 0; i < 8; i++)
    printf("%d ", full_dst[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", partial_dst[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", alias_buf[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dyn_dst[i]);
  printf("\n");
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut full_src: [u8; 8] = [0; 8];
// LOWERING-DAG:     let mut full_dst: [u8; 8] = [0; 8];
// LOWERING-DAG:     let mut partial_src: [u8; 8] = [0; 8];
// LOWERING-DAG:     let mut partial_dst: [u8; 8] = [0; 8];
// LOWERING-DAG:     let mut alias_buf: [u8; 8] = [0; 8];
// LOWERING-DAG:     let mut dyn_src: [u8; 8] = [0; 8];
// LOWERING-DAG:     let mut dyn_dst: [u8; 8] = [0; 8];
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// LOWERING-DAG:     full_src = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [0; 8];
// LOWERING-DAG:     full_dst = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = full_dst.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = full_src.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-DAG:         memcpy(
// LOWERING-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as usize,
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [9, 8, 7, 6, 5, 4, 3, 2];
// LOWERING-DAG:     partial_src = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [0; 8];
// LOWERING-DAG:     partial_dst = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = partial_dst.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = partial_src.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-DAG:         memcpy(
// LOWERING-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as usize,
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// LOWERING-DAG:     alias_buf = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = alias_buf.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = alias_buf.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(4) };
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-DAG:         memcpy(
// LOWERING-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as usize,
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 1, 1, 1, 1, 1, 1, 1];
// LOWERING-DAG:     dyn_src = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [0; 8];
// LOWERING-DAG:     dyn_dst = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = get_count();
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_dst.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_src.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-DAG:         memcpy(
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
// LOWERING-DAG:             let {{__v[0-9]+}}: u8 = full_dst[({{__v[0-9]+}} as usize)];
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
// LOWERING-DAG:             let {{__v[0-9]+}}: u8 = partial_dst[({{__v[0-9]+}} as usize)];
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
// LOWERING-DAG:             let {{__v[0-9]+}}: u8 = alias_buf[({{__v[0-9]+}} as usize)];
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i3;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i3 = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let mut i4: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i4 = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i4;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 8;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = b"%d \0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = b"%d \0".as_ptr() as *mut u8;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i4;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:             let {{__v[0-9]+}}: u8 = dyn_dst[({{__v[0-9]+}} as usize)];
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i4;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i4 = {{__v[0-9]+}};
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
// REWRITES-DAG:     let mut full_src: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// REWRITES-DAG:     let mut full_dst: [u8; 8] = [0; 8];
// REWRITES-DAG:     let mut partial_src: [u8; 8] = [0; 8];
// REWRITES-DAG:     let mut partial_dst: [u8; 8] = [0; 8];
// REWRITES-DAG:     let mut alias_buf: [u8; 8] = [0; 8];
// REWRITES-DAG:     let mut dyn_src: [u8; 8] = [0; 8];
// REWRITES-DAG:     let mut dyn_dst: [u8; 8] = [0; 8];
// REWRITES-DAG:     full_dst.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     full_src.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     unsafe { full_dst[(0usize..8usize)].copy_from_slice(&full_src[(0usize..8usize)]) };
// REWRITES-DAG:     partial_src = [9, 8, 7, 6, 5, 4, 3, 2];
// REWRITES-DAG:     partial_dst = [0; 8];
// REWRITES-DAG:     partial_dst.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     partial_src.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     unsafe { partial_dst[(0usize..4usize)].copy_from_slice(&partial_src[(0usize..4usize)]) };
// REWRITES-DAG:     alias_buf = [1, 2, 3, 4, 5, 6, 7, 8];
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = alias_buf.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = alias_buf.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(4) };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         std::ptr::copy_nonoverlapping({{__v[0-9]+}} as *const u8, {{__v[0-9]+}} as *mut u8, (4 as u64) as usize)
// REWRITES-DAG:     };
// REWRITES-DAG:     dyn_src = [1, 1, 1, 1, 1, 1, 1, 1];
// REWRITES-DAG:     dyn_dst = [0; 8];
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = get_count();
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_dst.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_src.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         std::ptr::copy_nonoverlapping(
// REWRITES-DAG:             {{__v[0-9]+}} as *const u8,
// REWRITES-DAG:             {{__v[0-9]+}} as *mut u8,
// REWRITES-DAG:             ({{__v[0-9]+}} as u64) as usize,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let mut i: i32 = 0;
// REWRITES-DAG:     while i < 8 {
// REWRITES-DAG:         unsafe { printf(c"%d ".as_ptr(), full_dst[((i as i64) as usize)] as i32) };
// REWRITES-DAG:         i += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     let mut i2: i32 = 0;
// REWRITES-DAG:     while i2 < 8 {
// REWRITES-DAG:         unsafe { printf(c"%d ".as_ptr(), partial_dst[((i2 as i64) as usize)] as i32) };
// REWRITES-DAG:         i2 += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     let mut i3: i32 = 0;
// REWRITES-DAG:     while i3 < 8 {
// REWRITES-DAG:         unsafe { printf(c"%d ".as_ptr(), alias_buf[((i3 as i64) as usize)] as i32) };
// REWRITES-DAG:         i3 += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     let mut i4: i32 = 0;
// REWRITES-DAG:     while i4 < 8 {
// REWRITES-DAG:         unsafe { printf(c"%d ".as_ptr(), dyn_dst[((i4 as i64) as usize)] as i32) };
// REWRITES-DAG:         i4 += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { printf(c"\n".as_ptr()) };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
