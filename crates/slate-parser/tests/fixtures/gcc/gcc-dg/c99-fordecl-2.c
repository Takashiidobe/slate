/* Test for C99 declarations in for loops.  Test constraints.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

void
foo (void)
{
  /* See comments in check_for_loop_decls (c-decl.c) for the presumptions
     behind these tests.  */
  int j = 0;
  for (int i = 1, bar (void); i <= 10; i++) /* { dg-bogus "warning" "warning in place of error" } */
    /* { dg-error "bar" "function in for loop" { target *-*-* } .-1 } */
    j += i;

  for (static int i = 1; i <= 10; i++) /* { dg-bogus "warning" "warning in place of error" } */
    /* { dg-error "static" "static in for loop" { target *-*-* } .-1 } */
    j += i;

  for (extern int i; j <= 500; j++) /* { dg-bogus "warning" "warning in place of error" } */
    /* { dg-error "extern" "extern in for loop" { target *-*-* } .-1 } */
    j += 5;

  for (enum { FOO } i = FOO; i < 10; i++) /* { dg-bogus "warning" "warning in place of error" } */
    /* { dg-error "FOO" "enum value in for loop" { target *-*-* } .-1 } */
    j += i;

  for (enum BAR { FOO } i = FOO; i < 10; i++) /* { dg-bogus "warning" "warning in place of error" } */
    /* { dg-error "FOO" "enum value in for loop" { target *-*-* } .-1 } */
    /* { dg-error "BAR" "enum tag in for loop" { target *-*-* } .-2 } */
    j += i;
  for (typedef int T;;) /* { dg-error "non-variable" } */
    ;
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     type @type0 = enum : u32 {
// DEFAULT-NEXT:         %0 FOO = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 BAR = enum : u32 {
// DEFAULT-NEXT:         %0 FOO = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type2 T = i32;
// DEFAULT-NEXT:     global %4 i: i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     extern %5 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @bar() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %0 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %2 i: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%2), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%20));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), read<i32>(%2));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%22));
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%4), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %23: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%24));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), read<i32>(%4));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%26));
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%1), const<i32>(500))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%28));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(5));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%30));
// DEFAULT-NEXT:         for %16
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %8 i: @type0 [storage=automatic] = int_to_enum<@type0, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(enum_to_int<u32, reason=promotion>(read<@type0>(%8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %31: @type0 [synthetic] = read<@type0>(%8);
// DEFAULT-NEXT:                 let %32: @type0 [synthetic] = int_to_enum<@type0, reason=assign>(add<u32, overflow=wrap>(enum_to_int<u32, reason=promotion>(read<@type0>(%31)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                 write<@type0>(%8, read<@type0>(%32));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%33)), enum_to_int<u32, reason=promotion>(read<@type0>(%8))));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%34));
// DEFAULT-NEXT:         for %17
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %11 i: @type1 [storage=automatic] = int_to_enum<@type1, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(enum_to_int<u32, reason=promotion>(read<@type1>(%11)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %35: @type1 [synthetic] = read<@type1>(%11);
// DEFAULT-NEXT:                 let %36: @type1 [synthetic] = int_to_enum<@type1, reason=assign>(add<u32, overflow=wrap>(enum_to_int<u32, reason=promotion>(read<@type1>(%35)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                 write<@type1>(%11, read<@type1>(%36));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %37: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %38: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%37)), enum_to_int<u32, reason=promotion>(read<@type1>(%11))));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%38));
// DEFAULT-NEXT:         for %18
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
