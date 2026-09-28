


int printf(const char *restrict, ...);
int scanf(const char * restrict, ...) ;

void test(void) {
  int justRight = 1;
  long tooLong = 2;

  printf("%D", justRight);
  printf("%D", tooLong);
  printf("%U", justRight);
  printf("%U", tooLong);
  printf("%O", justRight);
  printf("%O", tooLong);

#ifdef ALLOWED
  // expected-warning@-8 {{'D' conversion specifier is not supported by ISO C}} expected-note@-8 {{did you mean to use 'd'?}}
  // expected-warning@-8 {{'D' conversion specifier is not supported by ISO C}} expected-note@-8 {{did you mean to use 'd'?}} expected-warning@-8 {{format specifies type 'int' but the argument has type 'long'}}
  // expected-warning@-8 {{'U' conversion specifier is not supported by ISO C}} expected-note@-8 {{did you mean to use 'u'?}}
  // expected-warning@-8 {{'U' conversion specifier is not supported by ISO C}} expected-note@-8 {{did you mean to use 'u'?}} expected-warning@-8 {{format specifies type 'unsigned int' but the argument has type 'long'}}
  // expected-warning@-8 {{'O' conversion specifier is not supported by ISO C}} expected-note@-8 {{did you mean to use 'o'?}}
  // expected-warning@-8 {{'O' conversion specifier is not supported by ISO C}} expected-note@-8 {{did you mean to use 'o'?}} expected-warning@-8 {{format specifies type 'unsigned int' but the argument has type 'long'}}
#else
  // expected-warning@-15 {{invalid conversion specifier 'D'}}
  // expected-warning@-15 {{invalid conversion specifier 'D'}}
  // expected-warning@-15 {{invalid conversion specifier 'U'}}
  // expected-warning@-15 {{invalid conversion specifier 'U'}}
  // expected-warning@-15 {{invalid conversion specifier 'O'}}
  // expected-warning@-15 {{invalid conversion specifier 'O'}}
#endif
}

#ifdef ALLOWED
void testPrintf(short x, long y) {
  printf("%hD", x); // expected-warning{{conversion specifier is not supported by ISO C}} expected-note {{did you mean to use 'd'?}}
  printf("%lD", y); // expected-warning{{conversion specifier is not supported by ISO C}} expected-note {{did you mean to use 'd'?}}
  printf("%hU", x); // expected-warning{{conversion specifier is not supported by ISO C}} expected-note {{did you mean to use 'u'?}}
  printf("%lU", y); // expected-warning{{conversion specifier is not supported by ISO C}} expected-note {{did you mean to use 'u'?}}
  printf("%hO", x); // expected-warning{{conversion specifier is not supported by ISO C}} expected-note {{did you mean to use 'o'?}}
  printf("%lO", y); // expected-warning{{conversion specifier is not supported by ISO C}} expected-note {{did you mean to use 'o'?}}

  printf("%+'0.5lD", y); // expected-warning{{conversion specifier is not supported by ISO C}} expected-note {{did you mean to use 'd'?}}
  printf("% '0.5lD", y); // expected-warning{{conversion specifier is not supported by ISO C}} expected-note {{did you mean to use 'd'?}}
  printf("%#0.5lO", y); // expected-warning{{conversion specifier is not supported by ISO C}} expected-note {{did you mean to use 'o'?}}
  printf("%'0.5lU", y); // expected-warning{{conversion specifier is not supported by ISO C}} expected-note {{did you mean to use 'u'?}}
}

void testScanf(short *x, long *y) {
  scanf("%hD", x); // expected-warning{{conversion specifier is not supported by ISO C}} expected-note {{did you mean to use 'd'?}}
  scanf("%lD", y); // expected-warning{{conversion specifier is not supported by ISO C}} expected-note {{did you mean to use 'd'?}}
  scanf("%hU", x); // expected-warning{{conversion specifier is not supported by ISO C}} expected-note {{did you mean to use 'u'?}}
  scanf("%lU", y); // expected-warning{{conversion specifier is not supported by ISO C}} expected-note {{did you mean to use 'u'?}}
  scanf("%hO", x); // expected-warning{{conversion specifier is not supported by ISO C}} expected-note {{did you mean to use 'o'?}}
  scanf("%lO", y); // expected-warning{{conversion specifier is not supported by ISO C}} expected-note {{did you mean to use 'o'?}}
}
#endif

// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-ARGS -target=i686-pc-windows-msvc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT -Wformat-non-iso

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
// DEFAULT-NEXT:     global %7 .str7: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 68, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 68, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 85, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 85, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %12 .str12: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%5 <unnamed>: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @scanf(%6 <unnamed>: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 justRight: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %4 tooLong: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%7)), read<i32>(%3));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%8)), read<i32>(%4));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%9)), read<i32>(%3));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%10)), read<i32>(%4));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%11)), read<i32>(%3));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%12)), read<i32>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
