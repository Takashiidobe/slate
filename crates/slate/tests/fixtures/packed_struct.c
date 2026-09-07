#include <stddef.h>
#include <stdio.h>

struct __attribute__((packed)) Packed {
  char a;
  int  b;
  char c;
};

int main(void) {
  struct Packed p;
  p.a = 1;
  p.b = 0x11223344;
  p.c = 2;

  printf("%zu %zu\n", sizeof(struct Packed), _Alignof(struct Packed));
  printf("%zu %zu %zu\n", offsetof(struct Packed, a),
         offsetof(struct Packed, b), offsetof(struct Packed, c));
  printf("%d %x %d\n", p.a, p.b, p.c);
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
// LOWERING-NEXT: #[repr(C, packed)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Packed {
// LOWERING-X86_64-GNU-NEXT:     a: i8,
// LOWERING-AARCH64-GNU-NEXT:     a: u8,
// LOWERING-NEXT:     b: i32,
// LOWERING-X86_64-GNU-NEXT:     c: i8,
// LOWERING-AARCH64-GNU-NEXT:     c: u8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() -> std::process::ExitCode {
// LOWERING-NEXT:     let mut p: Packed = Packed { a: 0, b: 0, c: 0 };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 1;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 1;
// LOWERING-NEXT:     unsafe { std::ptr::write_unaligned(std::ptr::addr_of_mut!(p.a), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 287454020;
// LOWERING-NEXT:     unsafe { std::ptr::write_unaligned(std::ptr::addr_of_mut!(p.b), {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 2;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 2;
// LOWERING-NEXT:     unsafe { std::ptr::write_unaligned(std::ptr::addr_of_mut!(p.c), {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu %zu\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<Packed>() as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::align_of::<Packed>() as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu %zu %zu\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::offset_of!(Packed, a) as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::offset_of!(Packed, b) as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::offset_of!(Packed, c) as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %x %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(p.a)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %x %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(p.a)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(p.b)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(p.c)) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(p.c)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     return std::process::ExitCode::SUCCESS;
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
// REWRITES-NEXT: #[repr(C, packed)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Packed {
// REWRITES-X86_64-GNU-NEXT:     a: i8,
// REWRITES-AARCH64-GNU-NEXT:     a: u8,
// REWRITES-NEXT:     b: i32,
// REWRITES-X86_64-GNU-NEXT:     c: i8,
// REWRITES-AARCH64-GNU-NEXT:     c: u8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() -> std::process::ExitCode {
// REWRITES-NEXT:     let mut p: Packed = Packed { a: 0, b: 0, c: 0 };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_unaligned(std::ptr::addr_of_mut!(p.a), 1 as i8) };
// REWRITES-AARCH64-GNU-NEXT:     unsafe { std::ptr::write_unaligned(std::ptr::addr_of_mut!(p.a), 1 as u8) };
// REWRITES-NEXT:     unsafe { std::ptr::write_unaligned(std::ptr::addr_of_mut!(p.b), 287454020 as i32) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_unaligned(std::ptr::addr_of_mut!(p.c), 2 as i8) };
// REWRITES-AARCH64-GNU-NEXT:     unsafe { std::ptr::write_unaligned(std::ptr::addr_of_mut!(p.c), 2 as u8) };
// REWRITES-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%zu %zu\n".as_ptr(),
// REWRITES-NEXT:             std::mem::size_of::<Packed>() as u64,
// REWRITES-NEXT:             std::mem::align_of::<Packed>() as u64,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// REWRITES-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%zu %zu %zu\n".as_ptr(),
// REWRITES-NEXT:             std::mem::offset_of!(Packed, a) as u64,
// REWRITES-NEXT:             std::mem::offset_of!(Packed, b) as u64,
// REWRITES-NEXT:             std::mem::offset_of!(Packed, c) as u64,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %x %d\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(p.a)) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %x %d\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(p.a)) };
// REWRITES-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}} as i32,
// REWRITES-NEXT:             unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(p.b)) },
// REWRITES-NEXT:             (unsafe { std::ptr::read_unaligned(std::ptr::addr_of!(p.c)) }) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// REWRITES-NEXT:     return std::process::ExitCode::SUCCESS;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
