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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @check_fa_work(%5 c: ptr<const i8>, %6 f: ptr<const i8>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 d: i8 [storage=automatic] [const] = truncate<i8, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         if ge<ptr<const i8>>(read<ptr<const i8>>(%5), addr_of<ptr<const i8>>(%7))
// DEFAULT-NEXT:             return from_bool<i32, reason=return>(logical_and<bool>(ge<ptr<const i8>>(read<ptr<const i8>>(%5), read<ptr<const i8>>(%6)), ge<ptr<const i8>>(read<ptr<const i8>>(%6), addr_of<ptr<const i8>>(%7))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return from_bool<i32, reason=return>(logical_and<bool>(le<ptr<const i8>>(read<ptr<const i8>>(%5), read<ptr<const i8>>(%6)), le<ptr<const i8>>(read<ptr<const i8>>(%6), addr_of<ptr<const i8>>(%7))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @check_fa_mid(%8 c: ptr<const i8>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 f: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(call<ptr<void>, signature=fn(u32) -> ptr<void>>(__builtin_frame_address, reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%1, read<ptr<const i8>>(%8), read<ptr<const i8>>(%9)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @check_fa(%10 unused: ptr<i8>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 c: i8 [storage=automatic] [const] = truncate<i8, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%2, addr_of<ptr<const i8>>(%11)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @how_much() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 unused: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(__builtin_alloca, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(call<i32, signature=fn() -> i32>(%4)))));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i8>) -> i32>(%3, read<ptr<i8>>(%13)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
