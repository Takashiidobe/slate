extern void abort(void);

static int max;

static void __attribute__((noinline)) storemax(int i) {
  if (i > max)
    max = i;
}

static int CallFunctionRec(int (*fun)(int depth), int depth) {
  if (!fun(depth)) {
    return 0;
  }
  if (depth < 10) {
    CallFunctionRec(fun, depth + 1);
  }
  return 1;
}

static int CallFunction(int (*fun)(int depth)) {
  return CallFunctionRec(fun, 1) && !fun(0);
}

static int callback(int depth) {
  storemax(depth);
  return depth != 0;
}

int main() {
  CallFunction(callback);
  if (max != 10)
    abort();
  return 0;
}


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
// DEFAULT-NEXT:     global %1 max: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @storemax(%3 i: i32) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%3), read<i32>(%1))
// DEFAULT-NEXT:             write<i32>(%1, read<i32>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @CallFunctionRec(%6 fun: ptr<fn(i32) -> i32>, %7 depth: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(%6), read<i32>(%7)), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%7), const<i32>(10))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<fn(i32) -> i32>, i32) -> i32>(%4, read<ptr<fn(i32) -> i32>>(%6), add<i32, overflow=ub>(read<i32>(%7), const<i32>(1)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @CallFunction(%10 fun: ptr<fn(i32) -> i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<fn(i32) -> i32>, i32) -> i32>(%4, read<ptr<fn(i32) -> i32>>(%10), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%14, not<bool>(ne<i32>(call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(%10), const<i32>(0)), const<i32>(0))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%14, const<bool>(false));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(read<bool>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @callback(%12 depth: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, read<i32>(%12));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(read<i32>(%12), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<fn(i32) -> i32>) -> i32>(%8, function_decay<ptr<fn(i32) -> i32>>(%11));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
