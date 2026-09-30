/* PR rtl-optimization/57861 */

extern void  abort(void);
short        a = 1, f;
int          b, c, d, *g = &b, h, i, j;
unsigned int e;

static int foo(char p) {
  int k;
  for (c = 0; c < 2; c++) {
    i = (j = 0) || p;
    k = i * p;
    if (e < k) {
      short *l = &f;
      a        = d && h;
      *l       = 0;
    }
  }
  return 0;
}

int main() {
  *g = foo(a);
  if (a != 0)
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_b]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: i8) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_c]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i]], from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(const<i32>(0), const<i32>(0)), ne<i8>(read<i8>(%[[VALUE_p]]), const<i8>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_k]], mul<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), widen<i32, reason=promotion>(read<i8>(%[[VALUE_p]]))));
// DEFAULT-NEXT:                     if lt<u32>(read<u32>(%[[VALUE_e]]), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_k]])))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE_l:[0-9]+]] l: ptr<i16> [storage=automatic] = addr_of<ptr<i16>>(%[[VALUE_f]]);
// DEFAULT-NEXT:                             write<i16>(%[[VALUE_a]], from_bool<i16, reason=assign>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_h]]), const<i32>(0)))));
// DEFAULT-NEXT:                             write<i16>(deref(read<ptr<i16>>(%[[VALUE_l]])), truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_g]])), call<i32, signature=fn(i8) -> i32>(%[[VALUE_foo]], truncate<i8, reason=arg, fits=unknown>(read<i16>(%[[VALUE_a]]))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_a]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
