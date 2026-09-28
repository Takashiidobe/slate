#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>

static int bounded_cmp(const char *a, const char *b, size_t n) {
  return strncmp(a, b, n);
}

static size_t bounded_len(const char *s, size_t n) { return strnlen(s, n); }

static size_t spans(const char *s, const char *set) {
  return strspn(s, set) * 100 + strcspn(s, set);
}

int main(void) {
  const char abc[]    = "abc";
  const char abd[]    = "abd";
  const char text[]   = "abcdef";
  const char span[]   = "abacad";
  const char accept[] = "ab";
  const char empty[]  = "";
  const char reject[] = "cd";
  printf("%d %d %d %zu %zu %zu %zu %zu %zu\n", bounded_cmp(abc, abd, 2) == 0,
         bounded_cmp(abc, abd, 3) < 0, bounded_cmp(abc, abd, 0) == 0,
         bounded_len(abc, 99), bounded_len(text, 3), bounded_len(empty, 7),
         spans(span, accept), spans(span, empty), spans(empty, reject));
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %34 .str34: array<i8, 34> [storage=static] = code_units<array<i8, 34>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 122, 117, 32, 37, 122, 117, 32, 37, 122, 117, 32, 37, 122, 117, 32, 37, 122, 117, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%24 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @strncmp(%25 __s1: ptr<const i8>, %26 __s2: ptr<const i8>, %27 __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %3 @strcspn(%28 __s: ptr<const i8>, %29 __reject: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %4 @strspn(%30 __s: ptr<const i8>, %31 __accept: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %5 @strnlen(%32 __string: ptr<const i8>, %33 __maxlen: u64) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %6 @bounded_cmp(%7 a: ptr<const i8>, %8 b: ptr<const i8>, %9 n: u64) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%2, read<ptr<const i8>>(%7), read<ptr<const i8>>(%8), read<u64>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @bounded_len(%11 s: ptr<const i8>, %12 n: u64) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%5, read<ptr<const i8>>(%11), read<u64>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @spans(%14 s: ptr<const i8>, %15 set: ptr<const i8>) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u64, overflow=wrap>(mul<u64, overflow=wrap>(call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%4, read<ptr<const i8>>(%14), read<ptr<const i8>>(%15)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(100)))), call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%3, read<ptr<const i8>>(%14), read<ptr<const i8>>(%15)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %17 abc: array<i8, 4> [storage=automatic] [const] = code_units<array<i8, 4>>([97, 98, 99, 0]);
// DEFAULT-NEXT:         let %18 abd: array<i8, 4> [storage=automatic] [const] = code_units<array<i8, 4>>([97, 98, 100, 0]);
// DEFAULT-NEXT:         let %19 text: array<i8, 7> [storage=automatic] [const] = code_units<array<i8, 7>>([97, 98, 99, 100, 101, 102, 0]);
// DEFAULT-NEXT:         let %20 span: array<i8, 7> [storage=automatic] [const] = code_units<array<i8, 7>>([97, 98, 97, 99, 97, 100, 0]);
// DEFAULT-NEXT:         let %21 accept: array<i8, 3> [storage=automatic] [const] = code_units<array<i8, 3>>([97, 98, 0]);
// DEFAULT-NEXT:         let %22 empty: array<i8, 1> [storage=automatic] [const] = code_units<array<i8, 1>>([0]);
// DEFAULT-NEXT:         let %23 reject: array<i8, 3> [storage=automatic] [const] = code_units<array<i8, 3>>([99, 100, 0]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(34)>(%34)), from_bool<i32, reason=vararg>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%6, array_decay<ptr<const i8>, length=Some(4)>(%17), array_decay<ptr<const i8>, length=Some(4)>(%18), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))), const<i32>(0))), from_bool<i32, reason=vararg>(lt<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%6, array_decay<ptr<const i8>, length=Some(4)>(%17), array_decay<ptr<const i8>, length=Some(4)>(%18), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))), const<i32>(0))), from_bool<i32, reason=vararg>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%6, array_decay<ptr<const i8>, length=Some(4)>(%17), array_decay<ptr<const i8>, length=Some(4)>(%18), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))), const<i32>(0))), call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%10, array_decay<ptr<const i8>, length=Some(4)>(%17), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(99)))), call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%10, array_decay<ptr<const i8>, length=Some(7)>(%19), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))), call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%10, array_decay<ptr<const i8>, length=Some(1)>(%22), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))), call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%13, array_decay<ptr<const i8>, length=Some(7)>(%20), array_decay<ptr<const i8>, length=Some(3)>(%21)), call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%13, array_decay<ptr<const i8>, length=Some(7)>(%20), array_decay<ptr<const i8>, length=Some(1)>(%22)), call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%13, array_decay<ptr<const i8>, length=Some(1)>(%22), array_decay<ptr<const i8>, length=Some(3)>(%23)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
