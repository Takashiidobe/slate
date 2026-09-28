/* PR tree-optimization/80153 */

void check(int, int, int) __attribute__((noinline));
void check(int c, int c2, int val) {
  if (!val) {
    __builtin_abort();
  }
}

static const char *buf;
static int         l, i;

void _fputs(const char *str) __attribute__((noinline));
void _fputs(const char *str) {
  buf = str;
  i   = 0;
  l   = __builtin_strlen(buf);
}

char _fgetc() __attribute__((noinline));
char _fgetc() {
  char val = buf[i];
  i++;
  if (i > l)
    return -1;
  else
    return val;
}

static const char *string = "oops!\n";

int main(void) {
  int i;
  int c;

  _fputs(string);

  for (i = 0; i < __builtin_strlen(string); i++) {
    c = _fgetc();
    check(c, string[i], c == string[i]);
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
// DEFAULT-NEXT:     global %4 buf: ptr<const i8> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %5 l: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %6 i: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([111, 111, 112, 115, 33, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 string: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%22)) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @check(%1 c: i32, %2 c2: i32, %3 val: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%3), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%18);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @_fputs(%8 str: ptr<const i8>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<const i8>>(%4, read<ptr<const i8>>(%8));
// DEFAULT-NEXT:         write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%5, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%21, read<ptr<const i8>>(%4)))));
// DEFAULT-NEXT:         reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%21, read<ptr<const i8>>(%4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @__builtin_strlen(%20 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %9 @_fgetc() -> i8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 val: i8 [storage=automatic] = read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%4), read<i32>(%6))));
// DEFAULT-NEXT:         let %24: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%25));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%6), read<i32>(%5))
// DEFAULT-NEXT:             return truncate<i8, reason=return, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return read<i8>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %14 c: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>) -> void>(%7, read<ptr<const i8>>(%11));
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%13, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%13))), call<u64, signature=fn(ptr<const i8>) -> u64>(%21, read<ptr<const i8>>(%11)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%27));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%14, widen<i32, reason=assign>(call<i8, signature=fn() -> i8>(%9)));
// DEFAULT-NEXT:                     widen<i32, reason=assign>(call<i8, signature=fn() -> i8>(%9));
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%0, read<i32>(%14), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%11), read<i32>(%13))))), from_bool<i32, reason=arg>(eq<i32>(read<i32>(%14), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%11), read<i32>(%13))))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
