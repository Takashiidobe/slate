/* PR tree-optimization/60960 */

typedef unsigned char v4qi __attribute__((vector_size(4)));

__attribute__((noinline, noclone)) v4qi f1(v4qi v) { return v / 2; }

__attribute__((noinline, noclone)) v4qi f2(v4qi v) {
  return v / (v4qi){2, 2, 2, 2};
}

__attribute__((noinline, noclone)) v4qi f3(v4qi x, v4qi y) { return x / y; }

int main() {
  v4qi x = {5, 5, 5, 5};
  v4qi y = {2, 2, 2, 2};
  v4qi z = f1(x);
  if (__builtin_memcmp(&y, &z, sizeof(y)) != 0)
    __builtin_abort();
  z = f2(x);
  if (__builtin_memcmp(&y, &z, sizeof(y)) != 0)
    __builtin_abort();
  z = f3(x, y);
  if (__builtin_memcmp(&y, &z, sizeof(y)) != 0)
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
// DEFAULT-NEXT:     type @type0 v4qi = vector<u8, 4>;
// DEFAULT-NEXT:     fn %1 @f1(%2 v: vector<u8, 4>) -> vector<u8, 4> [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(coerce<i32>) -> coerce<i32>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<vector<u8, 4>, elementwise=true, by_zero=ub>(read<vector<u8, 4>>(%2), vector_splat<vector<u8, 4>, reason=usual_arith>(reinterpret<u8, reason=usual_arith, fits=unknown>(truncate<i8, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @f2(%4 v: vector<u8, 4>) -> vector<u8, 4> [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(coerce<i32>) -> coerce<i32>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<vector<u8, 4>, elementwise=true, by_zero=ub>(read<vector<u8, 4>>(%4), read<vector<u8, 4>>(compound_literal %12 [storage=automatic] = aggregate<vector<u8, 4>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @f3(%6 x: vector<u8, 4>, %7 y: vector<u8, 4>) -> vector<u8, 4> [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(coerce<i32>, coerce<i32>) -> coerce<i32>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<vector<u8, 4>, elementwise=true, by_zero=ub>(read<vector<u8, 4>>(%6), read<vector<u8, 4>>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @__builtin_memcmp(%13 <unnamed>: ptr<const void>, %14 <unnamed>: ptr<const void>, %15 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %17 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 x: vector<u8, 4> [storage=automatic] = aggregate<vector<u8, 4>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         let %10 y: vector<u8, 4> [storage=automatic] = aggregate<vector<u8, 4>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         let %11 z: vector<u8, 4> [storage=automatic] = call<vector<u8, 4>, signature=fn(vector<u8, 4>) -> vector<u8, 4>, abi=sysv64(coerce<i32>) -> coerce<i32>>(%1, read<vector<u8, 4>>(%9));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%16, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<u8, 4>>>(%10)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<u8, 4>>>(%11)), const<u64>(4)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         write<vector<u8, 4>>(%11, call<vector<u8, 4>, signature=fn(vector<u8, 4>) -> vector<u8, 4>, abi=sysv64(coerce<i32>) -> coerce<i32>>(%3, read<vector<u8, 4>>(%9)));
// DEFAULT-NEXT:         call<vector<u8, 4>, signature=fn(vector<u8, 4>) -> vector<u8, 4>, abi=sysv64(coerce<i32>) -> coerce<i32>>(%3, read<vector<u8, 4>>(%9));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%16, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<u8, 4>>>(%10)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<u8, 4>>>(%11)), const<u64>(4)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         write<vector<u8, 4>>(%11, call<vector<u8, 4>, signature=fn(vector<u8, 4>, vector<u8, 4>) -> vector<u8, 4>, abi=sysv64(coerce<i32>, coerce<i32>) -> coerce<i32>>(%5, read<vector<u8, 4>>(%9), read<vector<u8, 4>>(%10)));
// DEFAULT-NEXT:         call<vector<u8, 4>, signature=fn(vector<u8, 4>, vector<u8, 4>) -> vector<u8, 4>, abi=sysv64(coerce<i32>, coerce<i32>) -> coerce<i32>>(%5, read<vector<u8, 4>>(%9), read<vector<u8, 4>>(%10));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%16, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<u8, 4>>>(%10)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<u8, 4>>>(%11)), const<u64>(4)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
