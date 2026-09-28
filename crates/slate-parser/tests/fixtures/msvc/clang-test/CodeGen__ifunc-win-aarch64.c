

int foo(int) __attribute__ ((ifunc("foo_ifunc")));

static int f1(int i) {
  return i + 1;
}

static int f2(int i) {
  return i + 2;
}

typedef int (*foo_t)(int);

volatile int global;

static foo_t foo_ifunc(void) {
  return global ? f1 : f2;
}

int bar(void) {
  return foo(1);
}

extern void goo(void);

void bar2(void) {
  goo();
}

extern void goo(void) __attribute__ ((ifunc("goo_ifunc")));

void* goo_ifunc(void) {
  return 0;
}

/// The ifunc is emitted after its resolver.
void *hoo_ifunc(void) { return 0; }
extern void hoo(int) __attribute__ ((ifunc("hoo_ifunc")));

/// ifunc on Windows is lowered to global pointers and an indirect call.
/// Register the constructor for initialisation.

// CHECK   %0 = load ptr, ptr @0, align 8
// CHECK   %call = call i32 %0(i32 noundef 1)

// CHECK %0 = load ptr, ptr getelementptr inbounds ([3 x ptr], ptr @0, i32 0, i32 1), align 8
// CHECK call void %0()

// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-ARGS -target=aarch64-pc-windows-msvc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "aarch64-pc-windows-msvc" {
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
// DEFAULT-NEXT:     type @type0 foo_t = ptr<fn(i32) -> i32>;
// DEFAULT-NEXT:     global %6 global: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @foo(%14 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @f1(%2 i: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%2), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @f2(%4 i: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%4), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @foo_ifunc() -> ptr<fn(i32) -> i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<fn(i32) -> i32>>(ne<i32>(read<i32, volatile>(%6), const<i32>(0)), function_decay<ptr<fn(i32) -> i32>>(%1), function_decay<ptr<fn(i32) -> i32>>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @bar() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(%0, const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @goo() -> void [linkage=external];
// DEFAULT-NEXT:     fn %10 @bar2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @goo_ifunc() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @hoo_ifunc() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @hoo(%15 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
