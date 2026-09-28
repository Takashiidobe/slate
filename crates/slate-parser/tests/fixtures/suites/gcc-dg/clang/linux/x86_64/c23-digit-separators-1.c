/* Test C23 digit separators.  Valid usages.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors" } */

_Static_assert(123'45'6 == 123456);
_Static_assert(0'123 == 0123);
_Static_assert(0x1'23 == 0x123);
_Static_assert(0b1'01 == 0b101);

#define m(x) 0

_Static_assert(m(1'2) + (3'4) == 34);

_Static_assert(0x0'e - 0xe == 0);

#define a0      '.' -
#define acat(x) a##x
_Static_assert(acat(0 '.') == 0);

#define c0(x) 0
#define b0 c0 (
#define bcat(x) b##x
_Static_assert (bcat (0'\u00c0')) == 0);

extern void exit(int);
extern void abort(void);

int main(void) {
  if (314'159e-0'5f != 3.14159f)
    abort();
  exit(0);
}

#line 0'123
_Static_assert(__LINE__ == 123);

#line 4'56'7'8'9
_Static_assert(__LINE__ == 456789);



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
// DEFAULT-NEXT:     fn %0 @exit(%3 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(const<f32>(3.14159), const<f32>(3.14159))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
