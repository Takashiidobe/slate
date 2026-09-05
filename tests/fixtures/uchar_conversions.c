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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __mbstate_t {
// LOWERING-NEXT:     __count: i32,
// LOWERING-NEXT:     __value: {{anon_[0-9]+}},
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union {{anon_[0-9]+}} {
// LOWERING-NEXT:     __wch: i32,
// LOWERING-X86_64-GNU-NEXT:     __wchb: [i8; 4],
// LOWERING-AARCH64-GNU-NEXT:     __wchb: [u8; 4],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn mbrtoc16(
// LOWERING-NEXT:         _0: *mut u16,
// LOWERING-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-NEXT:         _2: usize,
// LOWERING-NEXT:         _3: *mut __mbstate_t,
// LOWERING-NEXT:     ) -> usize;
// LOWERING-NEXT:     fn c16rtomb(_0: *mut core::ffi::c_char, _1: u16, _2: *mut __mbstate_t) -> usize;
// LOWERING-NEXT:     fn mbrtoc32(
// LOWERING-NEXT:         _0: *mut u32,
// LOWERING-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-NEXT:         _2: usize,
// LOWERING-NEXT:         _3: *mut __mbstate_t,
// LOWERING-NEXT:     ) -> usize;
// LOWERING-NEXT:     fn c32rtomb(_0: *mut core::ffi::c_char, _1: u32, _2: *mut __mbstate_t) -> usize;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut state16: __mbstate_t = __mbstate_t {
// LOWERING-NEXT:         __count: 0,
// LOWERING-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut state32: __mbstate_t = __mbstate_t {
// LOWERING-NEXT:         __count: 0,
// LOWERING-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut converted16: u16 = 0;
// LOWERING-NEXT:     let mut converted32: u32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let mut multibyte16: [i8; 4] = [0; 4];
// LOWERING-X86_64-GNU-NEXT:     let mut multibyte32: [i8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let mut multibyte16: [u8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let mut multibyte32: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: __mbstate_t = __mbstate_t {
// LOWERING-NEXT:         __count: 0,
// LOWERING-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// LOWERING-NEXT:     };
// LOWERING-NEXT:     state16 = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: __mbstate_t = __mbstate_t {
// LOWERING-NEXT:         __count: 0,
// LOWERING-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// LOWERING-NEXT:     };
// LOWERING-NEXT:     state32 = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u16 = 0;
// LOWERING-NEXT:     converted16 = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     converted32 = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [0; 4];
// LOWERING-NEXT:     multibyte16 = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [0; 4];
// LOWERING-NEXT:     multibyte32 = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"A\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"A\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// LOWERING-NEXT:         mbrtoc16(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(converted16) as *mut u16,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(state16) as *mut __mbstate_t,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as u64;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = multibyte16.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = multibyte16.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u16 = 65;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// LOWERING-NEXT:         c16rtomb(
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as u16,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(state16) as *mut __mbstate_t,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as u64;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"B\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"B\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// LOWERING-NEXT:         mbrtoc32(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(converted32) as *mut u32,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(state32) as *mut __mbstate_t,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as u64;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = multibyte32.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = multibyte32.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 66;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// LOWERING-NEXT:         c32rtomb(
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as u32,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(state32) as *mut __mbstate_t,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as u64;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %zu %u %d %zu %zu %u %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu %zu %u %d %zu %zu %u %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u16 = converted16;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = multibyte16[({{__v[0-9]+}} as usize)];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = multibyte16[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = converted32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = multibyte32[({{__v[0-9]+}} as usize)];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = multibyte32[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __mbstate_t {
// REWRITES-NEXT:     __count: i32,
// REWRITES-NEXT:     __value: {{anon_[0-9]+}},
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union {{anon_[0-9]+}} {
// REWRITES-NEXT:     __wch: i32,
// REWRITES-X86_64-GNU-NEXT:     __wchb: [i8; 4],
// REWRITES-AARCH64-GNU-NEXT:     __wchb: [u8; 4],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn mbrtoc16(
// REWRITES-NEXT:         _0: *mut u16,
// REWRITES-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-NEXT:         _2: usize,
// REWRITES-NEXT:         _3: *mut __mbstate_t,
// REWRITES-NEXT:     ) -> usize;
// REWRITES-NEXT:     fn c16rtomb(_0: *mut core::ffi::c_char, _1: u16, _2: *mut __mbstate_t) -> usize;
// REWRITES-NEXT:     fn mbrtoc32(
// REWRITES-NEXT:         _0: *mut u32,
// REWRITES-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-NEXT:         _2: usize,
// REWRITES-NEXT:         _3: *mut __mbstate_t,
// REWRITES-NEXT:     ) -> usize;
// REWRITES-NEXT:     fn c32rtomb(_0: *mut core::ffi::c_char, _1: u32, _2: *mut __mbstate_t) -> usize;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut state16: __mbstate_t = __mbstate_t {
// REWRITES-NEXT:         __count: 0,
// REWRITES-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut state32: __mbstate_t = __mbstate_t {
// REWRITES-NEXT:         __count: 0,
// REWRITES-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut converted16: u16 = 0;
// REWRITES-NEXT:     let mut converted32: u32 = 0;
// REWRITES-X86_64-GNU-NEXT:     let mut multibyte16: [i8; 4] = [0; 4];
// REWRITES-X86_64-GNU-NEXT:     let mut multibyte32: [i8; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let mut multibyte16: [u8; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let mut multibyte32: [u8; 4] = [0; 4];
// REWRITES-NEXT:     state16 = __mbstate_t {
// REWRITES-NEXT:         __count: 0,
// REWRITES-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// REWRITES-NEXT:     };
// REWRITES-NEXT:     state32 = __mbstate_t {
// REWRITES-NEXT:         __count: 0,
// REWRITES-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// REWRITES-NEXT:     };
// REWRITES-NEXT:     multibyte16 = [0; 4];
// REWRITES-NEXT:     multibyte32 = [0; 4];
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// REWRITES-NEXT:         mbrtoc16(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(converted16) as *mut u16,
// REWRITES-NEXT:             c"A".as_ptr(),
// REWRITES-NEXT:             (1 as u64) as usize,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(state16) as *mut __mbstate_t,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as u64;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = multibyte16.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = multibyte16.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// REWRITES-NEXT:         c16rtomb(
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// REWRITES-NEXT:             65 as u16,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(state16) as *mut __mbstate_t,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as u64;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// REWRITES-NEXT:         mbrtoc32(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(converted32) as *mut u32,
// REWRITES-NEXT:             c"B".as_ptr(),
// REWRITES-NEXT:             (1 as u64) as usize,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(state32) as *mut __mbstate_t,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as u64;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = multibyte32.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = multibyte32.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%zu %zu %u %d %zu %zu %u %d\n".as_ptr(),
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             converted16 as u32,
// REWRITES-NEXT:             multibyte16[0] as i32,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             (unsafe {
// REWRITES-NEXT:                 c32rtomb(
// REWRITES-NEXT:                     {{__v[0-9]+}} as *mut core::ffi::c_char,
// REWRITES-NEXT:                     66 as u32,
// REWRITES-NEXT:                     std::ptr::addr_of_mut!(state32) as *mut __mbstate_t,
// REWRITES-NEXT:                 )
// REWRITES-NEXT:             }) as u64,
// REWRITES-NEXT:             converted32,
// REWRITES-NEXT:             multibyte32[0] as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
