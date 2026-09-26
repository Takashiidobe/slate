#include <stdatomic.h>
#include <stdio.h>

int main(void) {
  atomic_uchar u8  = 250;
  atomic_schar i8  = -5;
  atomic_uint  u32 = 1000u;
  atomic_llong i64 = -10000000000LL;

  unsigned char old_u8 =
      atomic_fetch_add_explicit(&u8, 3, memory_order_relaxed);
  signed char  old_i8 = atomic_fetch_sub_explicit(&i8, 7, memory_order_acq_rel);
  unsigned int old_u32 =
      atomic_fetch_xor_explicit(&u32, 0x00FFu, memory_order_release);
  long long old_i64 =
      atomic_exchange_explicit(&i64, 1234567890123LL, memory_order_acquire);

  printf("%u %d %u %lld %u %d %u %lld\n", old_u8, old_i8, old_u32, old_i64,
         (unsigned char)u8, (signed char)i8, (unsigned int)u32, (long long)i64);
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
// DEFAULT-NEXT:     type @type0 memory_order = enum : u32 {
// DEFAULT-NEXT:         %0 memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %1 memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %2 memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %3 memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %4 memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %5 memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 memory_order = @type0;
// DEFAULT-NEXT:     type @type2 atomic_schar = i8;
// DEFAULT-NEXT:     type @type3 atomic_uchar = u8;
// DEFAULT-NEXT:     type @type4 atomic_uint = u32;
// DEFAULT-NEXT:     type @type5 atomic_llong = i64;
// DEFAULT-NEXT:     global %23 .str23: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([37, 117, 32, 37, 100, 32, 37, 117, 32, 37, 108, 108, 100, 32, 37, 117, 32, 37, 100, 32, 37, 117, 32, 37, 108, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %12 @printf(%22 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 u8: atomic u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(250)));
// DEFAULT-NEXT:         let %15 i8: atomic i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(5)));
// DEFAULT-NEXT:         let %16 u32: atomic u32 [storage=automatic] = const<u32>(1000);
// DEFAULT-NEXT:         let %17 i64: atomic i64 [storage=automatic] = neg<i64, overflow=ub>(const<i64>(10000000000));
// DEFAULT-NEXT:         let %18 old_u8: u8 [storage=automatic];
// DEFAULT-NEXT:         let %24: u8 [synthetic] = update<u8, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic u8>>(%14)), add<u8, overflow=wrap>(old<u8>, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(3)))));
// DEFAULT-NEXT:         write<u8>(%18, read<u8>(%24));
// DEFAULT-NEXT:         let %19 old_i8: i8 [storage=automatic];
// DEFAULT-NEXT:         let %25: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i8>>(%15)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(7))));
// DEFAULT-NEXT:         write<i8>(%19, read<i8>(%25));
// DEFAULT-NEXT:         let %20 old_u32: u32 [storage=automatic];
// DEFAULT-NEXT:         let %26: u32 [synthetic] = update<u32, result=old, atomic=release>(deref(addr_of<ptr<atomic u32>>(%16)), xor<u32>(old<u32>, const<u32>(255)));
// DEFAULT-NEXT:         write<u32>(%20, read<u32>(%26));
// DEFAULT-NEXT:         let %21 old_i64: i64 [storage=automatic];
// DEFAULT-NEXT:         let %27: i64 [synthetic] = update<i64, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i64>>(%17)), const<i64>(1234567890123));
// DEFAULT-NEXT:         write<i64>(%21, read<i64>(%27));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(29)>(%23)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(%18))), widen<i32, reason=vararg>(read<i8>(%19)), read<u32>(%20), read<i64>(%21), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8, atomic=seq_cst>(%14))), widen<i32, reason=vararg>(read<i8, atomic=seq_cst>(%15)), read<u32, atomic=seq_cst>(%16), read<i64, atomic=seq_cst>(%17));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
