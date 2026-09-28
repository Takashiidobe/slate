/// Tests that we assign appropriate identifiers to indirect calls for no-prototype
/// functions based on call site argument types.





void foo() {
}

void foo_with_proto(void) {
}

void bar() {
  void (*fp)() = foo;
  fp();
}

struct my_struct;

struct my_struct *create_my_struct() {
  return 0;
}

struct my_struct *create_my_struct_with_proto(void) {
  return 0;
}

void test_struct_ptr_return() {
  struct my_struct *(*fp)() = create_my_struct;
  fp();
}

int baz() {
  return 1;
}

int baz_with_proto(void) {
  return 1;
}

void test_int_return() {
  int (*fp)() = baz;
  fp();
}

void foo_with_int_proto(int a) {
}

void test_no_proto_with_args() {
  void (*fp)() = foo;
  fp(1);
}

void foo_with_promoted_proto(int a, double b) {
}

/// Tests that multiple arguments passed to an unprototyped function pointer undergo
/// C default argument promotion (short -> int, float -> double) when reconstructing the prototype.
void test_promoted_args() {
  void (*fp)() = foo;
  fp((short)1, (float)2.0);
}

// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
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
// DEFAULT-NEXT:     type @type0 my_struct = struct incomplete;
// DEFAULT-NEXT:     fn %0 @foo(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1 @foo_with_proto() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @bar(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 fp: ptr<fn(unprototyped) -> void> [storage=automatic] = function_decay<ptr<fn(unprototyped) -> void>>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(read<ptr<fn(unprototyped) -> void>>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @create_my_struct(unprototyped) -> ptr<@type0> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<@type0>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @create_my_struct_with_proto() -> ptr<@type0> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<@type0>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test_struct_ptr_return(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 fp: ptr<fn(unprototyped) -> ptr<@type0>> [storage=automatic] = function_decay<ptr<fn(unprototyped) -> ptr<@type0>>>(%5);
// DEFAULT-NEXT:         call<ptr<@type0>, signature=fn(unprototyped) -> ptr<@type0>>(read<ptr<fn(unprototyped) -> ptr<@type0>>>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @baz(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @baz_with_proto() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test_int_return(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 fp: ptr<fn(unprototyped) -> i32> [storage=automatic] = function_decay<ptr<fn(unprototyped) -> i32>>(%9);
// DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(read<ptr<fn(unprototyped) -> i32>>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @foo_with_int_proto(%14 a: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test_no_proto_with_args(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %16 fp: ptr<fn(unprototyped) -> void> [storage=automatic] = function_decay<ptr<fn(unprototyped) -> void>>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(read<ptr<fn(unprototyped) -> void>>(%16), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @foo_with_promoted_proto(%18 a: i32, %19 b: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @test_promoted_args(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %21 fp: ptr<fn(unprototyped) -> void> [storage=automatic] = function_decay<ptr<fn(unprototyped) -> void>>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(read<ptr<fn(unprototyped) -> void>>(%21), widen<i32, reason=vararg>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), float_widen<f64, reason=vararg>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
