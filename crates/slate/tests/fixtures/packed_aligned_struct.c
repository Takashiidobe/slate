#include <stddef.h>
#include <stdio.h>

struct __attribute__((packed, aligned(4))) PackedAligned {
  char a;
  int  b;
};

int main(void) {
  struct PackedAligned s;
  s.a = 7;
  s.b = 0x1234;

  printf("%zu %zu\n", sizeof(struct PackedAligned),
         _Alignof(struct PackedAligned));
  printf("%zu %zu\n", offsetof(struct PackedAligned, a),
         offsetof(struct PackedAligned, b));
  printf("%d %x\n", s.a, s.b);

  s.b = s.b + 1;
  printf("%x\n", s.b);
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
// COMMON-LOWERING-NEXT: #[repr(C, packed)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct PackedAligned__packed {
// COMMON-LOWERING-NEXT:     b: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C, align(4))]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct PackedAligned(PackedAligned__packed);
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: impl core::ops::Deref for PackedAligned {
// COMMON-LOWERING-NEXT:     type Target = PackedAligned__packed;
// COMMON-LOWERING-NEXT:     fn deref(&self) -> &PackedAligned__packed {
// COMMON-LOWERING-NEXT:         &self.0
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: impl core::ops::DerefMut for PackedAligned {
// COMMON-LOWERING-NEXT:     fn deref_mut(&mut self) -> &mut PackedAligned__packed {
// COMMON-LOWERING-NEXT:         &mut self.0
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut s: aligned::Aligned<aligned::A4, PackedAligned> =
// COMMON-LOWERING-NEXT:         aligned::Aligned(PackedAligned(PackedAligned__packed { a: 0, b: 0 }));
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     s.a = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4660;
// COMMON-LOWERING-NEXT:     s.b = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<PackedAligned>() as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::align_of::<PackedAligned>() as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = s.b;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = s.b;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     s.b = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = s.b;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     a: i8,
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 7;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %x\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = s.a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%x\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     a: u8,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 7;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu %zu\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu %zu\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %x\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = s.a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%x\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: #[repr(C, packed)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct PackedAligned__packed {
// COMMON-REWRITES-NEXT:     b: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C, align(4))]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct PackedAligned(PackedAligned__packed);
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: impl core::ops::Deref for PackedAligned {
// COMMON-REWRITES-NEXT:     type Target = PackedAligned__packed;
// COMMON-REWRITES-NEXT:     fn deref(&self) -> &PackedAligned__packed {
// COMMON-REWRITES-NEXT:         &self.0
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: impl core::ops::DerefMut for PackedAligned {
// COMMON-REWRITES-NEXT:     fn deref_mut(&mut self) -> &mut PackedAligned__packed {
// COMMON-REWRITES-NEXT:         &mut self.0
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut s: aligned::Aligned<aligned::A4, PackedAligned> =
// COMMON-REWRITES-NEXT:         aligned::Aligned(PackedAligned(PackedAligned__packed { a: 0, b: 0 }));
// COMMON-REWRITES-NEXT:     s.a = 7;
// COMMON-REWRITES-NEXT:     s.b = 4660;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%zu %zu\n".as_ptr(),
// COMMON-REWRITES-NEXT:             std::mem::size_of::<PackedAligned>() as u64,
// COMMON-REWRITES-NEXT:             std::mem::align_of::<PackedAligned>() as u64,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%zu %zu\n".as_ptr(), 0 as u64, 1 as u64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = s.b;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %x\n".as_ptr(), s.a as i32, {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:     s.b += 1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = s.b;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%x\n".as_ptr(), {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     a: i8,
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     a: u8,
// SLATE-FILECHECK-END rewrites-aarch64-gnu
