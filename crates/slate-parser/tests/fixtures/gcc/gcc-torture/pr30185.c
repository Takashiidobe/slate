/* PR target/30185 */

extern void abort(void);

typedef struct S {
  char      a;
  long long b;
} S;

S foo(S x, S y) {
  S z;
  z.b = x.b / y.b;
  return z;
}

int main(void) {
  S a, b;
  a.b = 32LL;
  b.b = 4LL;
  if (foo(a, b).b != 8LL)
    abort();
  a.b = -8LL;
  b.b = -2LL;
  if (foo(a, b).b != 4LL)
    abort();
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 S = @type0;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 x: @type0, %5 y: @type0) -> @type0 [linkage=external] [abi=sysv64(coerce<i8, i64>, coerce<i8, i64>) -> coerce<i8, i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 z: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(field1(%6), div<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(field1(%4)), read<i64>(field1(%5))));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %9 b: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(field1(%8), const<i64>(32));
// DEFAULT-NEXT:         write<i64>(field1(%9), const<i64>(4));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(field1(temporary %10 = call<@type0, signature=fn(@type0, @type0) -> @type0, abi=sysv64(coerce<i8, i64>, coerce<i8, i64>) -> coerce<i8, i64>>(%3, copy<@type0, reason=arg>(read<@type0>(%8)), copy<@type0, reason=arg>(read<@type0>(%9))))), const<i64>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i64>(field1(%8), neg<i64, overflow=ub>(const<i64>(8)));
// DEFAULT-NEXT:         write<i64>(field1(%9), neg<i64, overflow=ub>(const<i64>(2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(field1(temporary %11 = call<@type0, signature=fn(@type0, @type0) -> @type0, abi=sysv64(coerce<i8, i64>, coerce<i8, i64>) -> coerce<i8, i64>>(%3, copy<@type0, reason=arg>(read<@type0>(%8)), copy<@type0, reason=arg>(read<@type0>(%9))))), const<i64>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
