/* PR tree-optimization/54471 */

#ifdef __SIZEOF_INT128__
#define T __int128
#else
#define T long long
#endif

extern void abort(void);

__attribute__((noinline)) unsigned T foo(T ixi, unsigned ctr) {
  unsigned T irslt = 1;
  T          ix    = ixi;

  for (; ctr; ctr--) {
    irslt *= ix;
    ix    *= ix;
  }

  if (irslt != 14348907)
    abort();
  return irslt;
}

int main() {
  unsigned T res;

  res = foo(3, 4);
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_ixi:[0-9]+]] ixi: i128, %[[VALUE_ctr:[0-9]+]] ctr: u32) -> u128 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_irslt:[0-9]+]] irslt: u128 [storage=automatic] = reinterpret<u128, reason=assign, fits=unknown>(widen<i128, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_ix:[0-9]+]] ix: i128 [storage=automatic] = read<i128>(%[[VALUE_ixi]]);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<u32>(read<u32>(%[[VALUE_ctr]]), const<u32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_ctr]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u32 [synthetic] = sub<u32, overflow=wrap>(read<u32>(%[[VALUE1]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_ctr]], read<u32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: u128 [synthetic] = read<u128>(%[[VALUE_irslt]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: u128 [synthetic] = mul<u128, overflow=wrap>(read<u128>(%[[VALUE3]]), reinterpret<u128, reason=usual_arith, fits=unknown>(read<i128>(%[[VALUE_ix]])));
// DEFAULT-NEXT:                     write<u128>(%[[VALUE_irslt]], read<u128>(%[[VALUE4]]));
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_ix]]);
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%[[VALUE5]]), read<i128>(%[[VALUE_ix]]));
// DEFAULT-NEXT:                     write<i128>(%[[VALUE_ix]], read<i128>(%[[VALUE6]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<u128>(read<u128>(%[[VALUE_irslt]]), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(14348907))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return read<u128>(%[[VALUE_irslt]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_res:[0-9]+]] res: u128 [storage=automatic];
// DEFAULT-NEXT:         write<u128>(%[[VALUE_res]], call<u128, signature=fn(i128, u32) -> u128>(%[[VALUE_foo]], widen<i128, reason=arg>(const<i32>(3)), reinterpret<u32, reason=arg, fits=always>(const<i32>(4))));
// DEFAULT-NEXT:         call<u128, signature=fn(i128, u32) -> u128>(%[[VALUE_foo]], widen<i128, reason=arg>(const<i32>(3)), reinterpret<u32, reason=arg, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
