/* PR middle-end/58564 */

extern void abort(void);
int         a, b;
short      *c, **d = &c;

int main() {
  b = (0, 0 > ((&c == d) & (1 && (a ^ 1)))) | 0U;
  if (b != 0)
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: ptr<i16> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: ptr<ptr<i16>> [storage=static] = addr_of<ptr<ptr<i16>>>(%[[VALUE_c]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], reinterpret<i32, reason=assign, fits=unknown>(or<u32>(reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(gt<i32>(const<i32>(0), and<i32>(from_bool<i32, reason=promotion>(eq<ptr<ptr<i16>>>(addr_of<ptr<ptr<i16>>>(%[[VALUE_c]]), read<ptr<ptr<i16>>>(%[[VALUE_d]]))), from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i32>(xor<i32>(read<i32>(%[[VALUE_a]]), const<i32>(1)), const<i32>(0)))))))), const<u32>(0))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
