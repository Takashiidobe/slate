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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct AfterExtraPop {
// LOWERING-NEXT:     a: i8,
// LOWERING-NEXT:     b: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct AfterNamedPop {
// LOWERING-NEXT:     a: i8,
// LOWERING-NEXT:     b: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct IgnoredBadAlign {
// LOWERING-NEXT:     a: i8,
// LOWERING-NEXT:     b: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, packed)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct NamedPushed {
// LOWERING-NEXT:     a: i8,
// LOWERING-NEXT:     b: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, packed)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct PackedButAligned__packed {
// LOWERING-NEXT:     a: i8,
// LOWERING-NEXT:     b: i32,
// LOWERING-NEXT:     c: i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, align(16))]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct PackedButAligned(PackedButAligned__packed);
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Deref for PackedButAligned {
// LOWERING-NEXT:     type Target = PackedButAligned__packed;
// LOWERING-NEXT:     fn deref(&self) -> &PackedButAligned__packed { &self.0 }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::DerefMut for PackedButAligned {
// LOWERING-NEXT:     fn deref_mut(&mut self) -> &mut PackedButAligned__packed { &mut self.0 }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct AfterExtraPop {
// REWRITES-NEXT:     a: i8,
// REWRITES-NEXT:     b: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct AfterNamedPop {
// REWRITES-NEXT:     a: i8,
// REWRITES-NEXT:     b: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct IgnoredBadAlign {
// REWRITES-NEXT:     a: i8,
// REWRITES-NEXT:     b: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, packed)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct NamedPushed {
// REWRITES-NEXT:     a: i8,
// REWRITES-NEXT:     b: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, packed)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct PackedButAligned__packed {
// REWRITES-NEXT:     a: i8,
// REWRITES-NEXT:     b: i32,
// REWRITES-NEXT:     c: i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, align(16))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct PackedButAligned(PackedButAligned__packed);
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Deref for PackedButAligned {
// REWRITES-NEXT:     type Target = PackedButAligned__packed;
// REWRITES-NEXT:     fn deref(&self) -> &PackedButAligned__packed { &self.0 }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::DerefMut for PackedButAligned {
// REWRITES-NEXT:     fn deref_mut(&mut self) -> &mut PackedButAligned__packed { &mut self.0 }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = std::mem::size_of::<IgnoredBadAlign>() as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = std::mem::size_of::<NamedPushed>() as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = std::mem::size_of::<AfterNamedPop>() as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = std::mem::size_of::<AfterExtraPop>() as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = std::mem::size_of::<PackedButAligned>() as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = std::mem::align_of::<PackedButAligned>() as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
