void abort(void);
void exit(int);

void f(int i, int j, int radius, int width, int N) {
  const int diff = i - radius;
  const int lowk = (diff > 0 ? diff : 0);
  int       k;

  for (k = lowk; k <= 2; k++) {
    int idx = ((k - i + radius) * width - j + radius);
    if (idx < 0)
      abort();
  }

  for (k = lowk; k <= 2; k++)
    ;
}

int main(int argc, char **argv) {
  int exc_rad = 2;
  int N       = 8;
  int i;
  for (i = 1; i < 4; i++)
    f(i, 1, exc_rad, 2 * exc_rad + 1, N);
  exit(0);
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
// DEFAULT-NEXT:     fn %1 @exit(%18 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @f(%3 i: i32, %4 j: i32, %5 radius: i32, %6 width: i32, %7 N: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 diff: i32 [storage=automatic] [const] = sub<i32, overflow=ub>(read<i32>(%3), read<i32>(%5));
// DEFAULT-NEXT:         let %9 lowk: i32 [storage=automatic] [const] = conditional<i32>(gt<i32>(read<i32>(%8), const<i32>(0)), read<i32>(%8), const<i32>(0));
// DEFAULT-NEXT:         let %10 k: i32 [storage=automatic];
// DEFAULT-NEXT:         for %19
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%9));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%10), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%23));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %11 idx: i32 [storage=automatic] = add<i32, overflow=ub>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%10), read<i32>(%3)), read<i32>(%5)), read<i32>(%6)), read<i32>(%4)), read<i32>(%5));
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(%11), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %20
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%9));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%10), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%25));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main(%13 argc: i32, %14 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15 exc_rad: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %16 N: i32 [storage=automatic] = const<i32>(8);
// DEFAULT-NEXT:         let %17 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %21
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%17, const<i32>(1));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%17), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32>(%27));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32, i32, i32, i32) -> void>(%2, read<i32>(%17), const<i32>(1), read<i32>(%15), add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(2), read<i32>(%15)), const<i32>(1)), read<i32>(%16));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
