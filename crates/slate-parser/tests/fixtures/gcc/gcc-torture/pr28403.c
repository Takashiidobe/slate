void abort(void);
void exit(int);

typedef unsigned long long ull;
int                        global;

int __attribute__((noinline)) foo(int x1, int x2, int x3, int x4, int x5,
                                  int x6, int x7, int x8) {
  global = x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8;
}

ull __attribute__((noinline)) bar(ull x) {
  foo(1, 2, 1, 3, 1, 4, 1, 5);
  return x >> global;
}

int main(void) {
  if (bar(0x123456789abcdefULL) != (0x123456789abcdefULL >> 18))
    abort();
  exit(0);
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
// DEFAULT-NEXT:     type @type0 ull = u64;
// DEFAULT-NEXT:     global %3 global: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%16 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo(%5 x1: i32, %6 x2: i32, %7 x3: i32, %8 x4: i32, %9 x5: i32, %10 x6: i32, %11 x7: i32, %12 x8: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%3, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%5), read<i32>(%6)), read<i32>(%7)), read<i32>(%8)), read<i32>(%9)), read<i32>(%10)), read<i32>(%11)), read<i32>(%12)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @bar(%14 x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32) -> i32>(%4, const<i32>(1), const<i32>(2), const<i32>(1), const<i32>(3), const<i32>(1), const<i32>(4), const<i32>(1), const<i32>(5));
// DEFAULT-NEXT:         return shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%14), read<i32>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64) -> u64>(%13, const<u64>(81985529216486895)), shr<u64, amount_out_of_range=ub, fill=zero_extend>(const<u64>(81985529216486895), const<i32>(18)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
