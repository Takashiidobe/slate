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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_get_kind:[0-9]+]] @get_kind(%[[VALUE_v:[0-9]+]] v: i32) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: volatile i32 [storage=automatic] = read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:         return read<i32, volatile>(%[[VALUE_k]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_some_call:[0-9]+]] @some_call() -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_example:[0-9]+]] @example(%[[VALUE_arg:[0-9]+]] arg: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_tmp:[0-9]+]] tmp: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_kind:[0-9]+]] kind: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%[[VALUE_get_kind]], read<i32>(%[[VALUE_arg]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(eq<i32>(read<i32>(%[[VALUE_kind]]), const<i32>(9)), eq<i32>(read<i32>(%[[VALUE_kind]]), const<i32>(10))), eq<i32>(read<i32>(%[[VALUE_kind]]), const<i32>(5)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_some_call]]), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if logical_or<bool>(eq<i32>(read<i32>(%[[VALUE_kind]]), const<i32>(9)), eq<i32>(read<i32>(%[[VALUE_kind]]), const<i32>(10)))
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_tmp]], read<i32>(%[[VALUE_arg]]));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_example]], const<i32>(10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
