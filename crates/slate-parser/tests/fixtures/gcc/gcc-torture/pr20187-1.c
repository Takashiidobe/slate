int a = 0x101;
int b = 0x100;

int test(void) {
  return (((unsigned char)(unsigned long long)((a ? a : 1) & (a * b))) ? 0 : 1);
}

int main(void) { return 1 - test(); }


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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] = const<i32>(257) [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] = const<i32>(256) [linkage=external];
// DEFAULT-NEXT:     fn %2 @test() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<u8>(truncate<u8, reason=explicit, fits=unknown>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(and<i32>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32>(%0), const<i32>(1)), mul<i32, overflow=ub>(read<i32>(%0), read<i32>(%1)))))), const<u8>(0)), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(const<i32>(1), call<i32, signature=fn() -> i32>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
