/* PR middle-end/37780.  */

#define VAL (8 * sizeof(int))

int __attribute__((noinline, noclone)) fooctz(int i) {
  return (i == 0) ? VAL : __builtin_ctz(i);
}

int __attribute__((noinline, noclone)) fooctz2(int i) {
  return (i != 0) ? __builtin_ctz(i) : VAL;
}

unsigned int __attribute__((noinline, noclone)) fooctz3(unsigned int i) {
  return (i > 0) ? __builtin_ctz(i) : VAL;
}

int __attribute__((noinline, noclone)) fooclz(int i) {
  return (i == 0) ? VAL : __builtin_clz(i);
}

int __attribute__((noinline, noclone)) fooclz2(int i) {
  return (i != 0) ? __builtin_clz(i) : VAL;
}

unsigned int __attribute__((noinline, noclone)) fooclz3(unsigned int i) {
  return (i > 0) ? __builtin_clz(i) : VAL;
}

int main(void) {
  if (fooctz(0) != VAL || fooctz2(0) != VAL || fooctz3(0) != VAL ||
      fooclz(0) != VAL || fooclz2(0) != VAL || fooclz3(0) != VAL)
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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ctz:[0-9]+]] @__builtin_ctz(%[[VALUE0:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fooctz:[0-9]+]] @fooctz(%[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(conditional<u64>(eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_i]]))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fooctz2:[0-9]+]] @fooctz2(%[[VALUE_i_2:[0-9]+]] i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(conditional<u64>(ne<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(0)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_i_2]]))))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fooctz3:[0-9]+]] @fooctz3(%[[VALUE_i_3:[0-9]+]] i: u32) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<u32, reason=return, fits=unknown>(conditional<u64>(gt<u32>(read<u32>(%[[VALUE_i_3]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], read<u32>(%[[VALUE_i_3]])))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clz:[0-9]+]] @__builtin_clz(%[[VALUE1:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fooclz:[0-9]+]] @fooclz(%[[VALUE_i_4:[0-9]+]] i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(conditional<u64>(eq<i32>(read<i32>(%[[VALUE_i_4]]), const<i32>(0)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_i_4]]))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fooclz2:[0-9]+]] @fooclz2(%[[VALUE_i_5:[0-9]+]] i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(conditional<u64>(ne<i32>(read<i32>(%[[VALUE_i_5]]), const<i32>(0)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_i_5]]))))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fooclz3:[0-9]+]] @fooclz3(%[[VALUE_i_6:[0-9]+]] i: u32) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<u32, reason=return, fits=unknown>(conditional<u64>(gt<u32>(read<u32>(%[[VALUE_i_6]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], read<u32>(%[[VALUE_i_6]])))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_fooctz]], const<i32>(0)))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_fooctz2]], const<i32>(0)))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], ne<u64>(widen<u64, reason=usual_arith>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_fooctz3]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_fooclz]], const<i32>(0)))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_fooclz2]], const<i32>(0)))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE5]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], ne<u64>(widen<u64, reason=usual_arith>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_fooclz3]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
