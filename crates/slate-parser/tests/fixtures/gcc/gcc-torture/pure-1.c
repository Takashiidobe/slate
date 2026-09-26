
/* Origin: Kaveh Ghazi <ghazi@caip.rutgers.edu> 2002-05-27.  */

/* Use a different function for each test so the link failures
   indicate which one is broken.  */
extern void link_error0(void);
extern void link_error1(void);
extern void link_error2(void);
extern void link_error3(void);
extern void link_error4(void);
extern void link_error5(void);
extern void link_error6(void);
extern void link_error7(void);

extern int i;

extern int func0(int) __attribute__((__pure__));
extern int func1(int) __attribute__((__const__));

/* GCC should automatically detect attributes for these functions.
   At -O3 They'll be inlined, but that's ok.  */
static int func2(int a) { return i + a; }        /* pure */
static int func3(int a) { return a * 3; }        /* const */
static int func4(int a) { return func0(a) + a; } /* pure */
static int func5(int a) { return a + func1(a); } /* const */
static int func6(int a) { return func2(a) + a; } /* pure */
static int func7(int a) { return a + func3(a); } /* const */

int main() {
  int i[10], r;

  i[0] = 0;
  r    = func0(0);
  if (i[0])
    link_error0();

  i[1] = 0;
  r    = func1(0);
  if (i[1])
    link_error1();

  i[2] = 0;
  r    = func2(0);
  if (i[2])
    link_error2();

  i[3] = 0;
  r    = func3(0);
  if (i[3])
    link_error3();

  i[4] = 0;
  r    = func4(0);
  if (i[4])
    link_error4();

  i[5] = 0;
  r    = func5(0);
  if (i[5])
    link_error5();

  i[6] = 0;
  r    = func6(0);
  if (i[6])
    link_error6();

  i[7] = 0;
  r    = func7(0);
  if (i[7])
    link_error7();

  return r;
}

int func0(int a) { return a - i; } /* pure */
int func1(int a) { return a - a; } /* const */

int i = 2;

#ifndef __OPTIMIZE__
/* Avoid link failures when not optimizing. */
void link_error0() {}
void link_error1() {}
void link_error2() {}
void link_error3() {}
void link_error4() {}
void link_error5() {}
void link_error6() {}
void link_error7() {}
#endif /* ! __OPTIMIZE__ */


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
// DEFAULT-NEXT:     global %8 i: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     fn %0 @link_error0() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1 @link_error1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @link_error2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @link_error3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @link_error4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @link_error5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @link_error6() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @link_error7() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @func0(%26 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(%26), read<i32>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @func1(%27 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(%27), read<i32>(%27));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @func2(%12 a: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%8), read<i32>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @func3(%14 a: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(read<i32>(%14), const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @func4(%16 a: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%9, read<i32>(%16)), read<i32>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @func5(%18 a: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%18), call<i32, signature=fn(i32) -> i32>(%10, read<i32>(%18)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @func6(%20 a: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%11, read<i32>(%20)), read<i32>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @func7(%22 a: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%22), call<i32, signature=fn(i32) -> i32>(%13, read<i32>(%22)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %24 i: array<i32, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %25 r: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%24), const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%25, call<i32, signature=fn(i32) -> i32>(%9, const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%9, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%24), const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%24), const<i32>(1))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%25, call<i32, signature=fn(i32) -> i32>(%10, const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%10, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%24), const<i32>(1)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%24), const<i32>(2))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%25, call<i32, signature=fn(i32) -> i32>(%11, const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%11, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%24), const<i32>(2)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%24), const<i32>(3))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%25, call<i32, signature=fn(i32) -> i32>(%13, const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%13, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%24), const<i32>(3)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%24), const<i32>(4))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%25, call<i32, signature=fn(i32) -> i32>(%15, const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%15, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%24), const<i32>(4)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%24), const<i32>(5))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%25, call<i32, signature=fn(i32) -> i32>(%17, const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%17, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%24), const<i32>(5)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%24), const<i32>(6))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%25, call<i32, signature=fn(i32) -> i32>(%19, const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%19, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%24), const<i32>(6)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%24), const<i32>(7))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%25, call<i32, signature=fn(i32) -> i32>(%21, const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%21, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%24), const<i32>(7)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         return read<i32>(%25);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
