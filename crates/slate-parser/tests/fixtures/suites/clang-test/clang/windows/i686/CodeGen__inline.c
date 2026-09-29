//




__attribute__((noreturn)) void __cdecl _exit(int _Code);
__inline void __cdecl _Exit(int status) { _exit(status); }

extern __inline int ei() { return 123; }

__inline int foo() {
  return ei();
}

int bar() { return foo(); }


__inline void unreferenced1() {}
extern __inline void unreferenced2() {}

__inline __attribute((__gnu_inline__)) void gnu_inline() {}
void (*P1)() = gnu_inline;

// PR3988
extern __inline __attribute__((gnu_inline)) void gnu_ei_inline() {}
void (*P)() = gnu_ei_inline;

int test1();
__inline int test1() { return 4; }
__inline int test2() { return 5; }
__inline int test2();
int test2();

void test_test1() { test1(); }
void test_test2() { test2(); }

// PR3989
extern __inline void test3() __attribute__((gnu_inline));
__inline void __attribute__((gnu_inline)) test3() {}

extern int test4(void);
extern __inline __attribute__ ((__gnu_inline__)) int test4(void)
{
  return 0;
}

void test_test4() { test4(); }

extern __inline int test5(void)  __attribute__ ((__gnu_inline__));
extern __inline int __attribute__ ((__gnu_inline__)) test5(void)
{
  return 0;
}

void test_test5() { test5(); }

// PR10233

__inline int test6() { return 0; }
extern int test6();


// No PR#, but this once crashed clang in C99 mode due to buggy extern inline
// redeclaration detection.
void test7() { }
void test7();

// PR11062; the fact that the function is named strlcpy matters here.
inline __typeof(sizeof(int)) strlcpy(char *dest, const char *src, __typeof(sizeof(int)) size) { return 3; }
void test8() { strlcpy(0,0,0); }

// PR10657; the test crashed in C99 mode
extern inline void test9() { }
void test9();

inline void testA() {}
void testA();

void testB();
inline void testB() {}
extern void testB();

extern inline void testC() {}
inline void testC();

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c99
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT -Wno-strict-prototypes

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_P1:[0-9]+]] P1: ptr<fn(unprototyped) -> void> [storage=static] = function_decay<ptr<fn(unprototyped) -> void>>(%[[VALUE_gnu_inline:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_P:[0-9]+]] P: ptr<fn(unprototyped) -> void> [storage=static] = function_decay<ptr<fn(unprototyped) -> void>>(%[[VALUE_gnu_ei_inline:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__exit:[0-9]+]] @_exit(%[[VALUE__Code:[0-9]+]] _Code: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE__Exit:[0-9]+]] @_Exit(%[[VALUE_status:[0-9]+]] status: i32) -> void [linkage=external] [inline=hint] [definition=emitted] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE__exit]], read<i32>(%[[VALUE_status]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ei:[0-9]+]] @ei(unprototyped) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(123);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(unprototyped) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_ei]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_foo]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_unreferenced1:[0-9]+]] @unreferenced1(unprototyped) -> void [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_unreferenced2:[0-9]+]] @unreferenced2(unprototyped) -> void [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_inline]] @gnu_inline(unprototyped) -> void [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_ei_inline]] @gnu_ei_inline(unprototyped) -> void [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(unprototyped) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(unprototyped) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_test1:[0-9]+]] @test_test1(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_test1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_test2:[0-9]+]] @test_test2(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_test2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(unprototyped) -> void [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_test4:[0-9]+]] @test_test4(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_test4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5:[0-9]+]] @test5() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_test5:[0-9]+]] @test_test5(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_test5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6:[0-9]+]] @test6(unprototyped) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test7:[0-9]+]] @test7(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strlcpy:[0-9]+]] @strlcpy(%[[VALUE_dest:[0-9]+]] dest: ptr<i8>, %[[VALUE_src:[0-9]+]] src: ptr<const i8>, %[[VALUE_size:[0-9]+]] size: u32) -> u32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=always>(const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test8:[0-9]+]] @test8(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<u32, signature=fn(ptr<i8>, ptr<const i8>, u32) -> u32>(%[[VALUE_strlcpy]], null<ptr<i8>>, null<ptr<const i8>>, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test9:[0-9]+]] @test9(unprototyped) -> void [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testA:[0-9]+]] @testA(unprototyped) -> void [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testB:[0-9]+]] @testB(unprototyped) -> void [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testC:[0-9]+]] @testC(unprototyped) -> void [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
