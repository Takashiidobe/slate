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
// LOWERING-NEXT: struct PackedAligned__packed {
// LOWERING-X86_64-GNU-NEXT:     a: i8,
// LOWERING-AARCH64-GNU-NEXT:     a: u8,
// LOWERING-NEXT:     b: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, align(4))]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct PackedAligned(PackedAligned__packed);
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Deref for PackedAligned {
// LOWERING-NEXT:     type Target = PackedAligned__packed;
// LOWERING-NEXT:     fn deref(&self) -> &PackedAligned__packed {
// LOWERING-NEXT:         &self.0
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::DerefMut for PackedAligned {
// LOWERING-NEXT:     fn deref_mut(&mut self) -> &mut PackedAligned__packed {
// LOWERING-NEXT:         &mut self.0
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut s: aligned::Aligned<aligned::A4, PackedAligned> =
// LOWERING-NEXT:         aligned::Aligned(PackedAligned(PackedAligned__packed { a: 0, b: 0 }));
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 7;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 7;
// LOWERING-NEXT:     s.a = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4660;
// LOWERING-NEXT:     s.b = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu %zu\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<PackedAligned>() as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::align_of::<PackedAligned>() as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu %zu\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %x\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = s.a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %x\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = s.a;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = s.b;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = s.b;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     s.b = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%x\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%x\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = s.b;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
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
// REWRITES-NEXT: #[repr(C, packed)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct PackedAligned__packed {
// REWRITES-X86_64-GNU-NEXT:     a: i8,
// REWRITES-AARCH64-GNU-NEXT:     a: u8,
// REWRITES-NEXT:     b: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, align(4))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct PackedAligned(PackedAligned__packed);
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Deref for PackedAligned {
// REWRITES-NEXT:     type Target = PackedAligned__packed;
// REWRITES-NEXT:     fn deref(&self) -> &PackedAligned__packed {
// REWRITES-NEXT:         &self.0
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::DerefMut for PackedAligned {
// REWRITES-NEXT:     fn deref_mut(&mut self) -> &mut PackedAligned__packed {
// REWRITES-NEXT:         &mut self.0
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut s: aligned::Aligned<aligned::A4, PackedAligned> =
// REWRITES-NEXT:         aligned::Aligned(PackedAligned(PackedAligned__packed { a: 0, b: 0 }));
// REWRITES-NEXT:     s.a = 7;
// REWRITES-NEXT:     s.b = 4660;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%zu %zu\n".as_ptr(),
// REWRITES-NEXT:             std::mem::size_of::<PackedAligned>() as u64,
// REWRITES-NEXT:             std::mem::align_of::<PackedAligned>() as u64,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe { printf(c"%zu %zu\n".as_ptr(), 0 as u64, 1 as u64) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = s.b;
// REWRITES-NEXT:     unsafe { printf(c"%d %x\n".as_ptr(), s.a as i32, {{__v[0-9]+}}) };
// REWRITES-NEXT:     s.b += 1;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = s.b;
// REWRITES-NEXT:     unsafe { printf(c"%x\n".as_ptr(), {{__v[0-9]+}}) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
