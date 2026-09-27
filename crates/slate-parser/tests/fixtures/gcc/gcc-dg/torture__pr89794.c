/* { dg-do run } */

typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

u32 a, b, c, d;

u32 foo(u32 f, u32 g, u32 g2, u32 g3, u16 h, u16 i) {
  (void)g, (void)g2, (void)g3, (void)h;
  d = __builtin_bswap64(i);
  __builtin_sub_overflow(0, d, &b);
  __builtin_memset(&i, c, 2);
  a = 0;
  return b + f + i + c;
}

int main(void) {
  u32 x = foo(0, 0, 0, 0, 0, 0);
  asm("" ::"r"(x));
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
// DEFAULT-NEXT:     type @type0 u16 = u16;
// DEFAULT-NEXT:     type @type1 u32 = u32;
// DEFAULT-NEXT:     type @type2 u64 = u64;
// DEFAULT-NEXT:     global %3 a: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 b: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 c: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 d: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %17 @__builtin_bswap64(%16 <unnamed>: u64) -> u64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %21 @__builtin_memset(%18 <unnamed>: ptr<void>, %19 <unnamed>: i32, %20 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %7 @foo(%8 f: u32, %9 g: u32, %10 g2: u32, %11 g3: u32, %12 h: u16, %13 i: u16) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         read<u32>(%9);
// DEFAULT-NEXT:         read<u32>(%10);
// DEFAULT-NEXT:         read<u32>(%11);
// DEFAULT-NEXT:         read<u16>(%12);
// DEFAULT-NEXT:         write<u32>(%6, truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%17, widen<u64, reason=arg>(read<u16>(%13)))));
// DEFAULT-NEXT:         overflow_sub<bool>(const<i32>(0), read<u32>(%6), deref(addr_of<ptr<u32>>(%4)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%21, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u16>>(%13)), reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%5)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         write<u32>(%3, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(read<u32>(%4), read<u32>(%8)), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%13))))), read<u32>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15 x: u32 [storage=automatic] = call<u32, signature=fn(u32, u32, u32, u32, u16, u16) -> u32>(%7, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         asm "" [dialect=att] {
// DEFAULT-NEXT:             in 0 "r" read<u32>(%15);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
