/* PR middle-end/87053 */

const union {
  struct {
    char x[4];
    char y[4];
  };
  struct {
    char z[8];
  };
} u = {{"1234", "567"}};

int main() {
  if (__builtin_strlen(u.z) != 7)
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type0 = union {
// DEFAULT-NEXT:         field0 <anonymous>: @type1;
// DEFAULT-NEXT:         field1 <anonymous>: @type2;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 4>;
// DEFAULT-NEXT:         field1 y: array<i8, 4>;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 z: array<i8, 8>;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %3 u: @type0 [storage=static] [const] = aggregate<@type0, zero_fill=false>(field0 = aggregate<@type1, zero_fill=false>(field0 = code_units<array<i8, 4>>([49, 50, 51, 52]), field1 = code_units<array<i8, 4>>([53, 54, 55, 0]))) [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(__builtin_strlen, array_decay<ptr<const i8>, length=Some(8)>(field0(field1(%3)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
