/* Test saving and restoring of SIMD registers.  */

void abort(void);

typedef short Q __attribute__((vector_size(8)));

Q q1 = {1, 2}, q2 = {3, 4}, q3 = {5, 6}, q4 = {7, 8};

Q w1, w2, w3, w4;
Q z1, z2, z3, z4;

volatile int dummy;

void __attribute__((__noinline__)) func0(void) { dummy = 1; }

void __attribute__((__noinline__)) func1(void) {
  Q a, b;
  a  = q1 * q2;
  b  = q3 * q4;
  w1 = a;
  w2 = b;
  func0();
  w3 = a;
  w4 = b;
}

void __attribute__((__noinline__)) func2(void) {
  Q a, b;
  a  = q1 + q2;
  b  = q3 - q4;
  z1 = a;
  z2 = b;
  func1();
  z3 = a;
  z4 = b;
}

int main(void) {
  func2();

  if (__builtin_memcmp(&w1, &w3, sizeof(Q)) != 0)
    abort();
  if (__builtin_memcmp(&w2, &w4, sizeof(Q)) != 0)
    abort();
  if (__builtin_memcmp(&z1, &z3, sizeof(Q)) != 0)
    abort();
  if (__builtin_memcmp(&z2, &z4, sizeof(Q)) != 0)
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
// DEFAULT-NEXT:     type @type0 Q = vector<i16, 4>;
// DEFAULT-NEXT:     global %2 q1: vector<i16, 4> [storage=static] = aggregate<vector<i16, 4>, zero_fill=true>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(2))) [linkage=external];
// DEFAULT-NEXT:     global %3 q2: vector<i16, 4> [storage=static] = aggregate<vector<i16, 4>, zero_fill=true>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(3)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(4))) [linkage=external];
// DEFAULT-NEXT:     global %4 q3: vector<i16, 4> [storage=static] = aggregate<vector<i16, 4>, zero_fill=true>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(5)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(6))) [linkage=external];
// DEFAULT-NEXT:     global %5 q4: vector<i16, 4> [storage=static] = aggregate<vector<i16, 4>, zero_fill=true>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(7)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(8))) [linkage=external];
// DEFAULT-NEXT:     global %6 w1: vector<i16, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 w2: vector<i16, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 w3: vector<i16, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 w4: vector<i16, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 z1: vector<i16, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 z2: vector<i16, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 z3: vector<i16, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 z4: vector<i16, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 dummy: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %15 @func0() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, volatile>(%14, const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @func1() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %17 a: vector<i16, 4> [storage=automatic];
// DEFAULT-NEXT:         let %18 b: vector<i16, 4> [storage=automatic];
// DEFAULT-NEXT:         write<vector<i16, 4>>(%17, mul<vector<i16, 4>, elementwise=true, overflow=wrap>(read<vector<i16, 4>>(%2), read<vector<i16, 4>>(%3)));
// DEFAULT-NEXT:         write<vector<i16, 4>>(%18, mul<vector<i16, 4>, elementwise=true, overflow=wrap>(read<vector<i16, 4>>(%4), read<vector<i16, 4>>(%5)));
// DEFAULT-NEXT:         write<vector<i16, 4>>(%6, read<vector<i16, 4>>(%17));
// DEFAULT-NEXT:         write<vector<i16, 4>>(%7, read<vector<i16, 4>>(%18));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         write<vector<i16, 4>>(%8, read<vector<i16, 4>>(%17));
// DEFAULT-NEXT:         write<vector<i16, 4>>(%9, read<vector<i16, 4>>(%18));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @func2() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %20 a: vector<i16, 4> [storage=automatic];
// DEFAULT-NEXT:         let %21 b: vector<i16, 4> [storage=automatic];
// DEFAULT-NEXT:         write<vector<i16, 4>>(%20, add<vector<i16, 4>, elementwise=true, overflow=wrap>(read<vector<i16, 4>>(%2), read<vector<i16, 4>>(%3)));
// DEFAULT-NEXT:         write<vector<i16, 4>>(%21, sub<vector<i16, 4>, elementwise=true, overflow=wrap>(read<vector<i16, 4>>(%4), read<vector<i16, 4>>(%5)));
// DEFAULT-NEXT:         write<vector<i16, 4>>(%10, read<vector<i16, 4>>(%20));
// DEFAULT-NEXT:         write<vector<i16, 4>>(%11, read<vector<i16, 4>>(%21));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         write<vector<i16, 4>>(%12, read<vector<i16, 4>>(%20));
// DEFAULT-NEXT:         write<vector<i16, 4>>(%13, read<vector<i16, 4>>(%21));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%19);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i16, 4>>>(%6)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i16, 4>>>(%8)), const<u64>(8)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i16, 4>>>(%7)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i16, 4>>>(%9)), const<u64>(8)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i16, 4>>>(%10)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i16, 4>>>(%12)), const<u64>(8)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i16, 4>>>(%11)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i16, 4>>>(%13)), const<u64>(8)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
