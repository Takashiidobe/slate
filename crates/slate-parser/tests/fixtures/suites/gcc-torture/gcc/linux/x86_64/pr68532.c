/* { dg-options "-O2 -ftree-vectorize -fno-vect-cost-model" } */
/* { dg-additional-options "-fno-common" { target hppa*-*-hpux* } } */

#define SIZE 128
unsigned short _Alignas(16) in[SIZE];

__attribute__((noinline)) int test(unsigned short sum, unsigned short *in,
                                   int x) {
  for (int j = 0; j < SIZE; j += 8)
    sum += in[j] * x;
  return sum;
}

int main() {
  for (int i = 0; i < SIZE; i++)
    in[i] = i;
  if (test(0, in, 1) != 960)
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
// DEFAULT-NEXT:     global %[[VALUE_in:[0-9]+]] in: array<u16, 128> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_sum:[0-9]+]] sum: u16, %[[VALUE_in_2:[0-9]+]] in: ptr<u16>, %[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(128))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(8));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_sum]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE3]]))), mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_in_2]]), read<i32>(%[[VALUE_j]])))))), read<i32>(%[[VALUE_x]])))));
// DEFAULT-NEXT:                 write<u16>(%[[VALUE_sum]], read<u16>(%[[VALUE4]]));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(widen<u32, reason=return>(read<u16>(%[[VALUE_sum]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(128))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(128)>(%[[VALUE_in]]), read<i32>(%[[VALUE_i]]))), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(read<i32>(%[[VALUE_i]]))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16, ptr<u16>, i32) -> i32>(%[[VALUE_test]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), array_decay<ptr<u16>, length=Some(128)>(%[[VALUE_in]]), const<i32>(1)), const<i32>(960))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
