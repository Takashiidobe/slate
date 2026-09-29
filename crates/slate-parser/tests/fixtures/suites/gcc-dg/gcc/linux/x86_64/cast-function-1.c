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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 a: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_str_t:[0-9]+]] str_t = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_foo1:[0-9]+]] @foo1(%[[VALUE_arg:[0-9]+]] arg: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_arg]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_arg]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo2:[0-9]+]] @foo2(%[[VALUE_arg_2:[0-9]+]] arg: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_arg_2]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_arg_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_d:[0-9]+]] d: f64, %[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_s:[0-9]+]] s: @type[[TYPE0]]) -> void [linkage=external] [abi=sysv64(scalar, scalar, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%[[VALUE_d]], call<f64, signature=fn(i32) -> f64>(pointer_cast<ptr<fn(i32) -> f64>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_foo1]])), read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<f64, signature=fn(i32) -> f64>(pointer_cast<ptr<fn(i32) -> f64>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_foo1]])), read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], call<i32, signature=fn(f64) -> i32>(pointer_cast<ptr<fn(f64) -> i32>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_foo1]])), read<f64>(%[[VALUE_d]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(pointer_cast<ptr<fn(f64) -> i32>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_foo1]])), read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(%[[VALUE_s]], copy<@type[[TYPE0]], reason=assign>(call<@type[[TYPE0]], signature=fn(i32) -> @type[[TYPE0]], abi=sysv64(scalar) -> native_c>(pointer_cast<ptr<fn(i32) -> @type[[TYPE0]]>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_foo1]])), read<i32>(%[[VALUE_i]]))));
// DEFAULT-NEXT:         copy<@type[[TYPE0]], reason=assign>(call<@type[[TYPE0]], signature=fn(i32) -> @type[[TYPE0]], abi=sysv64(scalar) -> native_c>(pointer_cast<ptr<fn(i32) -> @type[[TYPE0]]>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_foo1]])), read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(pointer_cast<ptr<fn(i32) -> void>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_foo1]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<f64>(%[[VALUE_d]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo1]], read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo1]], read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo1]], read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_d]], call<f64, signature=fn(i32) -> f64>(pointer_cast<ptr<fn(i32) -> f64>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%[[VALUE_foo2]])), read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<f64, signature=fn(i32) -> f64>(pointer_cast<ptr<fn(i32) -> f64>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%[[VALUE_foo2]])), read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], call<i32, signature=fn(f64) -> i32>(pointer_cast<ptr<fn(f64) -> i32>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%[[VALUE_foo2]])), read<f64>(%[[VALUE_d]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(pointer_cast<ptr<fn(f64) -> i32>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%[[VALUE_foo2]])), read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(%[[VALUE_s]], copy<@type[[TYPE0]], reason=assign>(call<@type[[TYPE0]], signature=fn(i32) -> @type[[TYPE0]], abi=sysv64(scalar) -> native_c>(pointer_cast<ptr<fn(i32) -> @type[[TYPE0]]>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%[[VALUE_foo2]])), read<i32>(%[[VALUE_i]]))));
// DEFAULT-NEXT:         copy<@type[[TYPE0]], reason=assign>(call<@type[[TYPE0]], signature=fn(i32) -> @type[[TYPE0]], abi=sysv64(scalar) -> native_c>(pointer_cast<ptr<fn(i32) -> @type[[TYPE0]]>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%[[VALUE_foo2]])), read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(pointer_cast<ptr<fn(i32) -> void>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%[[VALUE_foo2]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<f64>(%[[VALUE_d]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], call<i32, signature=fn(i32) -> i32>(pointer_cast<ptr<fn(i32) -> i32>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%[[VALUE_foo2]])), read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(pointer_cast<ptr<fn(i32) -> i32>, reason=explicit>(function_decay<ptr<fn(unprototyped) -> i32>>(%[[VALUE_foo2]])), read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_foo2]], read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
