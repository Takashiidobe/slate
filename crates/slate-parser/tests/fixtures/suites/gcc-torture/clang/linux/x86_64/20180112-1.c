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
// DEFAULT-NEXT:     type @type0 u32 = u32;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @bug(%4 result: ptr<u32>) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 ss: volatile u32 [storage=automatic] = const<u32>(4294967295);
// DEFAULT-NEXT:         let %6 d: volatile u32 [storage=automatic] = const<u32>(4008636142);
// DEFAULT-NEXT:         let %7 tt: u32 [storage=automatic] = and<u32>(read<u32, volatile>(%6), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8388608)));
// DEFAULT-NEXT:         let %8 r: u32 [storage=automatic] = shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%7), const<i32>(8));
// DEFAULT-NEXT:         write<u32>(%8, or<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%8), const<i32>(31)), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%8), const<i32>(1))));
// DEFAULT-NEXT:         let %9 u: u32 [storage=automatic] = xor<u32>(read<u32>(%8), read<u32, volatile>(%5));
// DEFAULT-NEXT:         let %10 off: u32 [storage=automatic] = shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%9), const<i32>(1));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%4)), read<u32>(%7));
// DEFAULT-NEXT:         return read<u32>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 l: u32 [storage=automatic];
// DEFAULT-NEXT:         let %13 off: u32 [storage=automatic] = call<u32, signature=fn(ptr<u32>) -> u32>(%3, addr_of<ptr<u32>>(%12));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%13), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
