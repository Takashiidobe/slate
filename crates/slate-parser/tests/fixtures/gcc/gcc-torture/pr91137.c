long long a;
unsigned  b;
int       c[70];
int       d[70][70];
int       e;

__attribute__((noinline)) void f(long long *g, int p2) { *g = p2; }

__attribute__((noinline)) void fn2() {
  for (int j = 0; j < 70; j++) {
    for (int i = 0; i < 70; i++) {
      if (b)
        c[i] = 0;
      for (int l = 0; l < 70; l++)
        d[i][1] = d[l][i];
    }
    for (int k = 0; k < 70; k++)
      e = c[0];
  }
}

int main() {
  b = 5;
  for (int j = 0; j < 70; ++j)
    c[j] = 2075593088;
  fn2();
  f(&a, e);
  if (a)
    __builtin_abort();
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
// DEFAULT-NEXT:     global %0 a: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: array<i32, 70> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: array<array<i32, 70>, 70> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %5 @f(%6 g: ptr<i64>, %7 p2: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%6)), widen<i64, reason=assign>(read<i32>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @fn2() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %9 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%9), const<i32>(70))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%21));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %16
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %10 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%10), const<i32>(70))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %22: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                             let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%10, read<i32>(%23));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%1), const<u32>(0))
// DEFAULT-NEXT:                                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(70)>(%2), read<i32>(%10))), const<i32>(0));
// DEFAULT-NEXT:                                 for %17
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         let %11 l: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                                     condition: lt<i32>(read<i32>(%11), const<i32>(70))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %24: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                                         let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%11, read<i32>(%25));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(70)>(deref(ptr_offset<ptr<array<i32, 70>>, subtract=false, element=array<i32, 70>, overflow=ub>(array_decay<ptr<array<i32, 70>>, length=Some(70)>(%3), read<i32>(%10)))), const<i32>(1))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(70)>(deref(ptr_offset<ptr<array<i32, 70>>, subtract=false, element=array<i32, 70>, overflow=ub>(array_decay<ptr<array<i32, 70>>, length=Some(70)>(%3), read<i32>(%11)))), read<i32>(%10)))));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     for %18
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %12 k: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%12), const<i32>(70))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %26: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                             let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%12, read<i32>(%27));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<i32>(%4, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(70)>(%2), const<i32>(0)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<u32>(%1, reinterpret<u32, reason=assign, fits=always>(const<i32>(5)));
// DEFAULT-NEXT:         for %19
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %14 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%14), const<i32>(70))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%29));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(70)>(%2), read<i32>(%14))), const<i32>(2075593088));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i64>, i32) -> void>(%5, addr_of<ptr<i64>>(%0), read<i32>(%4));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%0), const<i64>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
