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
// DEFAULT-NEXT:     type @type[[TYPE_v4qi:[0-9]+]] v4qi = vector<u8, 4>;
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_v:[0-9]+]] v: vector<u8, 4>) -> vector<u8, 4> [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(coerce<i32>) -> coerce<i32>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<vector<u8, 4>, elementwise=true, by_zero=ub>(read<vector<u8, 4>>(%[[VALUE_v]]), vector_splat<vector<u8, 4>, reason=usual_arith>(reinterpret<u8, reason=usual_arith, fits=unknown>(truncate<i8, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_v_2:[0-9]+]] v: vector<u8, 4>) -> vector<u8, 4> [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(coerce<i32>) -> coerce<i32>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<vector<u8, 4>, elementwise=true, by_zero=ub>(read<vector<u8, 4>>(%[[VALUE_v_2]]), read<vector<u8, 4>>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<vector<u8, 4>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_x:[0-9]+]] x: vector<u8, 4>, %[[VALUE_y:[0-9]+]] y: vector<u8, 4>) -> vector<u8, 4> [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(coerce<i32>, coerce<i32>) -> coerce<i32>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<vector<u8, 4>, elementwise=true, by_zero=ub>(read<vector<u8, 4>>(%[[VALUE_x]]), read<vector<u8, 4>>(%[[VALUE_y]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcmp:[0-9]+]] @__builtin_memcmp(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: vector<u8, 4> [storage=automatic] = aggregate<vector<u8, 4>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: vector<u8, 4> [storage=automatic] = aggregate<vector<u8, 4>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: vector<u8, 4> [storage=automatic] = call<vector<u8, 4>, signature=fn(vector<u8, 4>) -> vector<u8, 4>, abi=sysv64(coerce<i32>) -> coerce<i32>>(%[[VALUE_f1]], read<vector<u8, 4>>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<u8, 4>>>(%[[VALUE_y_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<u8, 4>>>(%[[VALUE_z]])), const<u64>(4)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<vector<u8, 4>>(%[[VALUE_z]], call<vector<u8, 4>, signature=fn(vector<u8, 4>) -> vector<u8, 4>, abi=sysv64(coerce<i32>) -> coerce<i32>>(%[[VALUE_f2]], read<vector<u8, 4>>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:         call<vector<u8, 4>, signature=fn(vector<u8, 4>) -> vector<u8, 4>, abi=sysv64(coerce<i32>) -> coerce<i32>>(%[[VALUE_f2]], read<vector<u8, 4>>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<u8, 4>>>(%[[VALUE_y_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<u8, 4>>>(%[[VALUE_z]])), const<u64>(4)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<vector<u8, 4>>(%[[VALUE_z]], call<vector<u8, 4>, signature=fn(vector<u8, 4>, vector<u8, 4>) -> vector<u8, 4>, abi=sysv64(coerce<i32>, coerce<i32>) -> coerce<i32>>(%[[VALUE_f3]], read<vector<u8, 4>>(%[[VALUE_x_2]]), read<vector<u8, 4>>(%[[VALUE_y_2]])));
// DEFAULT-NEXT:         call<vector<u8, 4>, signature=fn(vector<u8, 4>, vector<u8, 4>) -> vector<u8, 4>, abi=sysv64(coerce<i32>, coerce<i32>) -> coerce<i32>>(%[[VALUE_f3]], read<vector<u8, 4>>(%[[VALUE_x_2]]), read<vector<u8, 4>>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<u8, 4>>>(%[[VALUE_y_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<u8, 4>>>(%[[VALUE_z]])), const<u64>(4)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
