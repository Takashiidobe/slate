/* { dg-do run } */
/* { dg-options "-O2" } */

extern void abort (void);


__attribute__ ((noinline, noclone))
static __INT32_TYPE__ rt32 (__INT32_TYPE__ x, int y, __INT32_TYPE__ z) {
  return (__builtin_bswap32(x) >> y) & z;
}
#define TEST32(X,Y,Z) if(((__builtin_bswap32(X)>>Y)&Z)!=rt32(X,Y,Z)) abort()
void test32(__INT32_TYPE__ x)
{
  TEST32(x,0,1);
  TEST32(x,0,255);
  TEST32(x,1,1);
  TEST32(x,2,1);
  TEST32(x,3,1);
  TEST32(x,4,1);
  TEST32(x,5,1);
  TEST32(x,6,1);
  TEST32(x,7,1);
  TEST32(x,8,1);
  TEST32(x,8,255);
  TEST32(x,9,1);
  TEST32(x,10,1);
  TEST32(x,11,1);
  TEST32(x,12,1);
  TEST32(x,13,1);
  TEST32(x,14,1);
  TEST32(x,15,1);
  TEST32(x,16,1);
  TEST32(x,16,255);
  TEST32(x,17,1);
  TEST32(x,18,1);
  TEST32(x,19,1);
  TEST32(x,20,1);
  TEST32(x,21,1);
  TEST32(x,22,1);
  TEST32(x,23,1);
  TEST32(x,24,1);
  TEST32(x,24,255);
  TEST32(x,25,1);
  TEST32(x,26,1);
  TEST32(x,27,1);
  TEST32(x,28,1);
  TEST32(x,29,1);
  TEST32(x,30,1);
  TEST32(x,31,1);
}

#if __SIZEOF_LONG_LONG__ == 8
__attribute__ ((noinline, noclone))
static long long rt64 (long long x, int y, long long z) {
  return (__builtin_bswap64(x) >> y) & z;
}
#define TEST64(X,Y,Z) if(((__builtin_bswap64(X)>>Y)&Z)!=rt64(X,Y,Z)) abort()
void test64(long long x)
{
  TEST64(x,0,1);
  TEST64(x,0,255);
  TEST64(x,1,1);
  TEST64(x,2,1);
  TEST64(x,3,1);
  TEST64(x,4,1);
  TEST64(x,5,1);
  TEST64(x,6,1);
  TEST64(x,7,1);
  TEST64(x,8,1);
  TEST64(x,8,255);
  TEST64(x,9,1);
  TEST64(x,10,1);
  TEST64(x,11,1);
  TEST64(x,12,1);
  TEST64(x,13,1);
  TEST64(x,14,1);
  TEST64(x,15,1);
  TEST64(x,16,1);
  TEST64(x,16,255);
  TEST64(x,17,1);
  TEST64(x,18,1);
  TEST64(x,19,1);
  TEST64(x,20,1);
  TEST64(x,21,1);
  TEST64(x,22,1);
  TEST64(x,23,1);
  TEST64(x,24,1);
  TEST64(x,24,255);
  TEST64(x,25,1);
  TEST64(x,26,1);
  TEST64(x,27,1);
  TEST64(x,28,1);
  TEST64(x,29,1);
  TEST64(x,30,1);
  TEST64(x,31,1);
  TEST64(x,32,1);
  TEST64(x,32,255);
  TEST64(x,33,1);
  TEST64(x,34,1);
  TEST64(x,35,1);
  TEST64(x,36,1);
  TEST64(x,37,1);
  TEST64(x,38,1);
  TEST64(x,39,1);
  TEST64(x,40,1);
  TEST64(x,40,255);
  TEST64(x,41,1);
  TEST64(x,42,1);
  TEST64(x,43,1);
  TEST64(x,44,1);
  TEST64(x,45,1);
  TEST64(x,46,1);
  TEST64(x,47,1);
  TEST64(x,48,1);
  TEST64(x,48,255);
  TEST64(x,49,1);
  TEST64(x,50,1);
  TEST64(x,51,1);
  TEST64(x,52,1);
  TEST64(x,53,1);
  TEST64(x,54,1);
  TEST64(x,55,1);
  TEST64(x,56,1);
  TEST64(x,56,255);
  TEST64(x,57,1);
  TEST64(x,58,1);
  TEST64(x,59,1);
  TEST64(x,60,1);
  TEST64(x,61,1);
  TEST64(x,62,1);
  TEST64(x,63,1);
}
#endif

