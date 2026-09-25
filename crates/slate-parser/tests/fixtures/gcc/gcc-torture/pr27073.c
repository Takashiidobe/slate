void abort(void);
void exit(int);

void __attribute__((noinline)) foo(int *p, int d1, int d2, int d3, short count,
                                   int s1, int s2, int s3, int s4, int s5) {
  int n = count;
  while (n--) {
    *p++ = s1;
    *p++ = s2;
    *p++ = s3;
    *p++ = s4;
    *p++ = s5;
  }
}

int main() {
  int x[10], i;

  foo(x, 0, 0, 0, 2, 100, 200, 300, 400, 500);
  for (i = 0; i < 10; i++)
    if (x[i] != (i % 5 + 1) * 100)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%17 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 p: ptr<i32>, %4 d1: i32, %5 d2: i32, %6 d3: i32, %7 count: i16, %8 s1: i32, %9 s2: i32, %10 s3: i32, %11 s4: i32, %12 s5: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 n: i32 [storage=automatic] = widen<i32, reason=assign>(read<i16>(%7));
// DEFAULT-NEXT:         while %18 {
// DEFAULT-NEXT:             let %20: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:             let %21: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%13, read<i32>(%21));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%20), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %22: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:                 let %23: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%22), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%3, read<ptr<i32>>(%23));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%22)), read<i32>(%8));
// DEFAULT-NEXT:                 let %24: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:                 let %25: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%3, read<ptr<i32>>(%25));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%24)), read<i32>(%9));
// DEFAULT-NEXT:                 let %26: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:                 let %27: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%26), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%3, read<ptr<i32>>(%27));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%26)), read<i32>(%10));
// DEFAULT-NEXT:                 let %28: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:                 let %29: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%28), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%3, read<ptr<i32>>(%29));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%28)), read<i32>(%11));
// DEFAULT-NEXT:                 let %30: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:                 let %31: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%30), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%3, read<ptr<i32>>(%31));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%30)), read<i32>(%12));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15 x: array<i32, 10> [storage=automatic];
// DEFAULT-NEXT:         let %16 i: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32, i32, i32, i16, i32, i32, i32, i32, i32) -> void>(%2, array_decay<ptr<i32>, length=Some(10)>(%15), const<i32>(0), const<i32>(0), const<i32>(0), truncate<i16, reason=arg, fits=always>(const<i32>(2)), const<i32>(100), const<i32>(200), const<i32>(300), const<i32>(400), const<i32>(500));
// DEFAULT-NEXT:         for %19
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%16), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %32: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%33));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%15), read<i32>(%16)))), mul<i32, overflow=ub>(add<i32, overflow=ub>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%16), const<i32>(5)), const<i32>(1)), const<i32>(100)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
