
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
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_link_error0:[0-9]+]] @link_error0() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_link_error1:[0-9]+]] @link_error1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_link_error2:[0-9]+]] @link_error2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_link_error3:[0-9]+]] @link_error3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_link_error4:[0-9]+]] @link_error4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_link_error5:[0-9]+]] @link_error5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_link_error6:[0-9]+]] @link_error6() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_link_error7:[0-9]+]] @link_error7() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func0:[0-9]+]] @func0(%[[VALUE_a:[0-9]+]] a: i32) -> i32 [linkage=external] [memory=read] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func1:[0-9]+]] @func1(%[[VALUE_a_2:[0-9]+]] a: i32) -> i32 [linkage=external] [memory=none] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func2:[0-9]+]] @func2(%[[VALUE_a_3:[0-9]+]] a: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_a_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func3:[0-9]+]] @func3(%[[VALUE_a_4:[0-9]+]] a: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(read<i32>(%[[VALUE_a_4]]), const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func4:[0-9]+]] @func4(%[[VALUE_a_5:[0-9]+]] a: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_func0]], read<i32>(%[[VALUE_a_5]])), read<i32>(%[[VALUE_a_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func5:[0-9]+]] @func5(%[[VALUE_a_6:[0-9]+]] a: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_a_6]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE_func1]], read<i32>(%[[VALUE_a_6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func6:[0-9]+]] @func6(%[[VALUE_a_7:[0-9]+]] a: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_func2]], read<i32>(%[[VALUE_a_7]])), read<i32>(%[[VALUE_a_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func7:[0-9]+]] @func7(%[[VALUE_a_8:[0-9]+]] a: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_a_8]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE_func3]], read<i32>(%[[VALUE_a_8]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: array<i32, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_i_2]]), const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_func0]], const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_i_2]]), const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error0]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_i_2]]), const<i32>(1))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_func1]], const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_i_2]]), const<i32>(1)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error1]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_i_2]]), const<i32>(2))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_func2]], const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_func2]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_i_2]]), const<i32>(2)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error2]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_i_2]]), const<i32>(3))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_func3]], const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_func3]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_i_2]]), const<i32>(3)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error3]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_i_2]]), const<i32>(4))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_func4]], const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_func4]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_i_2]]), const<i32>(4)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error4]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_i_2]]), const<i32>(5))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_func5]], const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_func5]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_i_2]]), const<i32>(5)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error5]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_i_2]]), const<i32>(6))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_func6]], const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_func6]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_i_2]]), const<i32>(6)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error6]]);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_i_2]]), const<i32>(7))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_func7]], const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_func7]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_i_2]]), const<i32>(7)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error7]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
