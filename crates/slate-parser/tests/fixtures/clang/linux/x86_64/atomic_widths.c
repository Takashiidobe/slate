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
// DEFAULT-NEXT:     type @type[[TYPE_memory_order:[0-9]+]] memory_order = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_memory_order_relaxed:[0-9]+]] memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_memory_order_consume:[0-9]+]] memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acquire:[0-9]+]] memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_memory_order_release:[0-9]+]] memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acq_rel:[0-9]+]] memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE_memory_order_seq_cst:[0-9]+]] memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_memory_order_2:[0-9]+]] memory_order = @type[[TYPE_memory_order]];
// DEFAULT-NEXT:     type @type[[TYPE_atomic_schar:[0-9]+]] atomic_schar = i8;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_uchar:[0-9]+]] atomic_uchar = u8;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_uint:[0-9]+]] atomic_uint = u32;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_llong:[0-9]+]] atomic_llong = i64;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([37, 117, 32, 37, 100, 32, 37, 117, 32, 37, 108, 108, 100, 32, 37, 117, 32, 37, 100, 32, 37, 117, 32, 37, 108, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_u8:[0-9]+]] u8: atomic u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(250)));
// DEFAULT-NEXT:         let %[[VALUE_i8:[0-9]+]] i8: atomic i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(5)));
// DEFAULT-NEXT:         let %[[VALUE_u32:[0-9]+]] u32: atomic u32 [storage=automatic] = const<u32>(1000);
// DEFAULT-NEXT:         let %[[VALUE_i64:[0-9]+]] i64: atomic i64 [storage=automatic] = neg<i64, overflow=ub>(const<i64>(10000000000));
// DEFAULT-NEXT:         let %[[VALUE_old_u8:[0-9]+]] old_u8: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u8 [synthetic] = update<u8, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic u8>>(%[[VALUE_u8]])), add<u8, overflow=wrap>(old<u8>, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(3)))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_old_u8]], read<u8>(%[[VALUE0]]));
// DEFAULT-NEXT:         let %[[VALUE_old_i8:[0-9]+]] old_i8: i8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i8>>(%[[VALUE_i8]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(7))));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_old_i8]], read<i8>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE_old_u32:[0-9]+]] old_u32: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u32 [synthetic] = update<u32, result=old, atomic=release>(deref(addr_of<ptr<atomic u32>>(%[[VALUE_u32]])), xor<u32>(old<u32>, const<u32>(255)));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_old_u32]], read<u32>(%[[VALUE2]]));
// DEFAULT-NEXT:         let %[[VALUE_old_i64:[0-9]+]] old_i64: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i64 [synthetic] = update<i64, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i64>>(%[[VALUE_i64]])), const<i64>(1234567890123));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_old_i64]], read<i64>(%[[VALUE3]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(29)>(%[[VALUE_str]])), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(%[[VALUE_old_u8]]))), widen<i32, reason=vararg>(read<i8>(%[[VALUE_old_i8]])), read<u32>(%[[VALUE_old_u32]]), read<i64>(%[[VALUE_old_i64]]), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8, atomic=seq_cst>(%[[VALUE_u8]]))), widen<i32, reason=vararg>(read<i8, atomic=seq_cst>(%[[VALUE_i8]])), read<u32, atomic=seq_cst>(%[[VALUE_u32]]), read<i64, atomic=seq_cst>(%[[VALUE_i64]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
