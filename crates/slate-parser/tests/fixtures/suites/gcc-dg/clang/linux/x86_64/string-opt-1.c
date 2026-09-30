/* { dg-do compile } */
/* { dg-options "-O2" } */

char *buffer;
char *test;

#define SIZE 100

char *
__attribute__((noinline))
my_memcpy (char *d, char *s, unsigned l)
{
  return __builtin_memcpy (d, s, l);
}

char *
__attribute__((noinline))
my_mempcpy (char *d, char *s, unsigned l)
{
  return __builtin_mempcpy (d, s, l);
}

void
run_test (char *d, char *s, unsigned l)
{
  char *r = my_mempcpy (d, s, l);
  if (r != d + l)
    __builtin_abort ();

  r = my_memcpy (d, s, l);
  if (r != d)
    __builtin_abort ();
}

int
main (void)
{
  const char* const foo = "hello world";
  unsigned l = __builtin_strlen (foo) + 1;

  buffer = __builtin_malloc (SIZE);
  __builtin_memcpy (buffer, foo, l);
  test = __builtin_malloc (SIZE);

  run_test (test, buffer, l);

  return 0;
}

/* { dg-final { scan-assembler "mempcpy" } } */
/* { dg-final { scan-assembler "memcpy" } } */

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %[[VALUE_buffer:[0-9]+]] buffer: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_test:[0-9]+]] test: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_my_memcpy:[0-9]+]] @my_memcpy(%[[VALUE_d:[0-9]+]] d: ptr<i8>, %[[VALUE_s:[0-9]+]] s: ptr<i8>, %[[VALUE_l:[0-9]+]] l: u32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<i8>, reason=return>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_d]])), pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), widen<u64, reason=arg>(read<u32>(%[[VALUE_l]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_mempcpy:[0-9]+]] @__builtin_mempcpy(%[[VALUE3:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE4:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE5:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_my_mempcpy:[0-9]+]] @my_mempcpy(%[[VALUE_d_2:[0-9]+]] d: ptr<i8>, %[[VALUE_s_2:[0-9]+]] s: ptr<i8>, %[[VALUE_l_2:[0-9]+]] l: u32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<i8>, reason=return>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_mempcpy]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_d_2]])), pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%[[VALUE_s_2]])), widen<u64, reason=arg>(read<u32>(%[[VALUE_l_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_run_test:[0-9]+]] @run_test(%[[VALUE_d_3:[0-9]+]] d: ptr<i8>, %[[VALUE_s_3:[0-9]+]] s: ptr<i8>, %[[VALUE_l_3:[0-9]+]] l: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<i8>, ptr<i8>, u32) -> ptr<i8>>(%[[VALUE_my_mempcpy]], read<ptr<i8>>(%[[VALUE_d_3]]), read<ptr<i8>>(%[[VALUE_s_3]]), read<u32>(%[[VALUE_l_3]]));
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_r]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_d_3]]), read<u32>(%[[VALUE_l_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_r]], call<ptr<i8>, signature=fn(ptr<i8>, ptr<i8>, u32) -> ptr<i8>>(%[[VALUE_my_memcpy]], read<ptr<i8>>(%[[VALUE_d_3]]), read<ptr<i8>>(%[[VALUE_s_3]]), read<u32>(%[[VALUE_l_3]])));
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_r]]), read<ptr<i8>>(%[[VALUE_d_3]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strlen:[0-9]+]] @__builtin_strlen(%[[VALUE6:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_malloc:[0-9]+]] @__builtin_malloc(%[[VALUE7:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_foo:[0-9]+]] foo: ptr<const i8> [storage=automatic] [const] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str]]));
// DEFAULT-NEXT:         let %[[VALUE_l_4:[0-9]+]] l: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE___builtin_strlen]], read<ptr<const i8>>(%[[VALUE_foo]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_buffer]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(100))))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_buffer]])), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%[[VALUE_foo]])), widen<u64, reason=arg>(read<u32>(%[[VALUE_l_4]])));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_test]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(100))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ptr<i8>, u32) -> void>(%[[VALUE_run_test]], read<ptr<i8>>(%[[VALUE_test]]), read<ptr<i8>>(%[[VALUE_buffer]]), read<u32>(%[[VALUE_l_4]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
