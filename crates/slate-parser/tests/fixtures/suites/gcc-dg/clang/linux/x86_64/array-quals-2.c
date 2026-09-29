/* Test that pointers to arrays of differently qualified types aren't
   permitted in conditional expressions, and that qualifiers aren't
   lost in forming composite types.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do compile } */
/* { dg-options "-std=gnu17 -pedantic -Wno-discarded-array-qualifiers" } */
typedef const char T[1];
typedef const char T2[1];
typedef volatile char U[1];
T *p;
T2 *p2;
U *q;
void *f(void) { return 1 ? p : q; } /* { dg-warning "pointers to arrays with different qualifiers" } */
T *g(void) { return 1 ? p : p2; }

// SLATE-FILECHECK-STD DEFAULT gnu17
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
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = array<i8, 1>;
// DEFAULT-NEXT:     type @type[[TYPE_T2:[0-9]+]] T2 = array<i8, 1>;
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = array<i8, 1>;
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<const array<i8, 1>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p2:[0-9]+]] p2: ptr<const array<i8, 1>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: ptr<volatile array<i8, 1>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(conditional<ptr<const volatile array<i8, 1>>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<const volatile array<i8, 1>>, reason=usual_arith>(read<ptr<const array<i8, 1>>>(%[[VALUE_p]])), pointer_cast<ptr<const volatile array<i8, 1>>, reason=usual_arith>(read<ptr<volatile array<i8, 1>>>(%[[VALUE_q]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> ptr<const array<i8, 1>> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<const array<i8, 1>>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<const array<i8, 1>>>(%[[VALUE_p]]), read<ptr<const array<i8, 1>>>(%[[VALUE_p2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
