/* PR tree-optimization/81555 */

unsigned int  a = 1, d = 0xfaeU, e = 0xe376U;
_Bool         b = 0, f = 1;
unsigned char g = 1;

void foo(void) {
  _Bool c = a != b;
  if (c)
    f = 0;
  if (e & c & (unsigned char)d & c)
    g = 0;
}

int main() {
  foo();
  if (f || g != 1)
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
// DEFAULT-NEXT:     global %0 a: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %1 d: u32 [storage=static] = const<u32>(4014) [linkage=external];
// DEFAULT-NEXT:     global %2 e: u32 [storage=static] = const<u32>(58230) [linkage=external];
// DEFAULT-NEXT:     global %3 b: bool [storage=static] = ne<i32, reason=assign>(const<i32>(0), const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %4 f: bool [storage=static] = ne<i32, reason=assign>(const<i32>(1), const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %5 g: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     fn %6 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 c: bool [storage=automatic] = ne<u32>(read<u32>(%0), reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(read<bool>(%3))));
// DEFAULT-NEXT:         if read<bool>(%7)
// DEFAULT-NEXT:             write<bool>(%4, ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if ne<u32>(and<u32>(and<u32>(and<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(read<bool>(%7)))), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(read<u32>(%1)))))), reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(read<bool>(%7)))), const<u32>(0))
// DEFAULT-NEXT:             write<u8>(%5, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%4), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%5))), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
