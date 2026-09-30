/* { dg-require-effective-target return_address } */
void abort(void);

int check_fa_work(const char *, const char *) __attribute__((noinline, noipa));
int check_fa_mid(const char *) __attribute__((noinline, noipa));
int check_fa(char *) __attribute__((noinline, noipa));
int how_much(void) __attribute__((noinline, noipa));

int check_fa_work(const char *c, const char *f) {
  const char d = 0;

  if (c >= &d)
    return c >= f && f >= &d;
  else
    return c <= f && f <= &d;
}

int check_fa_mid(const char *c) {
  const char *f = __builtin_frame_address(0);

  /* Prevent a tail call to check_fa_work, eliding the current stack frame.  */
  return check_fa_work(c, f) != 0;
}

int check_fa(char *unused) {
  const char c = 0;

  /* Prevent a tail call to check_fa_mid, eliding the current stack frame.  */
  return check_fa_mid(&c) != 0;
}

int how_much(void) { return 8; }

int main(void) {
  char *unused = __builtin_alloca(how_much());

  if (!check_fa(unused))
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_check_fa_work:[0-9]+]] @check_fa_work(%[[VALUE_c:[0-9]+]] c: ptr<const i8>, %[[VALUE_f:[0-9]+]] f: ptr<const i8>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i8 [storage=automatic] [const] = truncate<i8, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         if ge<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_c]]), addr_of<ptr<const i8>>(%[[VALUE_d]]))
// DEFAULT-NEXT:             return from_bool<i32, reason=return>(logical_and<bool>(ge<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_c]]), read<ptr<const i8>>(%[[VALUE_f]])), ge<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_f]]), addr_of<ptr<const i8>>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return from_bool<i32, reason=return>(logical_and<bool>(le<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_c]]), read<ptr<const i8>>(%[[VALUE_f]])), le<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_f]]), addr_of<ptr<const i8>>(%[[VALUE_d]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check_fa_mid:[0-9]+]] @check_fa_mid(%[[VALUE_c_2:[0-9]+]] c: ptr<const i8>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_f_2:[0-9]+]] f: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE___builtin_frame_address:[0-9]+]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_check_fa_work]], read<ptr<const i8>>(%[[VALUE_c_2]]), read<ptr<const i8>>(%[[VALUE_f_2]])), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check_fa:[0-9]+]] @check_fa(%[[VALUE_unused:[0-9]+]] unused: ptr<i8>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c_3:[0-9]+]] c: i8 [storage=automatic] [const] = truncate<i8, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_check_fa_mid]], addr_of<ptr<const i8>>(%[[VALUE_c_3]])), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_how_much:[0-9]+]] @how_much() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_frame_address]] @__builtin_frame_address(%[[VALUE0:[0-9]+]] <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_alloca:[0-9]+]] @__builtin_alloca(%[[VALUE1:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_unused_2:[0-9]+]] unused: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(call<i32, signature=fn() -> i32>(%[[VALUE_how_much]])))));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_check_fa]], read<ptr<i8>>(%[[VALUE_unused_2]])), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