__attribute__ ((noinline, noclone))
static int rt16 (int x, int y, int z) {
  return (__builtin_bswap16(x) >> y) & z;
}
#define TEST16(X,Y,Z) if(((__builtin_bswap16(X)>>Y)&Z)!=rt16(X,Y,Z)) abort()
void test16(int x)
{
  TEST16(x,0,1);
  TEST16(x,0,255);
  TEST16(x,1,1);
  TEST16(x,2,1);
  TEST16(x,3,1);
  TEST16(x,4,1);
  TEST16(x,5,1);
  TEST16(x,6,1);
  TEST16(x,7,1);
  TEST16(x,8,1);
  TEST16(x,8,255);
  TEST16(x,9,1);
  TEST16(x,10,1);
  TEST16(x,11,1);
  TEST16(x,12,1);
  TEST16(x,13,1);
  TEST16(x,14,1);
  TEST16(x,15,1);
}

int main()
{
  test32(0x00000000);
  test32(0xffffffff);
  test32(0x00000001);
  test32(0x00000002);
  test32(0x00000004);
  test32(0x00000008);
  test32(0x00000010);
  test32(0x00000020);
  test32(0x00000040);
  test32(0x00000080);
  test32(0x00000100);
  test32(0x00000200);
  test32(0x00000400);
  test32(0x00000800);
  test32(0x00001000);
  test32(0x00002000);
  test32(0x00004000);
  test32(0x00008000);
  test32(0x00010000);
  test32(0x00020000);
  test32(0x00040000);
  test32(0x00080000);
  test32(0x00100000);
  test32(0x00200000);
  test32(0x00400000);
  test32(0x00800000);
  test32(0x01000000);
  test32(0x02000000);
  test32(0x04000000);
  test32(0x08000000);
  test32(0x10000000);
  test32(0x20000000);
  test32(0x40000000);
  test32(0x80000000);
  test32(0x12345678);
  test32(0x87654321);
  test32(0xdeadbeef);
  test32(0xcafebabe);

#if __SIZEOF_LONG_LONG__ == 8
  test64(0x0000000000000000ll);
  test64(0xffffffffffffffffll);
  test64(0x0000000000000001ll);
  test64(0x0000000000000002ll);
  test64(0x0000000000000004ll);
  test64(0x0000000000000008ll);
  test64(0x0000000000000010ll);
  test64(0x0000000000000020ll);
  test64(0x0000000000000040ll);
  test64(0x0000000000000080ll);
  test64(0x0000000000000100ll);
  test64(0x0000000000000200ll);
  test64(0x0000000000000400ll);
  test64(0x0000000000000800ll);
  test64(0x0000000000001000ll);
  test64(0x0000000000002000ll);
  test64(0x0000000000004000ll);
  test64(0x0000000000008000ll);
  test64(0x0000000000010000ll);
  test64(0x0000000000020000ll);
  test64(0x0000000000040000ll);
  test64(0x0000000000080000ll);
  test64(0x0000000000100000ll);
  test64(0x0000000000200000ll);
  test64(0x0000000000400000ll);
  test64(0x0000000000800000ll);
  test64(0x0000000001000000ll);
  test64(0x0000000002000000ll);
  test64(0x0000000004000000ll);
  test64(0x0000000008000000ll);
  test64(0x0000000010000000ll);
  test64(0x0000000020000000ll);
  test64(0x0000000040000000ll);
  test64(0x0000000080000000ll);
  test64(0x0000000100000000ll);
  test64(0x0000000200000000ll);
  test64(0x0000000400000000ll);
  test64(0x0000000800000000ll);
  test64(0x0000001000000000ll);
  test64(0x0000002000000000ll);
  test64(0x0000004000000000ll);
  test64(0x0000008000000000ll);
  test64(0x0000010000000000ll);
  test64(0x0000020000000000ll);
  test64(0x0000040000000000ll);
  test64(0x0000080000000000ll);
  test64(0x0000100000000000ll);
  test64(0x0000200000000000ll);
  test64(0x0000400000000000ll);
  test64(0x0000800000000000ll);
  test64(0x0001000000000000ll);
  test64(0x0002000000000000ll);
  test64(0x0004000000000000ll);
  test64(0x0008000000000000ll);
  test64(0x0010000000000000ll);
  test64(0x0020000000000000ll);
  test64(0x0040000000000000ll);
  test64(0x0080000000000000ll);
  test64(0x0100000000000000ll);
  test64(0x0200000000000000ll);
  test64(0x0400000000000000ll);
  test64(0x0800000000000000ll);
  test64(0x1000000000000000ll);
  test64(0x2000000000000000ll);
  test64(0x4000000000000000ll);
  test64(0x8000000000000000ll);
  test64(0x0123456789abcdefll);
  test64(0xfedcba9876543210ll);
  test64(0xdeadbeefdeadbeefll);
  test64(0xcafebabecafebabell);
#endif

  test16(0x0000);
  test16(0xffff);
  test16(0x0001);
  test16(0x0002);
  test16(0x0004);
  test16(0x0008);
  test16(0x0010);
  test16(0x0020);
  test16(0x0040);
  test16(0x0080);
  test16(0x0100);
  test16(0x0200);
  test16(0x0400);
  test16(0x0800);
  test16(0x1000);
  test16(0x2000);
  test16(0x4000);
  test16(0x8000);
  test16(0x1234);
  test16(0x4321);
  test16(0xdead);
  test16(0xbeef);
  test16(0xcafe);
  test16(0xbabe);

  return 0;
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bswap32:[0-9]+]] @__builtin_bswap32(%[[VALUE0:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_rt32:[0-9]+]] @rt32(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32, %[[VALUE_z:[0-9]+]] z: i32) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x]]))), read<i32>(%[[VALUE_y]])), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_z]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test32:[0-9]+]] @test32(%[[VALUE_x_2:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(0), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(0), const<i32>(255))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(1), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(2), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(3)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(3), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(4)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(4), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(5)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(5), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(6)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(6), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(7)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(7), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(8), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(8), const<i32>(255))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(9)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(9), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(10), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(11)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(11), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(12)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(12), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(13)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(13), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(14)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(14), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(15)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(15), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(16)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(16), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(16)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(16), const<i32>(255))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(17)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(17), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(18)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(18), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(19)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(19), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(20)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(20), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(21)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(21), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(22)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(22), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(23)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(23), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(24)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(24), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(24)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(24), const<i32>(255))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(25)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(25), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(26)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(26), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(27)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(27), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(28)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(28), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(29)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(29), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(30)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(30), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_2]]))), const<i32>(31)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt32]], read<i32>(%[[VALUE_x_2]]), const<i32>(31), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bswap64:[0-9]+]] @__builtin_bswap64(%[[VALUE1:[0-9]+]] <unnamed>: u64) -> u64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_rt64:[0-9]+]] @rt64(%[[VALUE_x_3:[0-9]+]] x: i64, %[[VALUE_y_2:[0-9]+]] y: i32, %[[VALUE_z_2:[0-9]+]] z: i64) -> i64 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i64, reason=return, fits=unknown>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_3]]))), read<i32>(%[[VALUE_y_2]])), reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_z_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test64:[0-9]+]] @test64(%[[VALUE_x_4:[0-9]+]] x: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(0)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(0), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(0)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(0), widen<i64, reason=arg>(const<i32>(255)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(1)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(1), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(2), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(3), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(4), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(5), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(6)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(6), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(7)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(7), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(8), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(8), widen<i64, reason=arg>(const<i32>(255)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(9)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(9), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(10)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(10), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(11)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(11), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(12)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(12), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(13)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(13), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(14)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(14), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(15)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(15), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(16)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(16), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(16)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(16), widen<i64, reason=arg>(const<i32>(255)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(17)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(17), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(18)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(18), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(19)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(19), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(20)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(20), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(21)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(21), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(22)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(22), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(23)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(23), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(24)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(24), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(24)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(24), widen<i64, reason=arg>(const<i32>(255)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(25)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(25), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(26)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(26), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(27)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(27), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(28)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(28), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(29)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(29), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(30)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(30), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(31)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(31), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(32)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(32), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(32)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(32), widen<i64, reason=arg>(const<i32>(255)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(33)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(33), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(34)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(34), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(35)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(35), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(36)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(36), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(37)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(37), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(38)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(38), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(39)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(39), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(40)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(40), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(40)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(40), widen<i64, reason=arg>(const<i32>(255)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(41)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(41), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(42)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(42), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(43)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(43), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(44)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(44), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(45)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(45), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(46)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(46), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(47)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(47), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(48)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(48), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(48)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(48), widen<i64, reason=arg>(const<i32>(255)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(49)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(49), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(50)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(50), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(51)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(51), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(52)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(52), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(53)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(53), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(54)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(54), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(55)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(55), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(56)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(56), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(56)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(255)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(56), widen<i64, reason=arg>(const<i32>(255)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(57)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(57), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(58)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(58), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(59)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(59), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(60)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(60), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(61)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(61), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(62)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(62), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x_4]]))), const<i32>(63)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn(i64, i32, i64) -> i64>(%[[VALUE_rt64]], read<i64>(%[[VALUE_x_4]]), const<i32>(63), widen<i64, reason=arg>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bswap16:[0-9]+]] @__builtin_bswap16(%[[VALUE2:[0-9]+]] <unnamed>: u16) -> u16 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_rt16:[0-9]+]] @rt16(%[[VALUE_x_5:[0-9]+]] x: i32, %[[VALUE_y_3:[0-9]+]] y: i32, %[[VALUE_z_3:[0-9]+]] z: i32) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_5]])))))), read<i32>(%[[VALUE_y_3]])), read<i32>(%[[VALUE_z_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test16:[0-9]+]] @test16(%[[VALUE_x_6:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(0)), const<i32>(1)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(0), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(0)), const<i32>(255)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(0), const<i32>(255)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(1)), const<i32>(1)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(1), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(2)), const<i32>(1)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(2), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(3)), const<i32>(1)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(3), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(4)), const<i32>(1)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(4), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(5)), const<i32>(1)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(5), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(6)), const<i32>(1)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(6), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(7)), const<i32>(1)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(7), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(8)), const<i32>(1)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(8), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(8)), const<i32>(255)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(8), const<i32>(255)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(9)), const<i32>(1)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(9), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(10)), const<i32>(1)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(10), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(11)), const<i32>(1)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(11), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(12)), const<i32>(1)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(12), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(13)), const<i32>(1)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(13), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(14)), const<i32>(1)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(14), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x_6]])))))), const<i32>(15)), const<i32>(1)), call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_rt16]], read<i32>(%[[VALUE_x_6]]), const<i32>(15), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], reinterpret<i32, reason=arg, fits=unknown>(const<u32>(4294967295)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(4));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(8));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(16));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(32));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(256));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(512));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(1024));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(2048));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(4096));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(8192));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(16384));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(32768));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(65536));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(131072));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(262144));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(524288));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(1048576));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(2097152));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(4194304));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(8388608));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(16777216));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(33554432));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(67108864));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(134217728));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(268435456));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(536870912));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(1073741824));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2147483648)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], const<i32>(305419896));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], reinterpret<i32, reason=arg, fits=unknown>(const<u32>(2271560481)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], reinterpret<i32, reason=arg, fits=unknown>(const<u32>(3735928559)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test32]], reinterpret<i32, reason=arg, fits=unknown>(const<u32>(3405691582)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], reinterpret<i64, reason=arg, fits=unknown>(const<u64>(18446744073709551615)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(2));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(4));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(8));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(16));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(32));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(64));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(128));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(256));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(512));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(1024));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(2048));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(4096));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(8192));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(16384));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(32768));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(65536));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(131072));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(262144));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(524288));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(1048576));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(2097152));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(4194304));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(8388608));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(16777216));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(33554432));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(67108864));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(134217728));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(268435456));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(536870912));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(1073741824));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(2147483648));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(4294967296));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(8589934592));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(17179869184));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(34359738368));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(68719476736));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(137438953472));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(274877906944));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(549755813888));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(1099511627776));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(2199023255552));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(4398046511104));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(8796093022208));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(17592186044416));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(35184372088832));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(70368744177664));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(140737488355328));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(281474976710656));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(562949953421312));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(1125899906842624));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(2251799813685248));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(4503599627370496));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(9007199254740992));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(18014398509481984));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(36028797018963968));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(72057594037927936));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(144115188075855872));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(288230376151711744));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(576460752303423488));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(1152921504606846976));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(2305843009213693952));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(4611686018427387904));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], reinterpret<i64, reason=arg, fits=unknown>(const<u64>(9223372036854775808)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], const<i64>(81985529216486895));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], reinterpret<i64, reason=arg, fits=unknown>(const<u64>(18364758544493064720)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], reinterpret<i64, reason=arg, fits=unknown>(const<u64>(16045690984833335023)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_test64]], reinterpret<i64, reason=arg, fits=unknown>(const<u64>(14627333968358193854)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(65535));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(4));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(8));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(16));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(32));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(256));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(512));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(1024));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(2048));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(4096));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(8192));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(16384));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(32768));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(4660));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(17185));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(57005));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(48879));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(51966));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test16]], const<i32>(47806));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
