/* Test C23 alignment support.  Test valid code.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

#include <stddef.h>

alignas (alignof (max_align_t)) char c;
extern alignas (max_align_t) char c;
extern char c;

extern alignas (max_align_t) short s;
alignas (max_align_t) short s;

alignas (int) int i;
extern int i;

alignas (max_align_t) long l;

alignas (max_align_t) long long ll;

alignas (max_align_t) float f;

alignas (max_align_t) double d;

alignas (max_align_t) _Complex long double cld;

alignas (0) alignas (int) alignas (char) char ca[10];

alignas ((int) alignof (max_align_t) + 0) int x;

enum e { E = alignof (max_align_t) };
alignas (E) int y;

void
func (void)
{
  alignas (max_align_t) long long auto_ll;
}

/* Valid, but useless.  */
alignas (0) struct s; /* { dg-warning "useless" } */

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 __max_align_ll: i64;
// DEFAULT-NEXT:         field1 __max_align_ld: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_max_align_t:[0-9]+]] max_align_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_e:[0-9]+]] e = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_E:[0-9]+]] E = const<i32>(16);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct incomplete;
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i8 [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: i16 [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: i64 [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ll:[0-9]+]] ll: i64 [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: f32 [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: f64 [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cld:[0-9]+]] cld: complex<f80> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ca:[0-9]+]] ca: array<i8, 10> [storage=static] [align=4] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_y:[0-9]+]] y: i32 [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_func:[0-9]+]] @func() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_auto_ll:[0-9]+]] auto_ll: i64 [storage=automatic] [align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
