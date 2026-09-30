/* Test builtin-memcpy (which may emit different code for different N).  */
#include <string.h>

void abort(void);

#define TESTSIZE 80

char src[TESTSIZE] __attribute__((aligned));
char dst[TESTSIZE] __attribute__((aligned));

void check(char *test, char *match, int n) {
  if (memcmp(test, match, n))
    abort();
}

#define TN(n)                                                                  \
  {                                                                            \
    memset(dst, 0, n);                                                         \
    memcpy(dst, src, n);                                                       \
    check(dst, src, n);                                                        \
  }
#define T(n)                                                                   \
  TN(n)                                                                        \
  TN((n) + 1)                                                                  \
  TN((n) + 2)                                                                  \
  TN((n) + 3)

int main(void) {
  int i, j;

  for (i = 0; i < sizeof(src); ++i)
    src[i] = 'a' + i % 26;

  T(0);
  T(4);
  T(8);
  T(12);
  T(16);
  T(20);
  T(24);
  T(28);
  T(32);
  T(36);
  T(40);
  T(44);
  T(48);
  T(52);
  T(56);
  T(60);
  T(64);
  T(68);
  T(72);
  T(76);

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
// DEFAULT-NEXT:     global %[[VALUE_src:[0-9]+]] src: array<i8, 80> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_dst:[0-9]+]] dst: array<i8, 80> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE___dest:[0-9]+]] __dest: ptr<void> [restrict], %[[VALUE___src:[0-9]+]] __src: ptr<const void> [restrict], %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE___s:[0-9]+]] __s: ptr<void>, %[[VALUE___c:[0-9]+]] __c: i32, %[[VALUE___n_2:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memcmp:[0-9]+]] @memcmp(%[[VALUE___s1:[0-9]+]] __s1: ptr<const void>, %[[VALUE___s2:[0-9]+]] __s2: ptr<const void>, %[[VALUE___n_3:[0-9]+]] __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_check:[0-9]+]] @check(%[[VALUE_test:[0-9]+]] test: ptr<i8>, %[[VALUE_match:[0-9]+]] match: ptr<i8>, %[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%[[VALUE_test]])), pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%[[VALUE_match]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_n]])))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), const<u64>(80))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), read<i32>(%[[VALUE_i]]))), truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(const<i32>(97), rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(26)))));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(0), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(0), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(0), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(0), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(0), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(0), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(0), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(0), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(4));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(4), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(4), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(4), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(4), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(4), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(4), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(4), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(4), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(4), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(8));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(8), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(8), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(8), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(8), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(8), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(8), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(8), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(8), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(8), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(12));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(12), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(12), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(12), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(16));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(16), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(16), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(16), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(16), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(16), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(16), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(16), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(16), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(16), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(20))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(20))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(20));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(20), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(20), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(20), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(20), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(20), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(20), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(20), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(20), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(20), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(24))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(24))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(24));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(24), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(24), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(24), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(24), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(24), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(24), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(24), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(24), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(24), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(28))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(28))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(28));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(28), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(28), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(28), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(28), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(28), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(28), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(28), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(28), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(28), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(32));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(32), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(32), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(32), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(32), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(32), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(32), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(32), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(32), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(32), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(36))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(36))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(36));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(36), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(36), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(36), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(36), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(36), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(36), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(36), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(36), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(36), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(40))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(40))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(40));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(40), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(40), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(40), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(40), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(40), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(40), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(40), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(40), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(40), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(44))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(44))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(44));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(44), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(44), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(44), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(44), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(44), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(44), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(44), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(44), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(44), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(48))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(48))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(48));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(48), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(48), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(48), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(48), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(48), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(48), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(48), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(48), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(48), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(52))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(52))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(52));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(52), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(52), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(52), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(52), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(52), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(52), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(52), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(52), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(52), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(56))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(56))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(56));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(56), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(56), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(56), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(56), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(56), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(56), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(56), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(56), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(56), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(60))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(60))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(60));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(60), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(60), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(60), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(60), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(60), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(60), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(60), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(60), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(60), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(64));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(64), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(64), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(64), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(64), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(64), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(64), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(64), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(64), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(64), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(68))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(68))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(68));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(68), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(68), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(68), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(68), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(68), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(68), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(68), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(68), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(68), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(72))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(72))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(72));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(72), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(72), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(72), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(72), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(72), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(72), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(72), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(72), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(72), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(76))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(76))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), const<i32>(76));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(76), const<i32>(1)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(76), const<i32>(1)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(76), const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(76), const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(76), const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(76), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(76), const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(76), const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>, ptr<i8>, i32) -> void>(%[[VALUE_check]], array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_dst]]), array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_src]]), add<i32, overflow=ub>(const<i32>(76), const<i32>(3)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
