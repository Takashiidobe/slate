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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __mbstate_t {
// LOWERING-NEXT:     __count: i32,
// LOWERING-NEXT:     __value: {{anon_[0-9]+}},
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union {{anon_[0-9]+}} {
// LOWERING-NEXT:     __wch: i32,
// LOWERING-NEXT:     __wchb: [i8; 4],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn mbrtoc16(_0: *mut u16, _1: *const i8, _2: usize, _3: *mut __mbstate_t) -> usize;
// LOWERING-NEXT:     fn c16rtomb(_0: *mut i8, _1: u16, _2: *mut __mbstate_t) -> usize;
// LOWERING-NEXT:     fn mbrtoc32(_0: *mut u32, _1: *const i8, _2: usize, _3: *mut __mbstate_t) -> usize;
// LOWERING-NEXT:     fn c32rtomb(_0: *mut i8, _1: u32, _2: *mut __mbstate_t) -> usize;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut state16: __mbstate_t = __mbstate_t { __count: 0, __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() } };
// LOWERING-NEXT:     let mut state32: __mbstate_t = __mbstate_t { __count: 0, __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() } };
// LOWERING-NEXT:     let mut converted16: u16 = 0;
// LOWERING-NEXT:     let mut converted32: u32 = 0;
// LOWERING-NEXT:     let mut multibyte16: [i8; 4] = [0; 4];
// LOWERING-NEXT:     let mut multibyte32: [i8; 4] = [0; 4];
// LOWERING-NEXT:     let mut read16: u64 = 0;
// LOWERING-NEXT:     let mut write16: u64 = 0;
// LOWERING-NEXT:     let mut read32: u64 = 0;
// LOWERING-NEXT:     let mut write32: u64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     state16 = __mbstate_t { __count: 0, __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() } };
// LOWERING-NEXT:     state32 = __mbstate_t { __count: 0, __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() } };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u16 = 0;
// LOWERING-NEXT:     converted16 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     converted32 = {{_v[0-9]+}};
// LOWERING-NEXT:     multibyte16 = [0, 0, 0, 0];
// LOWERING-NEXT:     multibyte32 = [0, 0, 0, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"A\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { mbrtoc16(std::ptr::addr_of_mut!(converted16) as *mut u16, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, std::ptr::addr_of_mut!(state16) as *mut __mbstate_t) }) as u64;
// LOWERING-NEXT:     read16 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = multibyte16.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u16 = 65;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { c16rtomb({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as u16, std::ptr::addr_of_mut!(state16) as *mut __mbstate_t) }) as u64;
// LOWERING-NEXT:     write16 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"B\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { mbrtoc32(std::ptr::addr_of_mut!(converted32) as *mut u32, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, std::ptr::addr_of_mut!(state32) as *mut __mbstate_t) }) as u64;
// LOWERING-NEXT:     read32 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = multibyte32.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 66;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { c32rtomb({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as u32, std::ptr::addr_of_mut!(state32) as *mut __mbstate_t) }) as u64;
// LOWERING-NEXT:     write32 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%zu %zu %u %d %zu %zu %u %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = read16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = write16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u16 = converted16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = multibyte16[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = read32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = write32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = converted32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = multibyte32[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __mbstate_t {
// REWRITES-NEXT:     __count: i32,
// REWRITES-NEXT:     __value: {{anon_[0-9]+}},
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union {{anon_[0-9]+}} {
// REWRITES-NEXT:     __wch: i32,
// REWRITES-NEXT:     __wchb: [i8; 4],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn mbrtoc16(_0: *mut u16, _1: *const i8, _2: usize, _3: *mut __mbstate_t) -> usize;
// REWRITES-NEXT:     fn c16rtomb(_0: *mut i8, _1: u16, _2: *mut __mbstate_t) -> usize;
// REWRITES-NEXT:     fn mbrtoc32(_0: *mut u32, _1: *const i8, _2: usize, _3: *mut __mbstate_t) -> usize;
// REWRITES-NEXT:     fn c32rtomb(_0: *mut i8, _1: u32, _2: *mut __mbstate_t) -> usize;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut state16: __mbstate_t = __mbstate_t { __count: 0, __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() } };
// REWRITES-NEXT: let mut state32: __mbstate_t = __mbstate_t { __count: 0, __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() } };
// REWRITES-NEXT: let mut converted16: u16 = 0;
// REWRITES-NEXT: let mut converted32: u32 = 0;
// REWRITES-NEXT: let mut multibyte16: [i8; 4] = [0; 4];
// REWRITES-NEXT: let mut multibyte32: [i8; 4] = [0; 4];
// REWRITES-NEXT: let mut read16: u64 = 0;
// REWRITES-NEXT: let mut write16: u64 = 0;
// REWRITES-NEXT: let mut read32: u64 = 0;
// REWRITES-NEXT: let mut write32: u64 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: state16 = __mbstate_t { __count: 0, __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() } };
// REWRITES-NEXT: state32 = __mbstate_t { __count: 0, __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() } };
// REWRITES-NEXT: converted16 = 0;
// REWRITES-NEXT: converted32 = 0;
// REWRITES-NEXT: multibyte16 = [0, 0, 0, 0];
// REWRITES-NEXT: multibyte32 = [0, 0, 0, 0];
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"A\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: read16 = (unsafe { mbrtoc16(std::ptr::addr_of_mut!(converted16) as *mut u16, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, std::ptr::addr_of_mut!(state16) as *mut __mbstate_t) }) as u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = multibyte16.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u16 = 65;
// REWRITES-NEXT: write16 = (unsafe { c16rtomb({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as u16, std::ptr::addr_of_mut!(state16) as *mut __mbstate_t) }) as u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"B\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: read32 = (unsafe { mbrtoc32(std::ptr::addr_of_mut!(converted32) as *mut u32, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, std::ptr::addr_of_mut!(state32) as *mut __mbstate_t) }) as u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = multibyte32.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 66;
// REWRITES-NEXT: write32 = (unsafe { c32rtomb({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as u32, std::ptr::addr_of_mut!(state32) as *mut __mbstate_t) }) as u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%zu %zu %u %d %zu %zu %u %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, read16, write16, converted16 as u32, multibyte16[({{_v[0-9]+}} as usize)] as i32, read32, write32, converted32, multibyte32[({{_v[0-9]+}} as usize)] as i32) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
