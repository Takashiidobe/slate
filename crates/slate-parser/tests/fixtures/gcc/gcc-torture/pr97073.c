/* PR middle-end/97073 */
/* { dg-additional-options "-mno-stv" { target i?86-*-* x86_64-*-* } } */

typedef unsigned long long L;
union U {
  L i;
  struct T {
    unsigned k;
    L        l;
  } j;
} u;

__attribute__((noinline, noclone)) void foo(L x) { u.j.l = u.i & x; }

int main() {
  u.i = 5;
  foo(-1ULL);
  if (u.j.l != 5)
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
// DEFAULT-NEXT:     type @type0 L = u64;
// DEFAULT-NEXT:     type @type1 U = union {
// DEFAULT-NEXT:         field0 i: u64;
// DEFAULT-NEXT:         field1 j: @type2;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type2 T = struct {
// DEFAULT-NEXT:         field0 k: u32;
// DEFAULT-NEXT:         field1 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %3 u: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo(%5 x: u64) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u64>(field1(field1(%3)), and<u64>(read<u64>(field0(%3)), read<u64>(%5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<u64>(field0(%3), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(5))));
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%4, neg<u64, overflow=wrap>(const<u64>(1)));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(field1(field1(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
