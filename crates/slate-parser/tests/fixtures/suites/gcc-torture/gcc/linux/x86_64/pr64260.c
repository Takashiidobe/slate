/* PR rtl-optimization/64260 */

int a = 1, b;

void foo(char p) {
  int t = 0;
  for (; b < 1; b++) {
    int *s = &a;
    if (--t)
      *s &= p;
    *s &= 1;
  }
}

int main() {
  foo(0);
  if (a != 0)
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 p: i8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 t: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %7
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%10));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %5 s: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%0);
// DEFAULT-NEXT:                     let %11: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                     let %12: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%4, read<i32>(%12));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%12), const<i32>(0))
// DEFAULT-NEXT:                         let %13: ptr<i32> [synthetic] = read<ptr<i32>>(%5);
// DEFAULT-NEXT:                         let %14: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%13)));
// DEFAULT-NEXT:                         let %15: i32 [synthetic] = and<i32>(read<i32>(%14), widen<i32, reason=promotion>(read<i8>(%3)));
// DEFAULT-NEXT:                         write<i32>(deref(read<ptr<i32>>(%13)), read<i32>(%15));
// DEFAULT-NEXT:                     let %16: ptr<i32> [synthetic] = read<ptr<i32>>(%5);
// DEFAULT-NEXT:                     let %17: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%16)));
// DEFAULT-NEXT:                     let %18: i32 [synthetic] = and<i32>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%16)), read<i32>(%18));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i8) -> void>(%2, truncate<i8, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
