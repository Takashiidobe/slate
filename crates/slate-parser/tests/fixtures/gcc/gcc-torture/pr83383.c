/* PR tree-optimization/83383 */

unsigned long long int a    = 16ULL;
unsigned char          b    = 195;
unsigned long long int c    = ~0ULL;
unsigned char          d    = 1;
unsigned long long int e[2] = {3625445792498952486ULL, 0};
unsigned long long int f[2] = {0, 8985037393681294663ULL};
unsigned long long int g    = 5052410635626804928ULL;

void foo() {
  a = ((signed char)a) < b;
  c = (d ? e[0] : 0) - (f[1] * a ? 1 : g);
}

int main() {
  foo();
  if (a != 1 || c != 3625445792498952485ULL)
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
// DEFAULT-NEXT:     global %0 a: u64 [storage=static] = const<u64>(16) [linkage=external];
// DEFAULT-NEXT:     global %1 b: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(195))) [linkage=external];
// DEFAULT-NEXT:     global %2 c: u64 [storage=static] = not<u64>(const<u64>(0)) [linkage=external];
// DEFAULT-NEXT:     global %3 d: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %4 e: array<u64, 2> [storage=static] [align=16] = aggregate<array<u64, 2>, zero_fill=false>(index0 = const<u64>(3625445792498952486), index1 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     global %5 f: array<u64, 2> [storage=static] [align=16] = aggregate<array<u64, 2>, zero_fill=false>(index0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))), index1 = const<u64>(8985037393681294663)) [linkage=external];
// DEFAULT-NEXT:     global %6 g: u64 [storage=static] = const<u64>(5052410635626804928) [linkage=external];
// DEFAULT-NEXT:     fn %7 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u64>(%0, from_bool<u64, reason=assign>(lt<i32>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(read<u64>(%0)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1))))));
// DEFAULT-NEXT:         write<u64>(%2, sub<u64, overflow=wrap>(conditional<u64>(ne<u8>(read<u8>(%3), const<u8>(0)), read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%4), const<i32>(0)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), conditional<u64>(ne<u64>(mul<u64, overflow=wrap>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%5), const<i32>(1)))), read<u64>(%0)), const<u64>(0)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), read<u64>(%6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(read<u64>(%0), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), ne<u64>(read<u64>(%2), const<u64>(3625445792498952485)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
