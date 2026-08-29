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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct NaturalAfter {
// LOWERING-NEXT:     tag: u8,
// LOWERING-NEXT:     value: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct NaturalBefore {
// LOWERING-NEXT:     tag: u8,
// LOWERING-NEXT:     value: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, packed)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct PackedOne {
// LOWERING-NEXT:     tag: u8,
// LOWERING-NEXT:     value: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, packed(2))]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct PackedTwo {
// LOWERING-NEXT:     tag: u8,
// LOWERING-NEXT:     value: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, packed(2))]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct PackedTwoAgain {
// LOWERING-NEXT:     tag: u8,
// LOWERING-NEXT:     value: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut packed: PackedOne = PackedOne { tag: 0, value: 0 };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     packed = PackedOne { tag: 29, value: 31 };
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = std::mem::size_of::<NaturalBefore>() as i32;
// LOWERING-NEXT:     let _v3: i32 = std::mem::offset_of!(NaturalBefore, value) as i32;
// LOWERING-NEXT:     let _v4: i32 = std::mem::size_of::<PackedTwo>() as i32;
// LOWERING-NEXT:     let _v5: i32 = std::mem::align_of::<PackedTwo>() as i32;
// LOWERING-NEXT:     let _v6: i32 = std::mem::offset_of!(PackedTwo, value) as i32;
// LOWERING-NEXT:     let _v7: i32 = 5;
// LOWERING-NEXT:     let _v8: i32 = std::mem::align_of::<PackedOne>() as i32;
// LOWERING-NEXT:     let _v9: i32 = std::mem::offset_of!(PackedOne, value) as i32;
// LOWERING-NEXT:     let _v10: i32 = std::mem::size_of::<PackedTwoAgain>() as i32;
// LOWERING-NEXT:     let _v11: i32 = std::mem::offset_of!(PackedTwoAgain, value) as i32;
// LOWERING-NEXT:     let _v12: i32 = std::mem::offset_of!(NaturalAfter, value) as i32;
// LOWERING-NEXT:     let _v13: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4, _v5, _v6, _v7, _v8, _v9, _v10, _v11, _v12) };
// LOWERING-NEXT:     let _v14: u8 = packed.tag;
// LOWERING-NEXT:     let _v15: i32 = _v14 as i32;
// LOWERING-NEXT:     let _v16: i32 = 29;
// LOWERING-NEXT:     let _v17: bool = _v15 == _v16;
// LOWERING-NEXT:     let _v18: bool = if _v17 {
// LOWERING-NEXT:         let _v19: u32 = packed.value;
// LOWERING-NEXT:         let _v20: u32 = 31;
// LOWERING-NEXT:         let _v21: bool = _v19 == _v20;
// LOWERING-NEXT:         _v21
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v22: bool = false;
// LOWERING-NEXT:         _v22
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v23: i32 = 0;
// LOWERING-NEXT:     let _v24: i32 = 1;
// LOWERING-NEXT:     let _v25: i32 = if _v18 { _v23 } else { _v24 };
// LOWERING-NEXT:     __retval = _v25;
// LOWERING-NEXT:     let _v26: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v26 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct NaturalAfter {
// REWRITES-NEXT:     tag: u8,
// REWRITES-NEXT:     value: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct NaturalBefore {
// REWRITES-NEXT:     tag: u8,
// REWRITES-NEXT:     value: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, packed)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct PackedOne {
// REWRITES-NEXT:     tag: u8,
// REWRITES-NEXT:     value: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, packed(2))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct PackedTwo {
// REWRITES-NEXT:     tag: u8,
// REWRITES-NEXT:     value: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, packed(2))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct PackedTwoAgain {
// REWRITES-NEXT:     tag: u8,
// REWRITES-NEXT:     value: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut packed: PackedOne = PackedOne { tag: 0, value: 0 };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: packed = PackedOne { tag: 29, value: 31 };
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = std::mem::size_of::<NaturalBefore>() as i32;
// REWRITES-NEXT: let _v3: i32 = std::mem::offset_of!(NaturalBefore, value) as i32;
// REWRITES-NEXT: let _v4: i32 = std::mem::size_of::<PackedTwo>() as i32;
// REWRITES-NEXT: let _v5: i32 = std::mem::align_of::<PackedTwo>() as i32;
// REWRITES-NEXT: let _v6: i32 = std::mem::offset_of!(PackedTwo, value) as i32;
// REWRITES-NEXT: let _v7: i32 = 5;
// REWRITES-NEXT: let _v8: i32 = std::mem::align_of::<PackedOne>() as i32;
// REWRITES-NEXT: let _v9: i32 = std::mem::offset_of!(PackedOne, value) as i32;
// REWRITES-NEXT: let _v10: i32 = std::mem::size_of::<PackedTwoAgain>() as i32;
// REWRITES-NEXT: let _v11: i32 = std::mem::offset_of!(PackedTwoAgain, value) as i32;
// REWRITES-NEXT: let _v12: i32 = std::mem::offset_of!(NaturalAfter, value) as i32;
// REWRITES-NEXT: let _v13: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4, _v5, _v6, _v7, _v8, _v9, _v10, _v11, _v12) };
// REWRITES-NEXT: let _v16: i32 = 29;
// REWRITES-NEXT: let _v18: bool = if (packed.tag as i32) == _v16 {
// REWRITES-NEXT:         let _v20: u32 = 31;
// REWRITES-NEXT:         let _v21: bool = packed.value == _v20;
// REWRITES-NEXT:     _v21
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v22: bool = false;
// REWRITES-NEXT:     _v22
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v23: i32 = 0;
// REWRITES-NEXT: let _v24: i32 = 1;
// REWRITES-NEXT: __retval = if _v18 { _v23 } else { _v24 };
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
