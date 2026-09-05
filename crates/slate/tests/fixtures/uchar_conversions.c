#include <stdio.h>
#include <uchar.h>

int main(void) {
  mbstate_t state16        = {0};
  mbstate_t state32        = {0};
  char16_t  converted16    = 0;
  char32_t  converted32    = 0;
  char      multibyte16[4] = {0};
  char      multibyte32[4] = {0};

  size_t read16  = mbrtoc16(&converted16, "A", 1, &state16);
  size_t write16 = c16rtomb(multibyte16, u'A', &state16);
  size_t read32  = mbrtoc32(&converted32, "B", 1, &state32);
  size_t write32 = c32rtomb(multibyte32, U'B', &state32);

  printf("%zu %zu %u %d %zu %zu %u %d\n", read16, write16,
         (unsigned)converted16, multibyte16[0], read32, write32,
         (unsigned)converted32, multibyte32[0]);
  return 0;
}

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
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct __mbstate_t {
// COMMON-LOWERING-NEXT:     __count: i32,
// COMMON-LOWERING-NEXT:     __value: {{anon_[0-9]+}},
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: union {{anon_[0-9]+}} {
// COMMON-LOWERING-NEXT:     __wch: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn mbrtoc16(
// COMMON-LOWERING-NEXT:         _0: *mut u16,
// COMMON-LOWERING-NEXT:         _1: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _2: usize,
// COMMON-LOWERING-NEXT:         _3: *mut __mbstate_t,
// COMMON-LOWERING-NEXT:     ) -> usize;
// COMMON-LOWERING-NEXT:     fn c16rtomb(_0: *mut core::ffi::c_char, _1: u16, _2: *mut __mbstate_t) -> usize;
// COMMON-LOWERING-NEXT:     fn mbrtoc32(
// COMMON-LOWERING-NEXT:         _0: *mut u32,
// COMMON-LOWERING-NEXT:         _1: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _2: usize,
// COMMON-LOWERING-NEXT:         _3: *mut __mbstate_t,
// COMMON-LOWERING-NEXT:     ) -> usize;
// COMMON-LOWERING-NEXT:     fn c32rtomb(_0: *mut core::ffi::c_char, _1: u32, _2: *mut __mbstate_t) -> usize;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut state16: __mbstate_t = __mbstate_t {
// COMMON-LOWERING-NEXT:         __count: 0,
// COMMON-LOWERING-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut state32: __mbstate_t = __mbstate_t {
// COMMON-LOWERING-NEXT:         __count: 0,
// COMMON-LOWERING-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut converted16: u16 = 0;
// COMMON-LOWERING-NEXT:     let mut converted32: u32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: __mbstate_t = __mbstate_t {
// COMMON-LOWERING-NEXT:         __count: 0,
// COMMON-LOWERING-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     state16 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: __mbstate_t = __mbstate_t {
// COMMON-LOWERING-NEXT:         __count: 0,
// COMMON-LOWERING-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     state32 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u16 = 0;
// COMMON-LOWERING-NEXT:     converted16 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     converted32 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     multibyte16 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     multibyte32 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-LOWERING-NEXT:         mbrtoc16(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(converted16) as *mut u16,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(state16) as *mut __mbstate_t,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u16 = 65;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-LOWERING-NEXT:         c16rtomb(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as u16,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(state16) as *mut __mbstate_t,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-LOWERING-NEXT:         mbrtoc32(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(converted32) as *mut u32,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(state32) as *mut __mbstate_t,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 66;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-LOWERING-NEXT:         c32rtomb(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as u32,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(state32) as *mut __mbstate_t,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u16 = converted16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = converted32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
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
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     __wchb: [i8; 4],
// LOWERING-X86_64-GNU-NEXT:     let mut multibyte16: [i8; 4] = [0; 4];
// LOWERING-X86_64-GNU-NEXT:     let mut multibyte32: [i8; 4] = [0; 4];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [0; 4];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [0; 4];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"A\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = multibyte16.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"B\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = multibyte32.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %zu %u %d %zu %zu %u %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = multibyte16[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = multibyte32[({{__v[0-9]+}} as usize)];
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     __wchb: [u8; 4],
// LOWERING-AARCH64-GNU-NEXT:     let mut multibyte16: [u8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let mut multibyte32: [u8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"A\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = multibyte16.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"B\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = multibyte32.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu %zu %u %d %zu %zu %u %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = multibyte16[({{__v[0-9]+}} as usize)];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = multibyte32[({{__v[0-9]+}} as usize)];
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
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct __mbstate_t {
// COMMON-REWRITES-NEXT:     __count: i32,
// COMMON-REWRITES-NEXT:     __value: {{anon_[0-9]+}},
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: union {{anon_[0-9]+}} {
// COMMON-REWRITES-NEXT:     __wch: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn mbrtoc16(
// COMMON-REWRITES-NEXT:         _0: *mut u16,
// COMMON-REWRITES-NEXT:         _1: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _2: usize,
// COMMON-REWRITES-NEXT:         _3: *mut __mbstate_t,
// COMMON-REWRITES-NEXT:     ) -> usize;
// COMMON-REWRITES-NEXT:     fn c16rtomb(_0: *mut core::ffi::c_char, _1: u16, _2: *mut __mbstate_t) -> usize;
// COMMON-REWRITES-NEXT:     fn mbrtoc32(
// COMMON-REWRITES-NEXT:         _0: *mut u32,
// COMMON-REWRITES-NEXT:         _1: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _2: usize,
// COMMON-REWRITES-NEXT:         _3: *mut __mbstate_t,
// COMMON-REWRITES-NEXT:     ) -> usize;
// COMMON-REWRITES-NEXT:     fn c32rtomb(_0: *mut core::ffi::c_char, _1: u32, _2: *mut __mbstate_t) -> usize;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut state16: __mbstate_t = __mbstate_t {
// COMMON-REWRITES-NEXT:         __count: 0,
// COMMON-REWRITES-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut state32: __mbstate_t = __mbstate_t {
// COMMON-REWRITES-NEXT:         __count: 0,
// COMMON-REWRITES-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut converted16: u16 = 0;
// COMMON-REWRITES-NEXT:     let mut converted32: u32 = 0;
// COMMON-REWRITES-NEXT:     state16 = __mbstate_t {
// COMMON-REWRITES-NEXT:         __count: 0,
// COMMON-REWRITES-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     state32 = __mbstate_t {
// COMMON-REWRITES-NEXT:         __count: 0,
// COMMON-REWRITES-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     multibyte16 = [0; 4];
// COMMON-REWRITES-NEXT:     multibyte32 = [0; 4];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-REWRITES-NEXT:         mbrtoc16(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(converted16) as *mut u16,
// COMMON-REWRITES-NEXT:             c"A".as_ptr(),
// COMMON-REWRITES-NEXT:             (1 as u64) as usize,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(state16) as *mut __mbstate_t,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     }) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-REWRITES-NEXT:         c16rtomb(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             65 as u16,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(state16) as *mut __mbstate_t,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     }) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-REWRITES-NEXT:         mbrtoc32(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(converted32) as *mut u32,
// COMMON-REWRITES-NEXT:             c"B".as_ptr(),
// COMMON-REWRITES-NEXT:             (1 as u64) as usize,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(state32) as *mut __mbstate_t,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     }) as u64;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%zu %zu %u %d %zu %zu %u %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             converted16 as u32,
// COMMON-REWRITES-NEXT:             multibyte16[0] as i32,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             (unsafe {
// COMMON-REWRITES-NEXT:                 c32rtomb(
// COMMON-REWRITES-NEXT:                     {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:                     66 as u32,
// COMMON-REWRITES-NEXT:                     std::ptr::addr_of_mut!(state32) as *mut __mbstate_t,
// COMMON-REWRITES-NEXT:                 )
// COMMON-REWRITES-NEXT:             }) as u64,
// COMMON-REWRITES-NEXT:             converted32,
// COMMON-REWRITES-NEXT:             multibyte32[0] as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     __wchb: [i8; 4],
// REWRITES-X86_64-GNU-NEXT:     let mut multibyte16: [i8; 4] = [0; 4];
// REWRITES-X86_64-GNU-NEXT:     let mut multibyte32: [i8; 4] = [0; 4];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = multibyte16.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = multibyte32.as_mut_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     __wchb: [u8; 4],
// REWRITES-AARCH64-GNU-NEXT:     let mut multibyte16: [u8; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let mut multibyte32: [u8; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = multibyte16.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = multibyte32.as_mut_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
