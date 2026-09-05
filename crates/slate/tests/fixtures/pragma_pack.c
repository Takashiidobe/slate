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
// COMMON-LOWERING-NEXT: struct NaturalAfter {
// COMMON-LOWERING-NEXT:     tag: u8,
// COMMON-LOWERING-NEXT:     value: u32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct NaturalBefore {
// COMMON-LOWERING-NEXT:     tag: u8,
// COMMON-LOWERING-NEXT:     value: u32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C, packed)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct PackedOne {
// COMMON-LOWERING-NEXT:     tag: u8,
// COMMON-LOWERING-NEXT:     value: u32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C, packed(2))]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct PackedTwo {
// COMMON-LOWERING-NEXT:     tag: u8,
// COMMON-LOWERING-NEXT:     value: u32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C, packed(2))]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct PackedTwoAgain {
// COMMON-LOWERING-NEXT:     tag: u8,
// COMMON-LOWERING-NEXT:     value: u32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut packed: PackedOne = PackedOne { tag: 0, value: 0 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: PackedOne = PackedOne { tag: 29, value: 31 };
// COMMON-LOWERING-NEXT:     packed = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::size_of::<NaturalBefore>() as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::offset_of!(NaturalBefore, value) as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::size_of::<PackedTwo>() as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::align_of::<PackedTwo>() as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::offset_of!(PackedTwo, value) as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::align_of::<PackedOne>() as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::offset_of!(PackedOne, value) as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::size_of::<PackedTwoAgain>() as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::offset_of!(PackedTwoAgain, value) as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::offset_of!(NaturalAfter, value) as i32;
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
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = packed.tag;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 29;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u32 = packed.value;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u32 = 31;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: struct NaturalAfter {
// COMMON-REWRITES-NEXT:     tag: u8,
// COMMON-REWRITES-NEXT:     value: u32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct NaturalBefore {
// COMMON-REWRITES-NEXT:     tag: u8,
// COMMON-REWRITES-NEXT:     value: u32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C, packed)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct PackedOne {
// COMMON-REWRITES-NEXT:     tag: u8,
// COMMON-REWRITES-NEXT:     value: u32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C, packed(2))]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct PackedTwo {
// COMMON-REWRITES-NEXT:     tag: u8,
// COMMON-REWRITES-NEXT:     value: u32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C, packed(2))]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct PackedTwoAgain {
// COMMON-REWRITES-NEXT:     tag: u8,
// COMMON-REWRITES-NEXT:     value: u32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut packed: PackedOne = PackedOne { tag: 0, value: 0 };
// COMMON-REWRITES-NEXT:     packed = PackedOne { tag: 29, value: 31 };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d %d %d %d %d %d %d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             std::mem::size_of::<NaturalBefore>() as i32,
// COMMON-REWRITES-NEXT:             std::mem::offset_of!(NaturalBefore, value) as i32,
// COMMON-REWRITES-NEXT:             std::mem::size_of::<PackedTwo>() as i32,
// COMMON-REWRITES-NEXT:             std::mem::align_of::<PackedTwo>() as i32,
// COMMON-REWRITES-NEXT:             std::mem::offset_of!(PackedTwo, value) as i32,
// COMMON-REWRITES-NEXT:             5 as i32,
// COMMON-REWRITES-NEXT:             std::mem::align_of::<PackedOne>() as i32,
// COMMON-REWRITES-NEXT:             std::mem::offset_of!(PackedOne, value) as i32,
// COMMON-REWRITES-NEXT:             std::mem::size_of::<PackedTwoAgain>() as i32,
// COMMON-REWRITES-NEXT:             std::mem::offset_of!(PackedTwoAgain, value) as i32,
// COMMON-REWRITES-NEXT:             std::mem::offset_of!(NaturalAfter, value) as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if (packed.tag as i32) == 29 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = packed.value == 31;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
