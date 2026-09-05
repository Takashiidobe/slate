#include <stddef.h>
#include <stdio.h>

#pragma pack(3)
struct IgnoredBadAlign {
  char a;
  int  b;
};
#pragma pack()

#pragma pack(push, lbl, 1)
struct NamedPushed {
  char a;
  int  b;
};
#pragma pack(pop, lbl)

struct AfterNamedPop {
  char a;
  int  b;
};

#pragma pack(pop)
struct AfterExtraPop {
  char a;
  int  b;
};

#pragma pack(push, 1)
struct __attribute__((aligned(16))) PackedButAligned {
  char a;
  int  b;
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
// COMMON-LOWERING-NEXT: struct AfterExtraPop {
// COMMON-LOWERING-NEXT:     b: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct AfterNamedPop {
// COMMON-LOWERING-NEXT:     b: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct IgnoredBadAlign {
// COMMON-LOWERING-NEXT:     b: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C, packed)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct NamedPushed {
// COMMON-LOWERING-NEXT:     b: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C, packed)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct PackedButAligned__packed {
// COMMON-LOWERING-NEXT:     b: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C, align(16))]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct PackedButAligned(PackedButAligned__packed);
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: impl core::ops::Deref for PackedButAligned {
// COMMON-LOWERING-NEXT:     type Target = PackedButAligned__packed;
// COMMON-LOWERING-NEXT:     fn deref(&self) -> &PackedButAligned__packed {
// COMMON-LOWERING-NEXT:         &self.0
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: impl core::ops::DerefMut for PackedButAligned {
// COMMON-LOWERING-NEXT:     fn deref_mut(&mut self) -> &mut PackedButAligned__packed {
// COMMON-LOWERING-NEXT:         &mut self.0
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::size_of::<IgnoredBadAlign>() as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::size_of::<NamedPushed>() as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::size_of::<AfterNamedPop>() as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::size_of::<AfterExtraPop>() as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::size_of::<PackedButAligned>() as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::align_of::<PackedButAligned>() as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
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
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     a: i8,
// LOWERING-X86_64-GNU-NEXT:     a: i8,
// LOWERING-X86_64-GNU-NEXT:     a: i8,
// LOWERING-X86_64-GNU-NEXT:     a: i8,
// LOWERING-X86_64-GNU-NEXT:     a: i8,
// LOWERING-X86_64-GNU-NEXT:     c: i8,
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     a: u8,
// LOWERING-AARCH64-GNU-NEXT:     a: u8,
// LOWERING-AARCH64-GNU-NEXT:     a: u8,
// LOWERING-AARCH64-GNU-NEXT:     a: u8,
// LOWERING-AARCH64-GNU-NEXT:     a: u8,
// LOWERING-AARCH64-GNU-NEXT:     c: u8,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d %d %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: struct AfterExtraPop {
// COMMON-REWRITES-NEXT:     b: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct AfterNamedPop {
// COMMON-REWRITES-NEXT:     b: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct IgnoredBadAlign {
// COMMON-REWRITES-NEXT:     b: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C, packed)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct NamedPushed {
// COMMON-REWRITES-NEXT:     b: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C, packed)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct PackedButAligned__packed {
// COMMON-REWRITES-NEXT:     b: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C, align(16))]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct PackedButAligned(PackedButAligned__packed);
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: impl core::ops::Deref for PackedButAligned {
// COMMON-REWRITES-NEXT:     type Target = PackedButAligned__packed;
// COMMON-REWRITES-NEXT:     fn deref(&self) -> &PackedButAligned__packed {
// COMMON-REWRITES-NEXT:         &self.0
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: impl core::ops::DerefMut for PackedButAligned {
// COMMON-REWRITES-NEXT:     fn deref_mut(&mut self) -> &mut PackedButAligned__packed {
// COMMON-REWRITES-NEXT:         &mut self.0
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d %d %d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             std::mem::size_of::<IgnoredBadAlign>() as i32,
// COMMON-REWRITES-NEXT:             std::mem::size_of::<NamedPushed>() as i32,
// COMMON-REWRITES-NEXT:             std::mem::size_of::<AfterNamedPop>() as i32,
// COMMON-REWRITES-NEXT:             std::mem::size_of::<AfterExtraPop>() as i32,
// COMMON-REWRITES-NEXT:             std::mem::size_of::<PackedButAligned>() as i32,
// COMMON-REWRITES-NEXT:             std::mem::align_of::<PackedButAligned>() as i32,
// COMMON-REWRITES-NEXT:             1 as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     a: i8,
// REWRITES-X86_64-GNU-NEXT:     a: i8,
// REWRITES-X86_64-GNU-NEXT:     a: i8,
// REWRITES-X86_64-GNU-NEXT:     a: i8,
// REWRITES-X86_64-GNU-NEXT:     a: i8,
// REWRITES-X86_64-GNU-NEXT:     c: i8,
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     a: u8,
// REWRITES-AARCH64-GNU-NEXT:     a: u8,
// REWRITES-AARCH64-GNU-NEXT:     a: u8,
// REWRITES-AARCH64-GNU-NEXT:     a: u8,
// REWRITES-AARCH64-GNU-NEXT:     a: u8,
// REWRITES-AARCH64-GNU-NEXT:     c: u8,
// SLATE-FILECHECK-END rewrites-aarch64-gnu
