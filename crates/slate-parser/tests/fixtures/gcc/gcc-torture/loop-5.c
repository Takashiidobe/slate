void abort(void);
void exit(int);

static int  ap(int i);
static void testit(void) {
  int ir[4] = {0, 1, 2, 3};
  int ix, n, m;
  n = 1;
  m = 3;
  for (ix = 1; ix <= 4; ix++) {
    if (n == 1)
      m = 4;
    else
      m = n - 1;
    ap(ir[n - 1]);
    n = m;
  }
}

static int t = 0;
static int a[4];

static int ap(int i) {
  if (t > 3)
    abort();
  a[t++] = i;
  return 1;
}

int main(void) {
  testit();
  if (a[0] != 0)
    abort();
  if (a[1] != 3)
    abort();
  if (a[2] != 2)
    abort();
  if (a[3] != 1)
    abort();
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
// DEFAULT-NEXT:     global %8 t: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %9 a: array<i32, 4> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%12 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @ap(%10 i: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%8), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %15: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%16));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%9), read<i32>(%15))), read<i32>(%10));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @testit() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 ir: array<i32, 4> [storage=automatic] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2), index3 = const<i32>(3));
// DEFAULT-NEXT:         let %5 ix: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 n: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 m: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%6, const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%7, const<i32>(3));
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(1));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%5), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%18));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%6), const<i32>(1))
// DEFAULT-NEXT:                         write<i32>(%7, const<i32>(4));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<i32>(%7, sub<i32, overflow=ub>(read<i32>(%6), const<i32>(1)));
// DEFAULT-NEXT:                     call<i32, signature=fn(i32) -> i32>(%2, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%4), sub<i32, overflow=ub>(read<i32>(%6), const<i32>(1))))));
// DEFAULT-NEXT:                     write<i32>(%6, read<i32>(%7));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%9), const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%9), const<i32>(1)))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%9), const<i32>(2)))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%9), const<i32>(3)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
