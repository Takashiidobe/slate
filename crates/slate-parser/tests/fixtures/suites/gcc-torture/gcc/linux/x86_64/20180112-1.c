/* PR rtl-optimization/83565 */
/* Testcase by Sergei Trofimovich <slyfox@inbox.ru> */

extern void abort(void);

typedef __UINT32_TYPE__ u32;

u32 bug(u32 *result) __attribute__((noinline));
u32 bug(u32 *result) {
  volatile u32 ss = 0xFFFFffff;
  volatile u32 d  = 0xEEEEeeee;
  u32          tt = d & 0x00800000;
  u32          r  = tt << 8;

  r = (r >> 31) | (r << 1);

  u32 u   = r ^ ss;
  u32 off = u >> 1;

  *result = tt;
  return off;
}

int main(void) {
  u32 l;
  u32 off = bug(&l);
  if (off != 0x7fffffff)
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
// DEFAULT-NEXT:     type @type[[TYPE_u32:[0-9]+]] u32 = u32;
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bug:[0-9]+]] @bug(%[[VALUE_result:[0-9]+]] result: ptr<u32>) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ss:[0-9]+]] ss: volatile u32 [storage=automatic] = const<u32>(4294967295);
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: volatile u32 [storage=automatic] = const<u32>(4008636142);
// DEFAULT-NEXT:         let %[[VALUE_tt:[0-9]+]] tt: u32 [storage=automatic] = and<u32>(read<u32, volatile>(%[[VALUE_d]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8388608)));
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: u32 [storage=automatic] = shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_tt]]), const<i32>(8));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r]], or<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_r]]), const<i32>(31)), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_r]]), const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: u32 [storage=automatic] = xor<u32>(read<u32>(%[[VALUE_r]]), read<u32, volatile>(%[[VALUE_ss]]));
// DEFAULT-NEXT:         let %[[VALUE_off:[0-9]+]] off: u32 [storage=automatic] = shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_u]]), const<i32>(1));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%[[VALUE_result]])), read<u32>(%[[VALUE_tt]]));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_off]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_off_2:[0-9]+]] off: u32 [storage=automatic] = call<u32, signature=fn(ptr<u32>) -> u32>(%[[VALUE_bug]], addr_of<ptr<u32>>(%[[VALUE_l]]));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_off_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
