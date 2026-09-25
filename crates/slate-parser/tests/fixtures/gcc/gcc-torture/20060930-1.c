/* PR rtl-optimization/28096 */
/* Origin: Jan Stein <jan@gatespacetelematics.com> */

extern void abort(void);

int bar(int, int) __attribute__((noinline));
int bar(int a, int b) {
  if (b != 1)
    abort();
}

void foo(int, int) __attribute__((noinline));
void foo(int e, int n) {
  int i, bb2, bb5;

  if (e > 0)
    e = -e;

  for (i = 0; i < n; i++) {
    if (e >= 0) {
      bb2 = 0;
      bb5 = 0;
    } else {
      bb5 = -e;
      bb2 = bb5;
    }

    bar(bb5, bb2);
  }
}

int main(void) {
  foo(1, 1);
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
// DEFAULT-NEXT:     fn %1 @bar(%2 a: i32, %3 b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @foo(%5 e: i32, %6 n: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 bb2: i32 [storage=automatic];
// DEFAULT-NEXT:         let %9 bb5: i32 [storage=automatic];
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%5, neg<i32, overflow=ub>(read<i32>(%5)));
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), read<i32>(%6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ge<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:                             write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%9, neg<i32, overflow=ub>(read<i32>(%5)));
// DEFAULT-NEXT:                             write<i32>(%8, read<i32>(%9));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     call<i32, signature=fn(i32, i32) -> i32>(%1, read<i32>(%9), read<i32>(%8));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(1), const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
