/* Test C23 attribute syntax.  Test GNU attributes appertain to
   appropriate constructs.  Attributes on types not being defined at
   the time.  */
/* { dg-do compile } */
/* { dg-options "-std=gnu23 -Wformat" } */

typedef void va_type (const char *, ...);
typedef va_type [[gnu::format (printf, 1, 2)]] printf_like_1;
typedef void printf_like_2 (const char *, ...) [[gnu::format (printf, 1, 2)]];
typedef __typeof__ (void (const char *, ...) [[gnu::format (printf, 1, 2)]])
  printf_like_3;

va_type func1;
printf_like_1 func2;
printf_like_2 func3;
printf_like_3 func4;
va_type [[gnu::format (printf, 1, 2)]] *func5 (void);

void
func_test (void)
{
  func1 ("%s", 1);
  func2 ("%s", 1); /* { dg-warning "expects argument" } */
  func3 ("%s", 1); /* { dg-warning "expects argument" } */
  func4 ("%s", 1); /* { dg-warning "expects argument" } */
  func5 () ("%s", 1); /* { dg-warning "expects argument" } */
}

typedef int A[2];

__typeof__ (int [[gnu::deprecated]]) var1; /* { dg-warning "deprecated" } */
__typeof__ (A [[gnu::deprecated]]) var2; /* { dg-warning "deprecated" } */
__typeof__ (int [3] [[gnu::deprecated]]) var3; /* { dg-warning "deprecated" } */

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
// DEFAULT-NEXT:     type @type[[TYPE_va_type:[0-9]+]] va_type = fn(ptr<const i8>, ...) -> void;
// DEFAULT-NEXT:     type @type[[TYPE_printf_like_1:[0-9]+]] printf_like_1 = fn(ptr<const i8>, ...) -> void;
// DEFAULT-NEXT:     type @type[[TYPE_printf_like_2:[0-9]+]] printf_like_2 = fn(ptr<const i8>, ...) -> void;
// DEFAULT-NEXT:     type @type[[TYPE_printf_like_3:[0-9]+]] printf_like_3 = fn(ptr<const i8>, ...) -> void;
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = array<i32, 2>;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_var1:[0-9]+]] var1: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_var2:[0-9]+]] var2: array<i32, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_var3:[0-9]+]] var3: array<i32, 3> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_func1:[0-9]+]] @func1(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_func2:[0-9]+]] @func2(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_func3:[0-9]+]] @func3(%[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_func4:[0-9]+]] @func4(%[[VALUE3:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_func5:[0-9]+]] @func5() -> ptr<fn(ptr<const i8>, ...) -> void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_func_test:[0-9]+]] @func_test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ...) -> void>(%[[VALUE_func1]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]])), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ...) -> void>(%[[VALUE_func2]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_2]])), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ...) -> void>(%[[VALUE_func3]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_3]])), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ...) -> void>(%[[VALUE_func4]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_4]])), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ...) -> void>(call<ptr<fn(ptr<const i8>, ...) -> void>, signature=fn() -> ptr<fn(ptr<const i8>, ...) -> void>>(%[[VALUE_func5]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_5]])), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
