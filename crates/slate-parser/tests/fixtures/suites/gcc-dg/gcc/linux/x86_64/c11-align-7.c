/* Test C11 alignment support.  Test code valid after the resolution
   of DR#444: alignment specifiers for struct and union members and
   compound literals.  */
/* { dg-do compile } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <stddef.h>

struct s
{
  _Alignas (_Alignof (max_align_t)) char c;
};

union u
{
  _Alignas (_Alignof (max_align_t)) char c;
};

char *p = &(_Alignas (_Alignof (max_align_t)) char) { 1 };
size_t size = sizeof (_Alignas (_Alignof (max_align_t)) char) { 1 };

// SLATE-FILECHECK-STD DEFAULT c11
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 __max_align_ll: i64;
// DEFAULT-NEXT:         field1 __max_align_ld: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_max_align_t:[0-9]+]] max_align_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=static] = addr_of<ptr<i8>>(compound_literal %[[VALUE0:[0-9]+]] [storage=static] [align=16] = truncate<i8, reason=assign, fits=always>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_size:[0-9]+]] size: u64 [storage=static] = const<u64>(1) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
