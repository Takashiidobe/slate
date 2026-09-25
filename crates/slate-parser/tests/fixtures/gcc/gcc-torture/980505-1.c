void abort(void);
void exit(int);

static int f(int) __attribute__((const));
int        main() {
  int f1, f2, x;
  x  = 1;
  f1 = f(x);
  x  = 2;
  f2 = f(x);
  if (f1 != 1 || f2 != 2)
    abort();
  exit(0);
}
static int f(int x) { return x; }


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
// DEFAULT-NEXT:     fn %1 @exit(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @f(%7 x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 f1: i32 [storage=automatic];
// DEFAULT-NEXT:         let %5 f2: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 x: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%6, const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%4, call<i32, signature=fn(i32) -> i32>(%2, read<i32>(%6)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%2, read<i32>(%6));
// DEFAULT-NEXT:         write<i32>(%6, const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%5, call<i32, signature=fn(i32) -> i32>(%2, read<i32>(%6)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%2, read<i32>(%6));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%4), const<i32>(1)), ne<i32>(read<i32>(%5), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
