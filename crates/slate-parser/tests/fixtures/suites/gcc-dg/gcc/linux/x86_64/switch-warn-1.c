/* { dg-do run } */
/* { dg-options "-O0" } */

extern void abort (void);
extern void exit (int);

/* Check that out-of-bounds case warnings work in the case that the
   testing expression is promoted.  */
int
foo1 (unsigned char i)
{
  switch (i)
    {
    case -1:   /* { dg-warning "case label value is less than minimum value for type|statement will never be executed" } */
      return 1;
    case 256:  /* { dg-warning "case label value exceeds maximum value for type" } */
      return 2;
    default:
      return 3;
    }
}

/* Like above, but for case ranges that need to be satured.  */
int
foo2 (unsigned char i)
{
  switch (i)
    {
    case -1 ... 1:   /* { dg-warning "lower value in case label range less than minimum value for type" } */
      return 1;
    case 254 ... 256:  /* { dg-warning "upper value in case label range exceeds maximum value for type" } */
      return 2;
    default:
      return 3;
    }
}

int
main (void)
{
  if (foo1 (10) != 3)
    abort ();
  if (foo2 (10) != 3)
    abort ();
  exit (0);
}


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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo1:[0-9]+]] @foo1(%[[VALUE_i:[0-9]+]] i: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %[[VALUE1:[0-9]+]] reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_i]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(-1):
// DEFAULT-NEXT:                     return const<i32>(1);
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(256):
// DEFAULT-NEXT:                     return const<i32>(2);
// DEFAULT-NEXT:                 default %[[VALUE1]]:
// DEFAULT-NEXT:                     return const<i32>(3);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo2:[0-9]+]] @foo2(%[[VALUE_i_2:[0-9]+]] i: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %[[VALUE2:[0-9]+]] reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_i_2]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE2]] const<i32>(-1) ... const<i32>(1):
// DEFAULT-NEXT:                     return const<i32>(1);
// DEFAULT-NEXT:                 case %[[VALUE2]] const<i32>(254) ... const<i32>(256):
// DEFAULT-NEXT:                     return const<i32>(2);
// DEFAULT-NEXT:                 default %[[VALUE2]]:
// DEFAULT-NEXT:                     return const<i32>(3);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u8) -> i32>(%[[VALUE_foo1]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(10)))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u8) -> i32>(%[[VALUE_foo2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(10)))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
