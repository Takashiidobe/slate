/* PR c/122982 */ 
/* { dg-do compile } */
/* { dg-options "-O0" } */

int* f (int);

struct __bounded_ptr {
 int k;
 int *buf __attribute__ ((counted_by (k)));
};

int*
f1 (int n) { return f (n); }

void h1 (void)
{ 
  int *p = (struct __bounded_ptr) {3, f1 (3)}.buf;
  __builtin_memset (p, 0, 3 * sizeof p);
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type0 __bounded_ptr = struct {
// DEFAULT-NEXT:         field0 k: i32;
// DEFAULT-NEXT:         field1 buf: ptr<i32>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %0 @f(%6 <unnamed>: i32) -> ptr<i32> [linkage=external];
// DEFAULT-NEXT:     fn %2 @f1(%3 n: i32) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%0, read<i32>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @__builtin_memset(%8 <unnamed>: ptr<void>, %9 <unnamed>: i32, %10 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @h1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 p: ptr<i32> [storage=automatic] = read<ptr<i32>>(field1(compound_literal %7 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(3), field1 = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%2, const<i32>(3)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%11, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%5)), const<i32>(0), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
