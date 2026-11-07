/* { dg-do compile } */
/* { dg-options "-std=gnu17 -Wall -Wshadow -fshow-column" } */

extern double strtod (const char *, char **);
#define UNUSED __attribute__ ((unused))

/* A built-in function may be overridden by an old-style definition
   specifying too few arguments... */
double cos ()  /* { dg-warning "shadows a built-in|number of arguments" } */ 
{
  return strtod ("nan", 0);
}

/* the right number, but the wrong type, arguments... */
double sin (foo)  /* { dg-warning "8:shadows a built-in" } */
     int foo UNUSED; /* { dg-warning "10:argument 'foo' doesn't match built-in prototype" } */
{
  return strtod ("nan", 0);
}

/* or too many arguments.  */
long double cosl (foo, bar)  /* { dg-warning "shadows a built-in|number of arguments" } */
     const char *foo UNUSED; /* { dg-warning "18:argument 'foo' doesn't match" } */
     int bar UNUSED;
{
  return strtod ("nan", 0);
}

// SLATE-FILECHECK-STD DEFAULT gnu17
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([110, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([110, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([110, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strtod:[0-9]+]] @strtod(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<ptr<i8>>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cos:[0-9]+]] @cos(unprototyped) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(ptr<const i8>, ptr<ptr<i8>>) -> f64>(%[[VALUE_strtod]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), null<ptr<ptr<i8>>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sin:[0-9]+]] @sin(%[[VALUE_foo:[0-9]+]] foo: i32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(ptr<const i8>, ptr<ptr<i8>>) -> f64>(%[[VALUE_strtod]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), null<ptr<ptr<i8>>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_cosl:[0-9]+]] @cosl(%[[VALUE_foo_2:[0-9]+]] foo: ptr<const i8>, %[[VALUE_bar:[0-9]+]] bar: i32) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_widen<f80, reason=return>(call<f64, signature=fn(ptr<const i8>, ptr<ptr<i8>>) -> f64>(%[[VALUE_strtod]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]])), null<ptr<ptr<i8>>>));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
