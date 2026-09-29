void abort(void);

int sub1(int i, int j) {
  typedef int c[i + 2];
  int         x[10], y[10];

  if (j == 2) {
    __builtin_memcpy(x, y, 10 * sizeof(int));
    return sizeof(c);
  } else
    return sizeof(c) * 3;
}

int main() {
  if (sub1(20, 3) != 66 * sizeof(int))
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
// DEFAULT-NEXT:     type @type[[TYPE_c:[0-9]+]] c = vla<i32, %[[VALUE0:[0-9]+]]>;
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sub1:[0-9]+]] @sub1(%[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_j:[0-9]+]] j: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(2))));
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: array<i32, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: array<i32, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_j]]), const<i32>(2))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_x]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_y]])), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))), const<u64>(4)));
// DEFAULT-NEXT:                 return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE0]]), const<u64>(4))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(mul<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE0]]), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_sub1]], const<i32>(20), const<i32>(3)))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(66))), const<u64>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
