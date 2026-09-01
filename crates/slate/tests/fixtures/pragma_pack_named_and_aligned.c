#include <stddef.h>
#include <stdio.h>

#pragma pack(3)
struct IgnoredBadAlign {
  char a;
  int b;
};
#pragma pack()

#pragma pack(push, lbl, 1)
struct NamedPushed {
  char a;
  int b;
};
#pragma pack(pop, lbl)

struct AfterNamedPop {
  char a;
  int b;
};

#pragma pack(pop)
struct AfterExtraPop {
  char a;
  int b;
};

#pragma pack(push, 1)
struct __attribute__((aligned(16))) PackedButAligned {
  char a;
  int b;
  char c;
};
#pragma pack(pop)

int main(void) {
  printf("%d %d %d %d %d %d %d\n", (int)sizeof(struct IgnoredBadAlign),
         (int)sizeof(struct NamedPushed), (int)sizeof(struct AfterNamedPop),
         (int)sizeof(struct AfterExtraPop),
         (int)sizeof(struct PackedButAligned),
         (int)_Alignof(struct PackedButAligned),
         (int)offsetof(struct PackedButAligned, b));
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
// LOWERING-NEXT: struct AfterExtraPop {
// LOWERING-NEXT:     a: i8,
// LOWERING-NEXT:     b: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct AfterNamedPop {
// LOWERING-NEXT:     a: i8,
// LOWERING-NEXT:     b: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct IgnoredBadAlign {
// LOWERING-NEXT:     a: i8,
// LOWERING-NEXT:     b: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, packed)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct NamedPushed {
// LOWERING-NEXT:     a: i8,
// LOWERING-NEXT:     b: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, packed)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct PackedButAligned__packed {
// LOWERING-NEXT:     a: i8,
// LOWERING-NEXT:     b: i32,
// LOWERING-NEXT:     c: i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, align(16))]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct PackedButAligned(PackedButAligned__packed);
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Deref for PackedButAligned {
// LOWERING-NEXT:     type Target = PackedButAligned__packed;
// LOWERING-NEXT:     fn deref(&self) -> &PackedButAligned__packed {
// LOWERING-NEXT:         &self.0
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::DerefMut for PackedButAligned {
// LOWERING-NEXT:     fn deref_mut(&mut self) -> &mut PackedButAligned__packed {
// LOWERING-NEXT:         &mut self.0
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::size_of::<IgnoredBadAlign>() as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::size_of::<NamedPushed>() as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::size_of::<AfterNamedPop>() as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::size_of::<AfterExtraPop>() as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::size_of::<PackedButAligned>() as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::align_of::<PackedButAligned>() as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT: struct AfterExtraPop {
// REWRITES-NEXT:     a: i8,
// REWRITES-NEXT:     b: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct AfterNamedPop {
// REWRITES-NEXT:     a: i8,
// REWRITES-NEXT:     b: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct IgnoredBadAlign {
// REWRITES-NEXT:     a: i8,
// REWRITES-NEXT:     b: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, packed)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct NamedPushed {
// REWRITES-NEXT:     a: i8,
// REWRITES-NEXT:     b: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, packed)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct PackedButAligned__packed {
// REWRITES-NEXT:     a: i8,
// REWRITES-NEXT:     b: i32,
// REWRITES-NEXT:     c: i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, align(16))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct PackedButAligned(PackedButAligned__packed);
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Deref for PackedButAligned {
// REWRITES-NEXT:     type Target = PackedButAligned__packed;
// REWRITES-NEXT:     fn deref(&self) -> &PackedButAligned__packed {
// REWRITES-NEXT:         &self.0
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::DerefMut for PackedButAligned {
// REWRITES-NEXT:     fn deref_mut(&mut self) -> &mut PackedButAligned__packed {
// REWRITES-NEXT:         &mut self.0
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d %d %d %d\n".as_ptr(),
// REWRITES-NEXT:             std::mem::size_of::<IgnoredBadAlign>() as i32,
// REWRITES-NEXT:             std::mem::size_of::<NamedPushed>() as i32,
// REWRITES-NEXT:             std::mem::size_of::<AfterNamedPop>() as i32,
// REWRITES-NEXT:             std::mem::size_of::<AfterExtraPop>() as i32,
// REWRITES-NEXT:             std::mem::size_of::<PackedButAligned>() as i32,
// REWRITES-NEXT:             std::mem::align_of::<PackedButAligned>() as i32,
// REWRITES-NEXT:             1 as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
