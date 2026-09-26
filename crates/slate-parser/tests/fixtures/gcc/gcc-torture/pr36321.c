/* See 'gcc.target/nvptx/__builtin_alloca_0-1-O0.c'.
   { dg-xfail-if TODO { nvptx-*-* && { ! nvptx_softstack } } { "-O0" } { "" } }
 */

extern void abort(void);

extern __SIZE_TYPE__ strlen(const char *);
void                 foo(char *str) {
  int   len2 = strlen(str);
  char *a    = (char *)__builtin_alloca(0);
  char *b    = (char *)__builtin_alloca(len2 * 3);

  if ((int)(a - b) < (len2 * 3)) {
#ifdef _WIN32
    abort();
#endif
    return;
  }
}

static char *volatile argp = "pr36321.x";

int main(int argc, char **argv) {
  foo(argp);
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
// DEFAULT-NEXT:     global %12 .str12: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([112, 114, 51, 54, 51, 50, 49, 46, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 argp: volatile ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(10)>(%12) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @strlen(%11 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 str: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 len2: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(strlen, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%3)))));
// DEFAULT-NEXT:         let %5 a: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(__builtin_alloca, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         let %6 b: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(__builtin_alloca, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(mul<i32, overflow=ub>(read<i32>(%4), const<i32>(3))))));
// DEFAULT-NEXT:         if lt<i32>(truncate<i32, reason=explicit, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%5), read<ptr<i8>>(%6))), mul<i32, overflow=ub>(read<i32>(%4), const<i32>(3)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main(%9 argc: i32, %10 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%2, read<ptr<i8>, volatile>(%7));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
