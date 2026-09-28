/* PR c/12085 */
/* Origin: David Hollenberg <dhollen@mosis.org> */

/* Verify that the compiler doesn't inline a function at
   a calling point where it is viewed with a different
   prototype than the actual one.  */

/* { dg-do compile } */
/* { dg-options "-std=gnu17 -O3" } */

int foo1(int);
int foo2();

typedef struct {
  double d;
  int a;
} str_t;

void bar(double d, int i, str_t s)
{
  d = ((double (*) (int)) foo1) (i);  /* { dg-warning "8:non-compatible|abort" } */
  i = ((int (*) (double)) foo1) (d);  /* { dg-warning "8:non-compatible|abort" } */
  s = ((str_t (*) (int)) foo1) (i);   /* { dg-warning "8:non-compatible|abort" } */
  ((void (*) (int)) foo1) (d);        /* { dg-warning "non-compatible|abort" } */
  i = ((int (*) (int)) foo1) (i);     /* { dg-bogus "non-compatible|abort" } */
  (void) foo1 (i);                    /* { dg-bogus "non-compatible|abort" } */

  d = ((double (*) (int)) foo2) (i);  /* { dg-warning "8:non-compatible|abort" } */
  i = ((int (*) (double)) foo2) (d);  /* { dg-bogus "non-compatible|abort" } */
  s = ((str_t (*) (int)) foo2) (i);   /* { dg-warning "non-compatible|abort" } */
  ((void (*) (int)) foo2) (d);        /* { dg-warning "non-compatible|abort" } */
  i = ((int (*) (int)) foo2) (i);     /* { dg-bogus "non-compatible|abort" } */
  (void) foo2 (i);                    /* { dg-bogus "non-compatible|abort" } */
}

int foo1(int arg)
{
  /* Prevent the function from becoming const and thus DCEd.  */
  __asm volatile ("" : "+r" (arg));
  return arg;
}

int foo2(arg)
  int arg;
{
  /* Prevent the function from becoming const and thus DCEd.  */
  __asm volatile ("" : "+r" (arg));
  return arg;
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 a: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 str_t = @type0;
// DEFAULT-NEXT:     fn %0 @foo1(%8 arg: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 32 place<i32>(%8);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1 @foo2(%9 arg: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 32 place<i32>(%9);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar(%5 d: f64, %6 i: i32, %7 s: @type0) -> void [linkage=external] [abi=sysv64(scalar, scalar, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%5, call<f64, signature=fn(i32) -> f64>(pointer_cast<ptr<fn(i32) -> f64>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%0)), read<i32>(%6)));
// DEFAULT-NEXT:         call<f64, signature=fn(i32) -> f64>(pointer_cast<ptr<fn(i32) -> f64>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%0)), read<i32>(%6));
// DEFAULT-NEXT:         write<i32>(%6, call<i32, signature=fn(f64) -> i32>(pointer_cast<ptr<fn(f64) -> i32>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%0)), read<f64>(%5)));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(pointer_cast<ptr<fn(f64) -> i32>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%0)), read<f64>(%5));
// DEFAULT-NEXT:         write<@type0>(%7, copy<@type0, reason=assign>(call<@type0, signature=fn(i32) -> @type0, abi=sysv64(scalar) -> native_c>(pointer_cast<ptr<fn(i32) -> @type0>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%0)), read<i32>(%6))));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn(i32) -> @type0, abi=sysv64(scalar) -> native_c>(pointer_cast<ptr<fn(i32) -> @type0>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%0)), read<i32>(%6)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(pointer_cast<ptr<fn(i32) -> void>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%0)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f64>(%5)));
// DEFAULT-NEXT:         write<i32>(%6, call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%6)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%6));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%6));
// DEFAULT-NEXT:         write<f64>(%5, call<f64, signature=fn(i32) -> f64>(pointer_cast<ptr<fn(i32) -> f64>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%1)), read<i32>(%6)));
// DEFAULT-NEXT:         call<f64, signature=fn(i32) -> f64>(pointer_cast<ptr<fn(i32) -> f64>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%1)), read<i32>(%6));
// DEFAULT-NEXT:         write<i32>(%6, call<i32, signature=fn(f64) -> i32>(pointer_cast<ptr<fn(f64) -> i32>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%1)), read<f64>(%5)));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(pointer_cast<ptr<fn(f64) -> i32>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%1)), read<f64>(%5));
// DEFAULT-NEXT:         write<@type0>(%7, copy<@type0, reason=assign>(call<@type0, signature=fn(i32) -> @type0, abi=sysv64(scalar) -> native_c>(pointer_cast<ptr<fn(i32) -> @type0>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%1)), read<i32>(%6))));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn(i32) -> @type0, abi=sysv64(scalar) -> native_c>(pointer_cast<ptr<fn(i32) -> @type0>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%1)), read<i32>(%6)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(pointer_cast<ptr<fn(i32) -> void>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%1)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f64>(%5)));
// DEFAULT-NEXT:         write<i32>(%6, call<i32, signature=fn(i32) -> i32>(pointer_cast<ptr<fn(i32) -> i32>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%1)), read<i32>(%6)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(pointer_cast<ptr<fn(i32) -> i32>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%1)), read<i32>(%6));
// DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%1, read<i32>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
