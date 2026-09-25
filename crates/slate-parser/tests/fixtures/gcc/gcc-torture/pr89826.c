typedef unsigned int       u32;
typedef unsigned long long u64;
u64                        a;
u32                        b;

u64 foo(u32 d) {
  a -= d ? 0 : ~a;
  return a + b;
}

int main(void) {
  u64 x = foo(2);
  if (x != 0)
    __builtin_abort();
  return 0;
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
// DEFAULT-NEXT:     type @type0 u32 = u32;
// DEFAULT-NEXT:     type @type1 u64 = u64;
// DEFAULT-NEXT:     global %2 a: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 b: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo(%5 d: u32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8: u64 [synthetic] = read<u64>(%2);
// DEFAULT-NEXT:         let %9: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%8), conditional<u64>(ne<u32>(read<u32>(%5), const<u32>(0)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), not<u64>(read<u64>(%2))));
// DEFAULT-NEXT:         write<u64>(%2, read<u64>(%9));
// DEFAULT-NEXT:         return add<u64, overflow=wrap>(read<u64>(%2), widen<u64, reason=usual_arith>(read<u32>(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 x: u64 [storage=automatic] = call<u64, signature=fn(u32) -> u64>(%4, reinterpret<u32, reason=arg, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%7), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
