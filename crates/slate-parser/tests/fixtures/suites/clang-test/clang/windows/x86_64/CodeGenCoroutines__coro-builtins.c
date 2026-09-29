
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
// DEFAULT-NEXT:     fn %[[VALUE_myAlloc:[0-9]+]] @myAlloc(%[[VALUE0:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_coro_id:[0-9]+]] @__builtin_coro_id(%[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE3:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE4:[0-9]+]] <unnamed>: ptr<void>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_coro_alloc:[0-9]+]] @__builtin_coro_alloc() -> bool [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_coro_noop:[0-9]+]] @__builtin_coro_noop() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_coro_begin:[0-9]+]] @__builtin_coro_begin(%[[VALUE5:[0-9]+]] <unnamed>: ptr<void>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_coro_size:[0-9]+]] @__builtin_coro_size() -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_coro_resume:[0-9]+]] @__builtin_coro_resume(%[[VALUE6:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_coro_frame:[0-9]+]] @__builtin_coro_frame() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_coro_destroy:[0-9]+]] @__builtin_coro_destroy(%[[VALUE7:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_coro_done:[0-9]+]] @__builtin_coro_done(%[[VALUE8:[0-9]+]] <unnamed>: ptr<void>) -> bool [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_coro_promise:[0-9]+]] @__builtin_coro_promise(%[[VALUE9:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE10:[0-9]+]] <unnamed>: i32, %[[VALUE11:[0-9]+]] <unnamed>: bool) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_coro_free:[0-9]+]] @__builtin_coro_free(%[[VALUE12:[0-9]+]] <unnamed>: ptr<void>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_coro_end:[0-9]+]] @__builtin_coro_end(%[[VALUE13:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE14:[0-9]+]] <unnamed>: bool) -> bool [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_coro_suspend:[0-9]+]] @__builtin_coro_suspend(%[[VALUE15:[0-9]+]] <unnamed>: bool) -> i8 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_promise:[0-9]+]] promise: i32 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(i32, ptr<void>, ptr<void>, ptr<void>) -> ptr<void>>(%[[VALUE___builtin_coro_id]], const<i32>(32), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_promise]])), null<ptr<void>>, null<ptr<void>>);
// DEFAULT-NEXT:         call<bool, signature=fn() -> bool>(%[[VALUE___builtin_coro_alloc]]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE___builtin_coro_noop]]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>) -> ptr<void>>(%[[VALUE___builtin_coro_begin]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_myAlloc]], reinterpret<i64, reason=arg, fits=unknown>(call<u64, signature=fn() -> u64>(%[[VALUE___builtin_coro_size]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_coro_resume]], call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE___builtin_coro_frame]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_coro_destroy]], call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE___builtin_coro_frame]]));
// DEFAULT-NEXT:         call<bool, signature=fn(ptr<void>) -> bool>(%[[VALUE___builtin_coro_done]], call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE___builtin_coro_frame]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, bool) -> ptr<void>>(%[[VALUE___builtin_coro_promise]], call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE___builtin_coro_frame]]), const<i32>(48), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>) -> ptr<void>>(%[[VALUE___builtin_coro_free]], call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE___builtin_coro_frame]]));
// DEFAULT-NEXT:         call<bool, signature=fn(ptr<void>, bool) -> bool>(%[[VALUE___builtin_coro_end]], call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE___builtin_coro_frame]]), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         call<i8, signature=fn(bool) -> i8>(%[[VALUE___builtin_coro_suspend]], ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
