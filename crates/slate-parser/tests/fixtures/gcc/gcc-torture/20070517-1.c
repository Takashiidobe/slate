/* PR rtl-optimization/31691 */
/* Origin: Chi-Hua Chen <stephaniechc-gccbug@yahoo.com> */

extern void abort(void);

static int get_kind(int) __attribute__((noinline));

static int get_kind(int v) {
  volatile int k = v;
  return k;
}

static int some_call(void) __attribute__((noinline));

static int some_call(void) { return 0; }

static void example(int arg) {
  int tmp, kind = get_kind(arg);

  if (kind == 9 || kind == 10 || kind == 5) {
    if (some_call() == 0) {
      if (kind == 9 || kind == 10)
        tmp = arg;
      else
        abort();
    }
  }
}

int main(void) {
  example(10);
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @get_kind(%2 v: i32) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 k: volatile i32 [storage=automatic] = read<i32>(%2);
// DEFAULT-NEXT:         return read<i32, volatile>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @some_call() -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @example(%6 arg: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 tmp: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 kind: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%6));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(eq<i32>(read<i32>(%8), const<i32>(9)), eq<i32>(read<i32>(%8), const<i32>(10))), eq<i32>(read<i32>(%8), const<i32>(5)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<i32>(call<i32, signature=fn() -> i32>(%4), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if logical_or<bool>(eq<i32>(read<i32>(%8), const<i32>(9)), eq<i32>(read<i32>(%8), const<i32>(10)))
// DEFAULT-NEXT:                             write<i32>(%7, read<i32>(%6));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%5, const<i32>(10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
