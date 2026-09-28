/* { dg-require-effective-target int32plus } */

int f(unsigned int a) __attribute__((noipa));
int f(unsigned int a) { return ((__builtin_bswap32(a)) >> 24) & 0x3; }

int g(unsigned int a) __attribute__((noipa));
int g(unsigned int a) { return a & 0x3; }

int main(void) {
  for (int b = 0; b <= 0xF; b++) {
    if (f(b) != g(b))
      __builtin_abort();
  }
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
// DEFAULT-NEXT:     fn %1 @f(%2 a: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%10, read<u32>(%2)), const<i32>(24)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @__builtin_bswap32(%9 <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %4 @g(%5 a: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(and<u32>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %7 b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%7), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u32) -> i32>(%1, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%7))), call<i32, signature=fn(u32) -> i32>(%4, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%7))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
