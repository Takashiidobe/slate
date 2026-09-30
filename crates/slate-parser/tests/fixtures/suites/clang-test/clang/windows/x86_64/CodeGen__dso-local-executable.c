

/// Static relocation model defaults to -fdirect-access-external-data and sets
/// dso_local on most global objects.


/// If -fno-direct-access-external-data is set, drop dso_local from global variable
/// declarations.









/// -fdirect-access-external-data is currently ignored for -fPIC.

int baz = 42;
__attribute__((dllimport)) extern int import_var;
__attribute__((weak)) extern int weak_bar;
extern int bar;
__attribute__((dllimport)) void import_func(void);

int *use_import(void) {
  import_func();
  return &import_var;
}

void foo(void);

int *zed(void) {
  foo();
  return baz ? &weak_bar : &bar;
}

__thread int local_thread_var = 42;
extern __thread int thread_var;
int *get_thread_var(int a) {
  return a ? &thread_var : &local_thread_var;
}

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
// DEFAULT-NEXT:     global %[[VALUE_baz:[0-9]+]] baz: i32 [storage=static] = const<i32>(42) [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_import_var:[0-9]+]] import_var: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     extern %[[VALUE_weak_bar:[0-9]+]] weak_bar: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     extern %[[VALUE_bar:[0-9]+]] bar: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_local_thread_var:[0-9]+]] local_thread_var: i32 [storage=thread] = const<i32>(42) [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_thread_var:[0-9]+]] thread_var: i32 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_import_func:[0-9]+]] @import_func() -> void [linkage=external] [dllimport];
// DEFAULT-NEXT:     fn %[[VALUE_use_import:[0-9]+]] @use_import() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_import_func]]);
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(%[[VALUE_import_var]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_zed:[0-9]+]] @zed() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:         return conditional<ptr<i32>>(ne<i32>(read<i32>(%[[VALUE_baz]]), const<i32>(0)), addr_of<ptr<i32>>(%[[VALUE_weak_bar]]), addr_of<ptr<i32>>(%[[VALUE_bar]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_get_thread_var:[0-9]+]] @get_thread_var(%[[VALUE_a:[0-9]+]] a: i32) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i32>>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)), addr_of<ptr<i32>>(%[[VALUE_thread_var]]), addr_of<ptr<i32>>(%[[VALUE_local_thread_var]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
