#include <stddef.h>
#include <stdio.h>

struct NaturalBefore {
  unsigned char tag;
  unsigned int  value;
};

#pragma pack(push, 2)
struct PackedTwo {
  unsigned char tag;
  unsigned int  value;
};

#pragma pack(push, 1)
struct PackedOne {
  unsigned char tag;
  unsigned int  value;
};
#pragma pack(pop)

struct PackedTwoAgain {
  unsigned char tag;
  unsigned int  value;
};
#pragma pack(pop)

struct NaturalAfter {
  unsigned char tag;
  unsigned int  value;
};

int main(void) {
  struct PackedOne packed = {29, 31};
  printf(
      "%d %d %d %d %d %d %d %d %d %d %d\n", (int)sizeof(struct NaturalBefore),
      (int)offsetof(struct NaturalBefore, value), (int)sizeof(struct PackedTwo),
      (int)_Alignof(struct PackedTwo), (int)offsetof(struct PackedTwo, value),
      (int)sizeof(packed), (int)_Alignof(struct PackedOne),
      (int)offsetof(struct PackedOne, value),
      (int)sizeof(struct PackedTwoAgain),
      (int)offsetof(struct PackedTwoAgain, value),
      (int)offsetof(struct NaturalAfter, value));
  return packed.tag == 29 && packed.value == 31 ? 0 : 1;
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
// LOWERING-NEXT: struct NaturalAfter {
// LOWERING-NEXT:     tag: u8,
// LOWERING-NEXT:     value: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct NaturalBefore {
// LOWERING-NEXT:     tag: u8,
// LOWERING-NEXT:     value: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, packed)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct PackedOne {
// LOWERING-NEXT:     tag: u8,
// LOWERING-NEXT:     value: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, packed(2))]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct PackedTwo {
// LOWERING-NEXT:     tag: u8,
// LOWERING-NEXT:     value: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, packed(2))]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct PackedTwoAgain {
// LOWERING-NEXT:     tag: u8,
// LOWERING-NEXT:     value: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut packed: PackedOne = PackedOne { tag: 0, value: 0 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     packed = PackedOne { tag: 29, value: 31 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::size_of::<NaturalBefore>() as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::offset_of!(NaturalBefore, value) as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::size_of::<PackedTwo>() as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::align_of::<PackedTwo>() as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::offset_of!(PackedTwo, value) as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::align_of::<PackedOne>() as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::offset_of!(PackedOne, value) as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::size_of::<PackedTwoAgain>() as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::offset_of!(PackedTwoAgain, value) as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::offset_of!(NaturalAfter, value) as i32;
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
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = packed.tag;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 29;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u32 = packed.value;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u32 = 31;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
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
// REWRITES-NEXT: struct NaturalAfter {
// REWRITES-NEXT:     tag: u8,
// REWRITES-NEXT:     value: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct NaturalBefore {
// REWRITES-NEXT:     tag: u8,
// REWRITES-NEXT:     value: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, packed)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct PackedOne {
// REWRITES-NEXT:     tag: u8,
// REWRITES-NEXT:     value: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, packed(2))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct PackedTwo {
// REWRITES-NEXT:     tag: u8,
// REWRITES-NEXT:     value: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, packed(2))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct PackedTwoAgain {
// REWRITES-NEXT:     tag: u8,
// REWRITES-NEXT:     value: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut packed: PackedOne = PackedOne { tag: 29, value: 31 };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d %d %d %d %d %d %d %d\n".as_ptr(),
// REWRITES-NEXT:             std::mem::size_of::<NaturalBefore>() as i32,
// REWRITES-NEXT:             std::mem::offset_of!(NaturalBefore, value) as i32,
// REWRITES-NEXT:             std::mem::size_of::<PackedTwo>() as i32,
// REWRITES-NEXT:             std::mem::align_of::<PackedTwo>() as i32,
// REWRITES-NEXT:             std::mem::offset_of!(PackedTwo, value) as i32,
// REWRITES-NEXT:             5 as i32,
// REWRITES-NEXT:             std::mem::align_of::<PackedOne>() as i32,
// REWRITES-NEXT:             std::mem::offset_of!(PackedOne, value) as i32,
// REWRITES-NEXT:             std::mem::size_of::<PackedTwoAgain>() as i32,
// REWRITES-NEXT:             std::mem::offset_of!(PackedTwoAgain, value) as i32,
// REWRITES-NEXT:             std::mem::offset_of!(NaturalAfter, value) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 29;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if (packed.tag as i32) == {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: u32 = 31;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = packed.value == {{_v[0-9]+}};
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// REWRITES-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
