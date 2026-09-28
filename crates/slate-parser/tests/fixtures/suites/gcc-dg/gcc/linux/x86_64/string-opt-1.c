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
// DEFAULT-NEXT:     global %0 buffer: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 test: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %21 @__builtin_memcpy(%18 <unnamed>: ptr<void>, %19 <unnamed>: ptr<const void>, %20 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @my_memcpy(%3 d: ptr<i8>, %4 s: ptr<i8>, %5 l: u32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<i8>, reason=return>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%21, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%3)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%4)), widen<u64, reason=arg>(read<u32>(%5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @__builtin_mempcpy(%22 <unnamed>: ptr<void>, %23 <unnamed>: ptr<const void>, %24 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @my_mempcpy(%7 d: ptr<i8>, %8 s: ptr<i8>, %9 l: u32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<i8>, reason=return>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%25, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%7)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%8)), widen<u64, reason=arg>(read<u32>(%9))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @run_test(%11 d: ptr<i8>, %12 s: ptr<i8>, %13 l: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %14 r: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<i8>, ptr<i8>, u32) -> ptr<i8>>(%6, read<ptr<i8>>(%11), read<ptr<i8>>(%12), read<u32>(%13));
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%14), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%11), read<u32>(%13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%26);
// DEFAULT-NEXT:         write<ptr<i8>>(%14, call<ptr<i8>, signature=fn(ptr<i8>, ptr<i8>, u32) -> ptr<i8>>(%2, read<ptr<i8>>(%11), read<ptr<i8>>(%12), read<u32>(%13)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<i8>, u32) -> ptr<i8>>(%2, read<ptr<i8>>(%11), read<ptr<i8>>(%12), read<u32>(%13));
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%14), read<ptr<i8>>(%11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%26);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @__builtin_strlen(%28 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %31 @__builtin_malloc(%30 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %16 foo: ptr<const i8> [storage=automatic] [const] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(12)>(%27));
// DEFAULT-NEXT:         let %17 l: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(call<u64, signature=fn(ptr<const i8>) -> u64>(%29, read<ptr<const i8>>(%16)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         write<ptr<i8>>(%0, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%31, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(100))))));
// DEFAULT-NEXT:         pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%31, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(100)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%21, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%0)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%16)), widen<u64, reason=arg>(read<u32>(%17)));
// DEFAULT-NEXT:         write<ptr<i8>>(%1, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%31, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(100))))));
// DEFAULT-NEXT:         pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%31, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(100)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ptr<i8>, u32) -> void>(%10, read<ptr<i8>>(%1), read<ptr<i8>>(%0), read<u32>(%17));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
