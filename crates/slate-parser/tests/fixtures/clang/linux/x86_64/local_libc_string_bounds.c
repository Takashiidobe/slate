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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 34> [storage=static] = code_units<array<i8, 34>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 122, 117, 32, 37, 122, 117, 32, 37, 122, 117, 32, 37, 122, 117, 32, 37, 122, 117, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strncmp:[0-9]+]] @strncmp(%[[VALUE___s1:[0-9]+]] __s1: ptr<const i8>, %[[VALUE___s2:[0-9]+]] __s2: ptr<const i8>, %[[VALUE___n:[0-9]+]] __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_strcspn:[0-9]+]] @strcspn(%[[VALUE___s:[0-9]+]] __s: ptr<const i8>, %[[VALUE___reject:[0-9]+]] __reject: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_strspn:[0-9]+]] @strspn(%[[VALUE___s_2:[0-9]+]] __s: ptr<const i8>, %[[VALUE___accept:[0-9]+]] __accept: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_strnlen:[0-9]+]] @strnlen(%[[VALUE___string:[0-9]+]] __string: ptr<const i8>, %[[VALUE___maxlen:[0-9]+]] __maxlen: u64) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_bounded_cmp:[0-9]+]] @bounded_cmp(%[[VALUE_a:[0-9]+]] a: ptr<const i8>, %[[VALUE_b:[0-9]+]] b: ptr<const i8>, %[[VALUE_n:[0-9]+]] n: u64) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%[[VALUE_strncmp]], read<ptr<const i8>>(%[[VALUE_a]]), read<ptr<const i8>>(%[[VALUE_b]]), read<u64>(%[[VALUE_n]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bounded_len:[0-9]+]] @bounded_len(%[[VALUE_s:[0-9]+]] s: ptr<const i8>, %[[VALUE_n_2:[0-9]+]] n: u64) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%[[VALUE_strnlen]], read<ptr<const i8>>(%[[VALUE_s]]), read<u64>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_spans:[0-9]+]] @spans(%[[VALUE_s_2:[0-9]+]] s: ptr<const i8>, %[[VALUE_set:[0-9]+]] set: ptr<const i8>) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u64, overflow=wrap>(mul<u64, overflow=wrap>(call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%[[VALUE_strspn]], read<ptr<const i8>>(%[[VALUE_s_2]]), read<ptr<const i8>>(%[[VALUE_set]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(100)))), call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%[[VALUE_strcspn]], read<ptr<const i8>>(%[[VALUE_s_2]]), read<ptr<const i8>>(%[[VALUE_set]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_abc:[0-9]+]] abc: array<i8, 4> [storage=automatic] [const] = code_units<array<i8, 4>>([97, 98, 99, 0]);
// DEFAULT-NEXT:         let %[[VALUE_abd:[0-9]+]] abd: array<i8, 4> [storage=automatic] [const] = code_units<array<i8, 4>>([97, 98, 100, 0]);
// DEFAULT-NEXT:         let %[[VALUE_text:[0-9]+]] text: array<i8, 7> [storage=automatic] [const] = code_units<array<i8, 7>>([97, 98, 99, 100, 101, 102, 0]);
// DEFAULT-NEXT:         let %[[VALUE_span:[0-9]+]] span: array<i8, 7> [storage=automatic] [const] = code_units<array<i8, 7>>([97, 98, 97, 99, 97, 100, 0]);
// DEFAULT-NEXT:         let %[[VALUE_accept:[0-9]+]] accept: array<i8, 3> [storage=automatic] [const] = code_units<array<i8, 3>>([97, 98, 0]);
// DEFAULT-NEXT:         let %[[VALUE_empty:[0-9]+]] empty: array<i8, 1> [storage=automatic] [const] = code_units<array<i8, 1>>([0]);
// DEFAULT-NEXT:         let %[[VALUE_reject:[0-9]+]] reject: array<i8, 3> [storage=automatic] [const] = code_units<array<i8, 3>>([99, 100, 0]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(34)>(%[[VALUE_str]])), from_bool<i32, reason=vararg>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%[[VALUE_bounded_cmp]], array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_abc]]), array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_abd]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))), const<i32>(0))), from_bool<i32, reason=vararg>(lt<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%[[VALUE_bounded_cmp]], array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_abc]]), array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_abd]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))), const<i32>(0))), from_bool<i32, reason=vararg>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%[[VALUE_bounded_cmp]], array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_abc]]), array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_abd]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))), const<i32>(0))), call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%[[VALUE_bounded_len]], array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_abc]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(99)))), call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%[[VALUE_bounded_len]], array_decay<ptr<const i8>, length=Some(7)>(%[[VALUE_text]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))), call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%[[VALUE_bounded_len]], array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_empty]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))), call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%[[VALUE_spans]], array_decay<ptr<const i8>, length=Some(7)>(%[[VALUE_span]]), array_decay<ptr<const i8>, length=Some(3)>(%[[VALUE_accept]])), call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%[[VALUE_spans]], array_decay<ptr<const i8>, length=Some(7)>(%[[VALUE_span]]), array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_empty]])), call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%[[VALUE_spans]], array_decay<ptr<const i8>, length=Some(1)>(%[[VALUE_empty]]), array_decay<ptr<const i8>, length=Some(3)>(%[[VALUE_reject]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
