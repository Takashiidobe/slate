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
// DEFAULT-NEXT:     type @type[[TYPE_Q:[0-9]+]] Q = vector<i16, 4>;
// DEFAULT-NEXT:     global %[[VALUE_q1:[0-9]+]] q1: vector<i16, 4> [storage=static] = aggregate<vector<i16, 4>, zero_fill=true>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q2:[0-9]+]] q2: vector<i16, 4> [storage=static] = aggregate<vector<i16, 4>, zero_fill=true>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(3)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q3:[0-9]+]] q3: vector<i16, 4> [storage=static] = aggregate<vector<i16, 4>, zero_fill=true>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(5)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(6))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q4:[0-9]+]] q4: vector<i16, 4> [storage=static] = aggregate<vector<i16, 4>, zero_fill=true>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(7)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(8))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_w1:[0-9]+]] w1: vector<i16, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_w2:[0-9]+]] w2: vector<i16, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_w3:[0-9]+]] w3: vector<i16, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_w4:[0-9]+]] w4: vector<i16, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_z1:[0-9]+]] z1: vector<i16, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_z2:[0-9]+]] z2: vector<i16, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_z3:[0-9]+]] z3: vector<i16, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_z4:[0-9]+]] z4: vector<i16, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_dummy:[0-9]+]] dummy: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_func0:[0-9]+]] @func0() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_dummy]], const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func1:[0-9]+]] @func1() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: vector<i16, 4> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: vector<i16, 4> [storage=automatic];
// DEFAULT-NEXT:         write<vector<i16, 4>>(%[[VALUE_a]], mul<vector<i16, 4>, elementwise=true, overflow=wrap>(read<vector<i16, 4>>(%[[VALUE_q1]]), read<vector<i16, 4>>(%[[VALUE_q2]])));
// DEFAULT-NEXT:         write<vector<i16, 4>>(%[[VALUE_b]], mul<vector<i16, 4>, elementwise=true, overflow=wrap>(read<vector<i16, 4>>(%[[VALUE_q3]]), read<vector<i16, 4>>(%[[VALUE_q4]])));
// DEFAULT-NEXT:         write<vector<i16, 4>>(%[[VALUE_w1]], read<vector<i16, 4>>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<vector<i16, 4>>(%[[VALUE_w2]], read<vector<i16, 4>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_func0]]);
// DEFAULT-NEXT:         write<vector<i16, 4>>(%[[VALUE_w3]], read<vector<i16, 4>>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<vector<i16, 4>>(%[[VALUE_w4]], read<vector<i16, 4>>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func2:[0-9]+]] @func2() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: vector<i16, 4> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: vector<i16, 4> [storage=automatic];
// DEFAULT-NEXT:         write<vector<i16, 4>>(%[[VALUE_a_2]], add<vector<i16, 4>, elementwise=true, overflow=wrap>(read<vector<i16, 4>>(%[[VALUE_q1]]), read<vector<i16, 4>>(%[[VALUE_q2]])));
// DEFAULT-NEXT:         write<vector<i16, 4>>(%[[VALUE_b_2]], sub<vector<i16, 4>, elementwise=true, overflow=wrap>(read<vector<i16, 4>>(%[[VALUE_q3]]), read<vector<i16, 4>>(%[[VALUE_q4]])));
// DEFAULT-NEXT:         write<vector<i16, 4>>(%[[VALUE_z1]], read<vector<i16, 4>>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:         write<vector<i16, 4>>(%[[VALUE_z2]], read<vector<i16, 4>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_func1]]);
// DEFAULT-NEXT:         write<vector<i16, 4>>(%[[VALUE_z3]], read<vector<i16, 4>>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:         write<vector<i16, 4>>(%[[VALUE_z4]], read<vector<i16, 4>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcmp:[0-9]+]] @__builtin_memcmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_func2]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i16, 4>>>(%[[VALUE_w1]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i16, 4>>>(%[[VALUE_w3]])), const<u64>(8)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i16, 4>>>(%[[VALUE_w2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i16, 4>>>(%[[VALUE_w4]])), const<u64>(8)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i16, 4>>>(%[[VALUE_z1]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i16, 4>>>(%[[VALUE_z3]])), const<u64>(8)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i16, 4>>>(%[[VALUE_z2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i16, 4>>>(%[[VALUE_z4]])), const<u64>(8)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
