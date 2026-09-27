/* Tests for #pragma init and #pragma fini.  */

/* { dg-do run { target *-*-solaris2.* } } */

extern void abort ();

#pragma init		/* { dg-warning "malformed" } */
#pragma init ()		/* { dg-warning "malformed" } */
#pragma init init_func	/* { dg-warning "malformed" } */

#pragma fini		/* { dg-warning "malformed" } */
#pragma fini ()		/* { dg-warning "malformed" } */
#pragma fini fini_func	/* { dg-warning "malformed" } */

#pragma init (init_func, init_static_func)

int glob_1, glob_2;

void init_func (void)
{
  glob_1 = 1;
}

static void init_static_func (void)
{
  glob_2 = 2;
}

#pragma fini (fini_func, fini_static_func)

void fini_func (void)
{

}

static void fini_static_func (void)
{

}

int main()
{
  if (glob_1 != 1)
    abort ();

  if (glob_2 != 2)
    abort ();

  return 0;
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     global %1 glob_1: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 glob_2: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort(unprototyped) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @init_func() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @init_static_func() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @fini_func() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @fini_static_func() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
