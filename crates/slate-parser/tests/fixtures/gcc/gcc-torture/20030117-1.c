void abort(void);
void exit(int);

int foo(int, int, int);
int bar(int, int, int);

int main(void) {
  if (foo(5, 10, 21) != 12)
    abort();

  if (bar(9, 12, 15) != 150)
    abort();

  exit(0);
}

int foo(int x, int y, int z) { return (x + y + z) / 3; }

int bar(int x, int y, int z) { return foo(x * x, y * y, z * z); }


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
// DEFAULT-NEXT:     fn %1 @exit(%11 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%5 x: i32, %6 y: i32, %7 z: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<i32, by_zero=ub, min_by_neg_one=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%5), read<i32>(%6)), read<i32>(%7)), const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar(%8 x: i32, %9 y: i32, %10 z: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32, i32, i32) -> i32>(%2, mul<i32, overflow=ub>(read<i32>(%8), read<i32>(%8)), mul<i32, overflow=ub>(read<i32>(%9), read<i32>(%9)), mul<i32, overflow=ub>(read<i32>(%10), read<i32>(%10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, i32) -> i32>(%2, const<i32>(5), const<i32>(10), const<i32>(21)), const<i32>(12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, i32) -> i32>(%3, const<i32>(9), const<i32>(12), const<i32>(15)), const<i32>(150))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
