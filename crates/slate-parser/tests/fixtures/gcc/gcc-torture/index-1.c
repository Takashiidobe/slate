/* { dg-skip-if "strict reloc overflow checking" { msp430-*-* } { "*" } {
 * "-mcpu=msp430" "-mlarge"} } */

void abort(void);
void exit(int);

int a[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13,
           14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
           28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39};

int f(long n) { return a[n - 100000]; }

int main(void) {
  if (f(100030L) != 30)
    abort();
  exit(0);
}


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
// DEFAULT-NEXT:     global %2 a: array<i32, 40> [storage=static] = aggregate<array<i32, 40>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2), index3 = const<i32>(3), index4 = const<i32>(4), index5 = const<i32>(5), index6 = const<i32>(6), index7 = const<i32>(7), index8 = const<i32>(8), index9 = const<i32>(9), index10 = const<i32>(10), index11 = const<i32>(11), index12 = const<i32>(12), index13 = const<i32>(13), index14 = const<i32>(14), index15 = const<i32>(15), index16 = const<i32>(16), index17 = const<i32>(17), index18 = const<i32>(18), index19 = const<i32>(19), index20 = const<i32>(20), index21 = const<i32>(21), index22 = const<i32>(22), index23 = const<i32>(23), index24 = const<i32>(24), index25 = const<i32>(25), index26 = const<i32>(26), index27 = const<i32>(27), index28 = const<i32>(28), index29 = const<i32>(29), index30 = const<i32>(30), index31 = const<i32>(31), index32 = const<i32>(32), index33 = const<i32>(33), index34 = const<i32>(34), index35 = const<i32>(35), index36 = const<i32>(36), index37 = const<i32>(37), index38 = const<i32>(38), index39 = const<i32>(39)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%6 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @f(%4 n: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(40)>(%2), sub<i64, overflow=ub>(read<i64>(%4), widen<i64, reason=usual_arith>(const<i32>(100000))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%3, const<i64>(100030)), const<i32>(30))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
