/* PR tree-optimization/102134 */

typedef unsigned long long u64;

u64 g;

void foo(u64 a, u64 b, u64 c, u64 *r) {
  b     *= b;
  u64 x  = a && ((b >> (c & 63)) | ((b << (c & 63)) & g));
  *r     = x + a;
}

int main() {
  u64 x;
  foo(1, 3000, 0, &x);
  if (x != 2)
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
// DEFAULT-NEXT:     type @type[[TYPE_u64:[0-9]+]] u64 = u64;
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: u64, %[[VALUE_b:[0-9]+]] b: u64, %[[VALUE_c:[0-9]+]] c: u64, %[[VALUE_r:[0-9]+]] r: ptr<u64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u64 [synthetic] = mul<u64, overflow=wrap>(read<u64>(%[[VALUE0]]), read<u64>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_b]], read<u64>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: u64 [storage=automatic] = from_bool<u64, reason=assign>(logical_and<bool>(ne<u64>(read<u64>(%[[VALUE_a]]), const<u64>(0)), ne<u64>(or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_b]]), and<u64>(read<u64>(%[[VALUE_c]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(63))))), and<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_b]]), and<u64>(read<u64>(%[[VALUE_c]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(63))))), read<u64>(%[[VALUE_g]]))), const<u64>(0))));
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%[[VALUE_r]])), add<u64, overflow=wrap>(read<u64>(%[[VALUE_x]]), read<u64>(%[[VALUE_a]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: u64 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(u64, u64, u64, ptr<u64>) -> void>(%[[VALUE_foo]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3000))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), addr_of<ptr<u64>>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_x_2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
