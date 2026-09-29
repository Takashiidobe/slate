/* String initializers for arrays must not be parenthesized.  Bug
   11250 from h.b.furuseth at usit.uio.no.  */
/* Origin: Joseph Myers <jsm@polyomino.org.uk> */
/* { dg-do compile } */
/* { dg-options "-std=c99 -pedantic-errors" } */

#include <stddef.h>

char *a = "a";
char *b = ("b");
char *c = (("c"));

char d[] = "d";
char e[] = ("e"); /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "parenthesized|near init" "paren array" { target *-*-* } .-1 } */
char f[] = (("f")); /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "parenthesized|near init" "paren array" { target *-*-* } .-1 } */

signed char g[] = { "d" };
unsigned char h[] = { ("e") }; /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "parenthesized|near init" "paren array" { target *-*-* } .-1 } */
signed char i[] = { (("f")) }; /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "parenthesized|near init" "paren array" { target *-*-* } .-1 } */


struct s { char a[10]; int b; wchar_t c[10]; };

struct s j = {
  "j",
  1,
  (L"j")
  /* { dg-error "parenthesized|near init" "paren array" { target *-*-* } .-1 } */
}; /* { dg-bogus "warning" "warning in place of error" } */

struct s k = {
  (("k")), /* { dg-bogus "warning" "warning in place of error" } */
  /* { dg-error "parenthesized|near init" "paren array" { target *-*-* } .-1 } */
  1,
  L"k"
};

struct s l = {
  .c = (L"l"), /* { dg-bogus "warning" "warning in place of error" } */
  /* { dg-error "parenthesized|near init" "paren array" { target *-*-* } .-1 } */
  .a = "l"
};

struct s m = {
  .c = L"m",
  .a = ("m")
  /* { dg-error "parenthesized|near init" "paren array" { target *-*-* } .-1 } */
}; /* { dg-bogus "warning" "warning in place of error" } */

char *n = (char []){ "n" };

char *o = (char []){ ("o") }; /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "parenthesized|near init" "paren array" { target *-*-* } .-1 } */

wchar_t *p = (wchar_t [5]){ (L"p") }; /* { dg-bogus "warning" "warning in place of error" } */
/* { dg-error "parenthesized|near init" "paren array" { target *-*-* } .-1 } */

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     type @type0 wchar_t = i32;
// DEFAULT-NEXT:     type @type1 s = struct {
// DEFAULT-NEXT:         field0 a: array<i8, 10>;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: array<i32, 10>;
// DEFAULT-NEXT:     } [size=56, align=4, offsets=[0, 12, 16]];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %1 a: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(2)>(%18) [linkage=external];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %2 b: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(2)>(%19) [linkage=external];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %3 c: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(2)>(%20) [linkage=external];
// DEFAULT-NEXT:     global %4 d: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([100, 0]) [linkage=external];
// DEFAULT-NEXT:     global %5 e: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([101, 0]) [linkage=external];
// DEFAULT-NEXT:     global %6 f: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([102, 0]) [linkage=external];
// DEFAULT-NEXT:     global %7 g: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([100, 0]) [linkage=external];
// DEFAULT-NEXT:     global %8 h: array<u8, 2> [storage=static] = code_units<array<u8, 2>>([101, 0]) [linkage=external];
// DEFAULT-NEXT:     global %9 i: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([102, 0]) [linkage=external];
// DEFAULT-NEXT:     global %11 j: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = code_units<array<i8, 10>>([106, 0, 0, 0, 0, 0, 0, 0, 0, 0]), field1 = const<i32>(1), field2 = code_units<array<i32, 10>>([106, 0, 0, 0, 0, 0, 0, 0, 0, 0])) [linkage=external];
// DEFAULT-NEXT:     global %12 k: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = code_units<array<i8, 10>>([107, 0, 0, 0, 0, 0, 0, 0, 0, 0]), field1 = const<i32>(1), field2 = code_units<array<i32, 10>>([107, 0, 0, 0, 0, 0, 0, 0, 0, 0])) [linkage=external];
// DEFAULT-NEXT:     global %13 l: @type1 [storage=static] = aggregate<@type1, zero_fill=true>(field0 = code_units<array<i8, 10>>([108, 0, 0, 0, 0, 0, 0, 0, 0, 0]), field2 = code_units<array<i32, 10>>([108, 0, 0, 0, 0, 0, 0, 0, 0, 0])) [linkage=external];
// DEFAULT-NEXT:     global %14 m: @type1 [storage=static] = aggregate<@type1, zero_fill=true>(field0 = code_units<array<i8, 10>>([109, 0, 0, 0, 0, 0, 0, 0, 0, 0]), field2 = code_units<array<i32, 10>>([109, 0, 0, 0, 0, 0, 0, 0, 0, 0])) [linkage=external];
// DEFAULT-NEXT:     global %15 n: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(2)>(compound_literal %21 [storage=static] = code_units<array<i8, 2>>([110, 0])) [linkage=external];
// DEFAULT-NEXT:     global %16 o: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(2)>(compound_literal %22 [storage=static] = code_units<array<i8, 2>>([111, 0])) [linkage=external];
// DEFAULT-NEXT:     global %17 p: ptr<i32> [storage=static] = array_decay<ptr<i32>, length=Some(5)>(compound_literal %23 [storage=static] = code_units<array<i32, 5>>([112, 0, 0, 0, 0])) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
