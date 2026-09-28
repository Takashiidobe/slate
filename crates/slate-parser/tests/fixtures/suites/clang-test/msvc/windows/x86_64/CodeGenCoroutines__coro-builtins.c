
void *myAlloc(long long);

void f(int n) {
  int promise;

  __builtin_coro_id(32, &promise, 0, 0);

  __builtin_coro_alloc();

  __builtin_coro_noop();

  __builtin_coro_begin(myAlloc(__builtin_coro_size()));

  __builtin_coro_resume(__builtin_coro_frame());

  __builtin_coro_destroy(__builtin_coro_frame());

  __builtin_coro_done(__builtin_coro_frame());

  __builtin_coro_promise(__builtin_coro_frame(), 48, 0);

  __builtin_coro_free(__builtin_coro_frame());

  __builtin_coro_end(__builtin_coro_frame(), 0);

  __builtin_coro_suspend(1);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17

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
// DEFAULT-NEXT:     fn %0 @myAlloc(%4 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %9 @__builtin_coro_id(%5 <unnamed>: i32, %6 <unnamed>: ptr<void>, %7 <unnamed>: ptr<void>, %8 <unnamed>: ptr<void>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %10 @__builtin_coro_alloc() -> bool [linkage=external];
// DEFAULT-NEXT:     fn %11 @__builtin_coro_noop() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %13 @__builtin_coro_begin(%12 <unnamed>: ptr<void>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %14 @__builtin_coro_size() -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %16 @__builtin_coro_resume(%15 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %17 @__builtin_coro_frame() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %19 @__builtin_coro_destroy(%18 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %21 @__builtin_coro_done(%20 <unnamed>: ptr<void>) -> bool [linkage=external];
// DEFAULT-NEXT:     fn %25 @__builtin_coro_promise(%22 <unnamed>: ptr<void>, %23 <unnamed>: i32, %24 <unnamed>: bool) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %27 @__builtin_coro_free(%26 <unnamed>: ptr<void>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %30 @__builtin_coro_end(%28 <unnamed>: ptr<void>, %29 <unnamed>: bool) -> bool [linkage=external];
// DEFAULT-NEXT:     fn %32 @__builtin_coro_suspend(%31 <unnamed>: bool) -> i8 [linkage=external];
// DEFAULT-NEXT:     fn %1 @f(%2 n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 promise: i32 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(i32, ptr<void>, ptr<void>, ptr<void>) -> ptr<void>>(%9, const<i32>(32), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%3)), null<ptr<void>>, null<ptr<void>>);
// DEFAULT-NEXT:         call<bool, signature=fn() -> bool>(%10);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn() -> ptr<void>>(%11);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>) -> ptr<void>>(%13, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%0, reinterpret<i64, reason=arg, fits=unknown>(call<u64, signature=fn() -> u64>(%14))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%16, call<ptr<void>, signature=fn() -> ptr<void>>(%17));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%19, call<ptr<void>, signature=fn() -> ptr<void>>(%17));
// DEFAULT-NEXT:         call<bool, signature=fn(ptr<void>) -> bool>(%21, call<ptr<void>, signature=fn() -> ptr<void>>(%17));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, bool) -> ptr<void>>(%25, call<ptr<void>, signature=fn() -> ptr<void>>(%17), const<i32>(48), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>) -> ptr<void>>(%27, call<ptr<void>, signature=fn() -> ptr<void>>(%17));
// DEFAULT-NEXT:         call<bool, signature=fn(ptr<void>, bool) -> bool>(%30, call<ptr<void>, signature=fn() -> ptr<void>>(%17), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         call<i8, signature=fn(bool) -> i8>(%32, ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
