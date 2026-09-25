/* PR middle-end/98366 */
/* { dg-require-effective-target int32 } */

typedef struct S {
  int a, b, c : 7, d : 8, e : 17;
} S;
const S f[] = {{0, 3, 4, 2, 0}};

int main() {
  if (__builtin_memcmp(f, (S[]){{.b = 3, .c = 4, .d = 2, .e = 0}}, sizeof(S)))
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32 : 7;
// DEFAULT-NEXT:         field3 d: i32 : 8;
// DEFAULT-NEXT:         field4 e: i32 : 17;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8, 8, 9], bit_offsets=[None, None, Some(64), Some(71), Some(79)], bit_units=[(8, 4)], field_units=[None, None, Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type1 S = @type0;
// DEFAULT-NEXT:     global %2 f: array<@type0, 1> [storage=static] [const] = aggregate<array<@type0, 1>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(3), field2 = const<i32>(4), field3 = const<i32>(2), field4 = const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const @type0>, length=Some(1)>(%2)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<@type0>, length=Some(1)>(compound_literal %4 [storage=automatic] = aggregate<array<@type0, 1>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=true>(field1 = const<i32>(3), field2 = const<i32>(4), field3 = const<i32>(2), field4 = const<i32>(0))))), const<u64>(12)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
