extern void abort();

void __attribute__((noinline)) bar(unsigned long long i) {
  if (i)
    abort();
}

static void __attribute__((always_inline)) foo(unsigned long long *r) {
  int i;

  for (i = 0;; i++)
    if (*r & ((unsigned long long)1 << (63 - i)))
      break;

  bar(i);
}

void __attribute__((noinline)) do_test(unsigned long long *r) {
  int i;

  for (i = 0; i < 2; ++i)
    foo(r);
}

int main() {
  unsigned long long r = 0x8000000000000001ull;

  do_test(&r);
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
// DEFAULT-NEXT:     fn %1 @bar(%2 i: u64) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%2), const<u64>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @foo(%4 r: ptr<u64>) -> void [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%14));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(and<u64>(read<u64>(deref(read<ptr<u64>>(%4))), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), sub<i32, overflow=ub>(const<i32>(63), read<i32>(%5)))), const<u64>(0))
// DEFAULT-NEXT:                     break %11;
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%1, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @do_test(%7 r: ptr<u64>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%16));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<u64>) -> void>(%3, read<ptr<u64>>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 r: u64 [storage=automatic] = const<u64>(9223372036854775809);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u64>) -> void>(%6, addr_of<ptr<u64>>(%10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
