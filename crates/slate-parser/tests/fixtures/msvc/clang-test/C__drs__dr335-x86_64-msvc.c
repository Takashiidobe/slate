/* RUN: %clang_cc1 -std=c89 -triple x86_64-pc-win32 -emit-llvm -o - %s | FileCheck %s
   RUN: %clang_cc1 -std=c89 -triple i686-pc-linux -emit-llvm -o - %s | FileCheck %s
   RUN: %clang_cc1 -std=c99 -triple x86_64-pc-win32 -emit-llvm -o - %s | FileCheck %s
   RUN: %clang_cc1 -std=c99 -triple i686-pc-linux -emit-llvm -o - %s | FileCheck %s
   RUN: %clang_cc1 -std=c11 -triple x86_64-pc-win32 -emit-llvm -o - %s | FileCheck %s
   RUN: %clang_cc1 -std=c11 -triple i686-pc-linux -emit-llvm -o - %s | FileCheck %s
   RUN: %clang_cc1 -std=c17 -triple x86_64-pc-win32 -emit-llvm -o - %s | FileCheck %s
   RUN: %clang_cc1 -std=c17 -triple i686-pc-linux -emit-llvm -o - %s | FileCheck %s
   RUN: %clang_cc1 -std=c2x -triple x86_64-pc-win32 -emit-llvm -o - %s | FileCheck %s
   RUN: %clang_cc1 -std=c2x -triple i686-pc-linux -emit-llvm -o - %s | FileCheck %s
 */

/* WG14 DR335: yes
 * _Bool bit-fields
 *
 * This validates the runtime behavior from the DR, see dr3xx.c for the compile
 * time enforcement portion.
 */
void dr335(void) {
  struct bits_ {
    _Bool bbf1 : 1;
  } bits = { 1 };

  bits.bbf1 = ~bits.bbf1;

  // First, load the value from bits.bbf1 and truncate it down to one-bit.


  // Second, perform the unary complement.


  // Finally, test the new value against 0. If it's nonzero, then assign one
  // into the bit-field, otherwise assign zero into the bit-field. Note, this
  // does not perform the operation on the promoted value, so this matches the
  // requirements in C99 6.3.1.2, so a bit-field of type _Bool behaves like a
  // _Bool and not like an [unsigned] int.
}

// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc
// SLATE-FILECHECK-DEFINES CFG0
// SLATE-FILECHECK-STD CFG0 c17
// SLATE-FILECHECK-DEFINES CFG1
// SLATE-FILECHECK-STD CFG1 c17
// SLATE-FILECHECK-DEFINES CFG2
// SLATE-FILECHECK-STD CFG2 c11
// SLATE-FILECHECK-DEFINES CFG3
// SLATE-FILECHECK-STD CFG3 c17
// SLATE-FILECHECK-DEFINES CFG4
// SLATE-FILECHECK-STD CFG4 c23

