#include <stdio.h>

unsigned long bump(unsigned long *p, int c) {
  return c ? (*p += 2) : (*p += 1);
}

int main() {
  unsigned long x = 10;
  unsigned long a = bump(&x, 1);
  unsigned long b = bump(&x, 0);
  printf("%lu %lu %lu\n", a, b, x);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 108, 117, 32, 37, 108, 117, 32, 37, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bump:[0-9]+]] @bump(%[[VALUE_p:[0-9]+]] p: ptr<u64>, %[[VALUE_c:[0-9]+]] c: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u64 [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0))
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: ptr<u64> [synthetic] = read<ptr<u64>>(%[[VALUE_p]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%[[VALUE1]])));
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:             write<u64>(deref(read<ptr<u64>>(%[[VALUE1]])), read<u64>(%[[VALUE3]]));
// DEFAULT-NEXT:             write<u64>(%[[VALUE0]], read<u64>(%[[VALUE3]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: ptr<u64> [synthetic] = read<ptr<u64>>(%[[VALUE_p]]);
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%[[VALUE4]])));
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE5]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:             write<u64>(deref(read<ptr<u64>>(%[[VALUE4]])), read<u64>(%[[VALUE6]]));
// DEFAULT-NEXT:             write<u64>(%[[VALUE0]], read<u64>(%[[VALUE6]]));
// DEFAULT-NEXT:         return read<u64>(%[[VALUE0]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(10)));
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: u64 [storage=automatic] = call<u64, signature=fn(ptr<u64>, i32) -> u64>(%[[VALUE_bump]], addr_of<ptr<u64>>(%[[VALUE_x]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: u64 [storage=automatic] = call<u64, signature=fn(ptr<u64>, i32) -> u64>(%[[VALUE_bump]], addr_of<ptr<u64>>(%[[VALUE_x]]), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str]])), read<u64>(%[[VALUE_a]]), read<u64>(%[[VALUE_b]]), read<u64>(%[[VALUE_x]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
