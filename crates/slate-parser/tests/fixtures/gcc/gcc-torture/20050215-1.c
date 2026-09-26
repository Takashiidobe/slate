/* PR middle-end/19857 */

typedef struct {
  char c[8];
} V
#ifdef __ELF__
    __attribute__((aligned(8)))
#endif
    ;
typedef __SIZE_TYPE__ size_t;
V                     v;
void                  abort(void);

int main(void) {
  V *w = &v;
  if (((size_t)((float *)((size_t)w & ~(size_t)3)) % 8) != 0 ||
      ((size_t)w & 1)) {
#ifndef __ELF__
    if (((size_t)&v & 7) == 0)
#endif
      abort();
  }
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 8>;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type1 V = @type0;
// DEFAULT-NEXT:     type @type2 size_t = u64;
// DEFAULT-NEXT:     global %3 v: @type0 [storage=static] [align=8] [linkage=external];
// DEFAULT-NEXT:     fn %4 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 w: ptr<@type0> [storage=automatic] = addr_of<ptr<@type0>>(%3);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(rem<u64, by_zero=ub>(ptr_to_int<u64, reason=explicit>(int_to_ptr<ptr<f32>, reason=explicit>(and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<@type0>>(%6)), not<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), ne<u64>(and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<@type0>>(%6)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), const<u64>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
