/* PR tree-optimization/65170 */

#ifdef __SIZEOF_INT128__
typedef unsigned __int128      V;
typedef unsigned long long int H;
#else
typedef unsigned long long int V;
typedef unsigned int           H;
#endif

__attribute__((noinline, noclone)) void foo(V b, V c) {
  V a;
  b &= (H)-1;
  c &= (H)-1;
  a  = b * c;
  if (a != 1)
    __builtin_abort();
}

int main() {
  foo(1, 1);
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
// DEFAULT-NEXT:     type @type0 V = u128;
// DEFAULT-NEXT:     type @type1 H = u64;
// DEFAULT-NEXT:     fn %7 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @foo(%3 b: u128, %4 c: u128) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 a: u128 [storage=automatic];
// DEFAULT-NEXT:         let %8: u128 [synthetic] = read<u128>(%3);
// DEFAULT-NEXT:         let %9: u128 [synthetic] = and<u128>(read<u128>(%8), widen<u128, reason=usual_arith>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))));
// DEFAULT-NEXT:         write<u128>(%3, read<u128>(%9));
// DEFAULT-NEXT:         let %10: u128 [synthetic] = read<u128>(%4);
// DEFAULT-NEXT:         let %11: u128 [synthetic] = and<u128>(read<u128>(%10), widen<u128, reason=usual_arith>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))));
// DEFAULT-NEXT:         write<u128>(%4, read<u128>(%11));
// DEFAULT-NEXT:         write<u128>(%5, mul<u128, overflow=wrap>(read<u128>(%3), read<u128>(%4)));
// DEFAULT-NEXT:         if ne<u128>(read<u128>(%5), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(u128, u128) -> void>(%2, reinterpret<u128, reason=arg, fits=unknown>(widen<i128, reason=arg>(const<i32>(1))), reinterpret<u128, reason=arg, fits=unknown>(widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
