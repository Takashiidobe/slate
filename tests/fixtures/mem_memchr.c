#include <stdio.h>

int main(void) {
  unsigned char       buf[8]    = {10, 20, 30, 40, 50, 60, 70, 80};
  const unsigned char cbuf[4]   = {9, 8, 7, 6};
  char                word[]    = "abc";
  int                 needle    = (int)buf[3];
  size_t volatile partial_count = 4;
  unsigned char *hit        = (unsigned char *)__builtin_memchr(buf, needle, 8);
  unsigned char *miss       = (unsigned char *)__builtin_memchr(buf, 99, 8);
  unsigned char *zero       = (unsigned char *)__builtin_memchr(buf, 10, 0);
  char          *nul_after  = (char *)__builtin_memchr(word, 0, sizeof word);
  char          *nul_equal  = (char *)__builtin_memchr(word, 0, 3);
  char          *nul_before = (char *)__builtin_memchr(word, 0, 2);
  unsigned char *partial =
      (unsigned char *)__builtin_memchr(buf, needle, partial_count);
  unsigned char *offset_base = buf + 2;
  unsigned char *offset =
      (unsigned char *)__builtin_memchr(offset_base, needle, 2);
  const unsigned char *const_hit =
      (const unsigned char *)__builtin_memchr(cbuf, 7, sizeof cbuf);
  printf("%ld %d %d %ld %d %d %ld %ld %ld\n", (long)(hit - buf), miss == 0,
         zero == 0, (long)(nul_after - word), nul_equal == 0, nul_before == 0,
         (long)(partial - buf), (long)(offset - offset_base),
         (long)(const_hit - cbuf));
  return 0;
}

// REWRITES-DAG: fn __slate_memchr(
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-NOT: let mut hit
// REWRITES-NOT: let mut miss
// REWRITES-NOT: let mut nul_after
// REWRITES-NOT: map_or(std::ptr::null_mut()
// REWRITES: {{^}}}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn __slate_memchr(__s: *const core::ffi::c_void, __c: i32, __n: usize) -> *mut core::ffi::c_void {
// COMMON-LOWERING-NEXT:     let b: u8 = __c as u8;
// COMMON-LOWERING-NEXT:     let bytes: *const u8 = __s as *const u8;
// COMMON-LOWERING-NEXT:     let mut i: usize = 0;
// COMMON-LOWERING-NEXT:     while i < __n {
// COMMON-LOWERING-NEXT:         if (unsafe { *bytes.add(i) }) == b {
// COMMON-LOWERING-NEXT:             return (unsafe { bytes.add(i) }) as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:         i += 1;
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return std::ptr::null_mut();
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 =
// COMMON-LOWERING-NEXT:         __SlateAllocaFrame0(0, [0; 4], [0; 4], [0; 8]);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [u8; 8] = [10, 20, 30, 40, 50, 60, 70, 80];
// COMMON-LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [9, 8, 7, 6];
// COMMON-LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.1 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = {{__slate_alloca_frame[0-9]+}}.3[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.0), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 99;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 =
// COMMON-LOWERING-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!({{__slate_alloca_frame[0-9]+}}.0)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT: struct __SlateAllocaFrame0(u64, [i8; 4], [u8; 4], [u8; 8]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [97, 98, 99, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%ld %d %d %ld %d %d %ld %ld %ld\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT: struct __SlateAllocaFrame0(u64, [u8; 4], [u8; 4], [u8; 8]);
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [97, 98, 99, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%ld %d %d %ld %d %d %ld %ld %ld\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn __slate_memchr(__s: *const core::ffi::c_void, __c: i32, __n: usize) -> *mut core::ffi::c_void {
// COMMON-REWRITES-NEXT:     let b: u8 = __c as u8;
// COMMON-REWRITES-NEXT:     let bytes: *const u8 = __s as *const u8;
// COMMON-REWRITES-NEXT:     let mut i: usize = 0;
// COMMON-REWRITES-NEXT:     while i < __n {
// COMMON-REWRITES-NEXT:         if (unsafe { *bytes.add(i) }) == b {
// COMMON-REWRITES-NEXT:             return (unsafe { bytes.add(i) }) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         i += 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     std::ptr::null_mut()
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 =
// COMMON-REWRITES-NEXT:         __SlateAllocaFrame0(0, [0; 4], [0; 4], [0; 8]);
// COMMON-REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = [10, 20, 30, 40, 50, 60, 70, 80];
// COMMON-REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.2 = [9, 8, 7, 6];
// COMMON-REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.1 = [97, 98, 99, 0];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.3[3] as i32;
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.0), 4 as u64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr(
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}} as i32,
// COMMON-REWRITES-NEXT:         (8 as u64) as usize,
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr(
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         99 as i32,
// COMMON-REWRITES-NEXT:         (8 as u64) as usize,
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr(
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         10 as i32,
// COMMON-REWRITES-NEXT:         (0 as u64) as usize,
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr(
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         0 as i32,
// COMMON-REWRITES-NEXT:         (4 as u64) as usize,
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr(
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         0 as i32,
// COMMON-REWRITES-NEXT:         (3 as u64) as usize,
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr(
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         0 as i32,
// COMMON-REWRITES-NEXT:         (2 as u64) as usize,
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 =
// COMMON-REWRITES-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!({{__slate_alloca_frame[0-9]+}}.0)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr(
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}} as i32,
// COMMON-REWRITES-NEXT:         (2 as u64) as usize,
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr(
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         7 as i32,
// COMMON-REWRITES-NEXT:         (4 as u64) as usize,
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == std::ptr::null_mut()) as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == std::ptr::null_mut()) as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == std::ptr::null_mut()) as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == std::ptr::null_mut()) as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 },
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: struct __SlateAllocaFrame0(u64, [i8; 4], [u8; 4], [u8; 8]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%ld %d %d %ld %d %d %ld %ld %ld\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT: struct __SlateAllocaFrame0(u64, [u8; 4], [u8; 4], [u8; 8]);
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%ld %d %d %ld %d %d %ld %ld %ld\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
