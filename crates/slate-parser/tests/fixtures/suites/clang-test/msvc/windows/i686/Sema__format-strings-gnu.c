

int printf(const char *restrict, ...);
int scanf(const char * restrict, ...) ;

void test(void) {
  long notLongEnough = 1;
  long long quiteLong = 2;

  printf("%Ld", notLongEnough); // expected-warning {{format specifies type 'long long' but the argument has type 'long'}}
  printf("%Ld", quiteLong);

#ifndef ALLOWED
  // expected-warning@-4 {{length modifier 'L' results in undefined behavior or no effect with 'd' conversion specifier}}
  // expected-note@-5 {{did you mean to use 'll'?}}

  // expected-warning@-6 {{length modifier 'L' results in undefined behavior or no effect with 'd' conversion specifier}}
  // expected-note@-7 {{did you mean to use 'll'?}}
#endif

#ifndef MS
  printf("%Z\n", quiteLong); // expected-warning{{invalid conversion specifier 'Z'}}
#endif
}

void testAlwaysInvalid(void) {
  // We should not suggest 'll' here!
  printf("%Lc", 'a'); // expected-warning {{length modifier 'L' results in undefined behavior or no effect with 'c' conversion specifier}}
  printf("%Ls", "a"); // expected-warning {{length modifier 'L' results in undefined behavior or no effect with 's' conversion specifier}}
}

#ifdef ALLOWED
// PR 9466: clang: doesn't know about %Lu, %Ld, and %Lx
void printf_longlong(long long x, unsigned long long y) {
  printf("%Ld", y); // no-warning
  printf("%Lu", y); // no-warning
  printf("%Lx", y); // no-warning
  printf("%Ld", x); // no-warning
  printf("%Lu", x); // no-warning
  printf("%Lx", x); // no-warning
  printf("%Ls", "hello"); // expected-warning {{length modifier 'L' results in undefined behavior or no effect with 's' conversion specifier}}
}

void scanf_longlong(long long *x, unsigned long long *y) {
  scanf("%Ld", y); // no-warning
  scanf("%Lu", y); // no-warning
  scanf("%Lx", y); // no-warning
  scanf("%Ld", x); // no-warning
  scanf("%Lu", x); // no-warning
  scanf("%Lx", x); // no-warning
  scanf("%Ls", "hello"); // expected-warning {{length modifier 'L' results in undefined behavior or no effect with 's' conversion specifier}}
}
#endif

// SLATE-FILECHECK-DEFINES DEFAULT -DMS
// SLATE-FILECHECK-STD DEFAULT c17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %8 .str8: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 76, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 76, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 76, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 76, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %12 .str12: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%6 <unnamed>: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @scanf(%7 <unnamed>: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 notLongEnough: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %4 quiteLong: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(2));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%8)), read<i32>(%3));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%9)), read<i64>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @testAlwaysInvalid() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%10)), const<i32>(97));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%11)), array_decay<ptr<i8>, length=Some(2)>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