// SLATE-FILECHECK-BEGIN CFG0
// CFG0: module {
// CFG0-NEXT:     target "x86_64-pc-windows-msvc" {
// CFG0-NEXT:         endian = little;
// CFG0-NEXT:         pointer [size=8, align=8];
// CFG0-NEXT:         stack_alignment = 16;
// CFG0-NEXT:         long_double = f64;
// CFG0-NEXT:         storage bool [size=1, align=1];
// CFG0-NEXT:         storage i8, u8 [size=1, align=1];
// CFG0-NEXT:         storage i16, u16 [size=2, align=2];
// CFG0-NEXT:         storage i32, u32 [size=4, align=4];
// CFG0-NEXT:         storage i64, u64 [size=8, align=8];
// CFG0-NEXT:         storage i128, u128 [size=16, align=16];
// CFG0-NEXT:         storage bf16 [size=2, align=2];
// CFG0-NEXT:         storage f16 [size=2, align=2];
// CFG0-NEXT:         storage f32 [size=4, align=4];
// CFG0-NEXT:         storage f64 [size=8, align=8];
// CFG0-NEXT:         storage f128 [size=16, align=16];
// CFG0-NEXT:         storage d32 [size=4, align=4];
// CFG0-NEXT:         storage d64 [size=8, align=8];
// CFG0-NEXT:         storage d128 [size=16, align=16];
// CFG0-NEXT:     }
// CFG0-NEXT:     type @type0 bits_ = struct {
// CFG0-NEXT:         field0 bbf1: bool : 1;
// CFG0-NEXT:     } [size=1, align=1, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// CFG0-NEXT:     fn %0 @dr335() -> void [linkage=external] [fallthrough=ret_void] {
// CFG0-NEXT:         let %2 bits: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// CFG0-NEXT:         write<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%2), ne<i32, reason=assign>(not<i32>(from_bool<i32, reason=promotion>(read<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%2)))), const<i32>(0)));
// CFG0-NEXT:     }
// CFG0-NEXT: }
// SLATE-FILECHECK-END CFG0
// SLATE-FILECHECK-BEGIN CFG1
// CFG1: module {
// CFG1-NEXT:     target "x86_64-pc-windows-msvc" {
// CFG1-NEXT:         endian = little;
// CFG1-NEXT:         pointer [size=8, align=8];
// CFG1-NEXT:         stack_alignment = 16;
// CFG1-NEXT:         long_double = f64;
// CFG1-NEXT:         storage bool [size=1, align=1];
// CFG1-NEXT:         storage i8, u8 [size=1, align=1];
// CFG1-NEXT:         storage i16, u16 [size=2, align=2];
// CFG1-NEXT:         storage i32, u32 [size=4, align=4];
// CFG1-NEXT:         storage i64, u64 [size=8, align=8];
// CFG1-NEXT:         storage i128, u128 [size=16, align=16];
// CFG1-NEXT:         storage bf16 [size=2, align=2];
// CFG1-NEXT:         storage f16 [size=2, align=2];
// CFG1-NEXT:         storage f32 [size=4, align=4];
// CFG1-NEXT:         storage f64 [size=8, align=8];
// CFG1-NEXT:         storage f128 [size=16, align=16];
// CFG1-NEXT:         storage d32 [size=4, align=4];
// CFG1-NEXT:         storage d64 [size=8, align=8];
// CFG1-NEXT:         storage d128 [size=16, align=16];
// CFG1-NEXT:     }
// CFG1-NEXT:     type @type0 bits_ = struct {
// CFG1-NEXT:         field0 bbf1: bool : 1;
// CFG1-NEXT:     } [size=1, align=1, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// CFG1-NEXT:     fn %0 @dr335() -> void [linkage=external] [fallthrough=ret_void] {
// CFG1-NEXT:         let %2 bits: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// CFG1-NEXT:         write<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%2), ne<i32, reason=assign>(not<i32>(from_bool<i32, reason=promotion>(read<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%2)))), const<i32>(0)));
// CFG1-NEXT:     }
// CFG1-NEXT: }
// SLATE-FILECHECK-END CFG1
// SLATE-FILECHECK-BEGIN CFG2
// CFG2: module {
// CFG2-NEXT:     target "x86_64-pc-windows-msvc" {
// CFG2-NEXT:         endian = little;
// CFG2-NEXT:         pointer [size=8, align=8];
// CFG2-NEXT:         stack_alignment = 16;
// CFG2-NEXT:         long_double = f64;
// CFG2-NEXT:         storage bool [size=1, align=1];
// CFG2-NEXT:         storage i8, u8 [size=1, align=1];
// CFG2-NEXT:         storage i16, u16 [size=2, align=2];
// CFG2-NEXT:         storage i32, u32 [size=4, align=4];
// CFG2-NEXT:         storage i64, u64 [size=8, align=8];
// CFG2-NEXT:         storage i128, u128 [size=16, align=16];
// CFG2-NEXT:         storage bf16 [size=2, align=2];
// CFG2-NEXT:         storage f16 [size=2, align=2];
// CFG2-NEXT:         storage f32 [size=4, align=4];
// CFG2-NEXT:         storage f64 [size=8, align=8];
// CFG2-NEXT:         storage f128 [size=16, align=16];
// CFG2-NEXT:         storage d32 [size=4, align=4];
// CFG2-NEXT:         storage d64 [size=8, align=8];
// CFG2-NEXT:         storage d128 [size=16, align=16];
// CFG2-NEXT:     }
// CFG2-NEXT:     type @type0 bits_ = struct {
// CFG2-NEXT:         field0 bbf1: bool : 1;
// CFG2-NEXT:     } [size=1, align=1, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// CFG2-NEXT:     fn %0 @dr335() -> void [linkage=external] [fallthrough=ret_void] {
// CFG2-NEXT:         let %2 bits: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// CFG2-NEXT:         write<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%2), ne<i32, reason=assign>(not<i32>(from_bool<i32, reason=promotion>(read<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%2)))), const<i32>(0)));
// CFG2-NEXT:     }
// CFG2-NEXT: }
// SLATE-FILECHECK-END CFG2
// SLATE-FILECHECK-BEGIN CFG3
// CFG3: module {
// CFG3-NEXT:     target "x86_64-pc-windows-msvc" {
// CFG3-NEXT:         endian = little;
// CFG3-NEXT:         pointer [size=8, align=8];
// CFG3-NEXT:         stack_alignment = 16;
// CFG3-NEXT:         long_double = f64;
// CFG3-NEXT:         storage bool [size=1, align=1];
// CFG3-NEXT:         storage i8, u8 [size=1, align=1];
// CFG3-NEXT:         storage i16, u16 [size=2, align=2];
// CFG3-NEXT:         storage i32, u32 [size=4, align=4];
// CFG3-NEXT:         storage i64, u64 [size=8, align=8];
// CFG3-NEXT:         storage i128, u128 [size=16, align=16];
// CFG3-NEXT:         storage bf16 [size=2, align=2];
// CFG3-NEXT:         storage f16 [size=2, align=2];
// CFG3-NEXT:         storage f32 [size=4, align=4];
// CFG3-NEXT:         storage f64 [size=8, align=8];
// CFG3-NEXT:         storage f128 [size=16, align=16];
// CFG3-NEXT:         storage d32 [size=4, align=4];
// CFG3-NEXT:         storage d64 [size=8, align=8];
// CFG3-NEXT:         storage d128 [size=16, align=16];
// CFG3-NEXT:     }
// CFG3-NEXT:     type @type0 bits_ = struct {
// CFG3-NEXT:         field0 bbf1: bool : 1;
// CFG3-NEXT:     } [size=1, align=1, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// CFG3-NEXT:     fn %0 @dr335() -> void [linkage=external] [fallthrough=ret_void] {
// CFG3-NEXT:         let %2 bits: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// CFG3-NEXT:         write<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%2), ne<i32, reason=assign>(not<i32>(from_bool<i32, reason=promotion>(read<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%2)))), const<i32>(0)));
// CFG3-NEXT:     }
// CFG3-NEXT: }
// SLATE-FILECHECK-END CFG3
// SLATE-FILECHECK-BEGIN CFG4
// CFG4: module {
// CFG4-NEXT:     target "x86_64-pc-windows-msvc" {
// CFG4-NEXT:         endian = little;
// CFG4-NEXT:         pointer [size=8, align=8];
// CFG4-NEXT:         stack_alignment = 16;
// CFG4-NEXT:         long_double = f64;
// CFG4-NEXT:         storage bool [size=1, align=1];
// CFG4-NEXT:         storage i8, u8 [size=1, align=1];
// CFG4-NEXT:         storage i16, u16 [size=2, align=2];
// CFG4-NEXT:         storage i32, u32 [size=4, align=4];
// CFG4-NEXT:         storage i64, u64 [size=8, align=8];
// CFG4-NEXT:         storage i128, u128 [size=16, align=16];
// CFG4-NEXT:         storage bf16 [size=2, align=2];
// CFG4-NEXT:         storage f16 [size=2, align=2];
// CFG4-NEXT:         storage f32 [size=4, align=4];
// CFG4-NEXT:         storage f64 [size=8, align=8];
// CFG4-NEXT:         storage f128 [size=16, align=16];
// CFG4-NEXT:         storage d32 [size=4, align=4];
// CFG4-NEXT:         storage d64 [size=8, align=8];
// CFG4-NEXT:         storage d128 [size=16, align=16];
// CFG4-NEXT:     }
// CFG4-NEXT:     type @type0 bits_ = struct {
// CFG4-NEXT:         field0 bbf1: bool : 1;
// CFG4-NEXT:     } [size=1, align=1, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// CFG4-NEXT:     fn %0 @dr335() -> void [linkage=external] [fallthrough=ret_void] {
// CFG4-NEXT:         let %2 bits: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// CFG4-NEXT:         write<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%2), ne<i32, reason=assign>(not<i32>(from_bool<i32, reason=promotion>(read<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%2)))), const<i32>(0)));
// CFG4-NEXT:     }
// CFG4-NEXT: }
// SLATE-FILECHECK-END CFG4
