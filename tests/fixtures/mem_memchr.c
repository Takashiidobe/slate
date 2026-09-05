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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: fn __slate_memchr(__s: *const core::ffi::c_void, __c: i32, __n: usize) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let b: u8 = __c as u8;
// LOWERING-NEXT:     let bytes: *const u8 = __s as *const u8;
// LOWERING-NEXT:     let mut i: usize = 0;
// LOWERING-NEXT:     while i < __n {
// LOWERING-NEXT:         if (unsafe { *bytes.add(i) }) == b {
// LOWERING-NEXT:             return (unsafe { bytes.add(i) }) as *mut core::ffi::c_void;
// LOWERING-NEXT:         }
// LOWERING-NEXT:         i += 1;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return std::ptr::null_mut();
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-X86_64-GNU-NEXT: struct __SlateAllocaFrame0(u64, [i8; 4], [u8; 4], [u8; 8]);
// LOWERING-AARCH64-GNU-NEXT: struct __SlateAllocaFrame0(u64, [u8; 4], [u8; 4], [u8; 8]);
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 =
// LOWERING-NEXT:         __SlateAllocaFrame0(0, [0; 4], [0; 4], [0; 8]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: [u8; 8] = [10, 20, 30, 40, 50, 60, 70, 80];
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [9, 8, 7, 6];
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.2 = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [97, 98, 99, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [97, 98, 99, 0];
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.1 = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = {{__slate_alloca_frame[0-9]+}}.3[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.0), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 99;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 =
// LOWERING-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!({{__slate_alloca_frame[0-9]+}}.0)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%ld %d %d %ld %d %d %ld %ld %ld\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%ld %d %d %ld %d %d %ld %ld %ld\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: fn __slate_memchr(__s: *const core::ffi::c_void, __c: i32, __n: usize) -> *mut core::ffi::c_void {
// REWRITES-NEXT:     let b: u8 = __c as u8;
// REWRITES-NEXT:     let bytes: *const u8 = __s as *const u8;
// REWRITES-NEXT:     let mut i: usize = 0;
// REWRITES-NEXT:     while i < __n {
// REWRITES-NEXT:         if (unsafe { *bytes.add(i) }) == b {
// REWRITES-NEXT:             return (unsafe { bytes.add(i) }) as *mut core::ffi::c_void;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         i += 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     std::ptr::null_mut()
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-X86_64-GNU-NEXT: struct __SlateAllocaFrame0(u64, [i8; 4], [u8; 4], [u8; 8]);
// REWRITES-AARCH64-GNU-NEXT: struct __SlateAllocaFrame0(u64, [u8; 4], [u8; 4], [u8; 8]);
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 =
// REWRITES-NEXT:         __SlateAllocaFrame0(0, [0; 4], [0; 4], [0; 8]);
// REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = [10, 20, 30, 40, 50, 60, 70, 80];
// REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.2 = [9, 8, 7, 6];
// REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.1 = [97, 98, 99, 0];
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.3[3] as i32;
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.0), 4 as u64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr(
// REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:         {{__v[0-9]+}} as i32,
// REWRITES-NEXT:         (8 as u64) as usize,
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr(
// REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:         99 as i32,
// REWRITES-NEXT:         (8 as u64) as usize,
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr(
// REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:         10 as i32,
// REWRITES-NEXT:         (0 as u64) as usize,
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr(
// REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:         0 as i32,
// REWRITES-NEXT:         (4 as u64) as usize,
// REWRITES-NEXT:     );
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr(
// REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:         0 as i32,
// REWRITES-NEXT:         (3 as u64) as usize,
// REWRITES-NEXT:     );
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr(
// REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:         0 as i32,
// REWRITES-NEXT:         (2 as u64) as usize,
// REWRITES-NEXT:     );
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 =
// REWRITES-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!({{__slate_alloca_frame[0-9]+}}.0)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{__v[0-9]+}}, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize);
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr(
// REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:         {{__v[0-9]+}} as i32,
// REWRITES-NEXT:         (2 as u64) as usize,
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr(
// REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:         7 as i32,
// REWRITES-NEXT:         (4 as u64) as usize,
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%ld %d %d %ld %d %d %ld %ld %ld\n".as_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%ld %d %d %ld %d %d %ld %ld %ld\n".as_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == std::ptr::null_mut()) as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == std::ptr::null_mut()) as i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == std::ptr::null_mut()) as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == std::ptr::null_mut()) as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 },
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
