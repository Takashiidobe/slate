/* Test C11 _Generic.  Valid uses.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

_Noreturn extern void exit (int);
_Noreturn extern void abort (void);

void
check (int n)
{
  if (n)
    abort ();
}

int
main (void)
{
  int n = 0;

  check (_Generic (n++, int: 0));
  /* _Generic should not evaluate its argument.  */
  check (n);

  check (_Generic (n, double: n++, default: 0));
  check (n);

  /* Qualifiers are removed for the purpose of type matching.  */
  const int cn = 0;
  check (_Generic (cn, int: 0, default: n++));
  check (n);
  check (_Generic ((const int) n, int: 0, default: n++));
  check (n);

  /* Arrays decay to pointers.  */
  int a[1];
  const int ca[1];
  check (_Generic (a, int *: 0, const int *: n++));
  check (n);
  check (_Generic (ca, const int *: 0, int *: n++));
  check (n);

  /* Functions decay to pointers.  */
  extern void f (void);
  check (_Generic (f, void (*) (void): 0, default: n++));
  check (n);

  /* _Noreturn is not part of the function type.  */
  check (_Generic (&abort, void (*) (void): 0, default: n++));
  check (n);

  /* Integer promotions do not occur.  */
  short s;
  check (_Generic (s, short: 0, int: n++));
  check (n);

  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_check:[0-9]+]] @check(%[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_n_2:[0-9]+]] n: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:         let %[[VALUE_cn:[0-9]+]] cn: i32 [storage=automatic] [const] = const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ca:[0-9]+]] ca: array<i32, 1> [storage=automatic] [const];
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: i16 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
