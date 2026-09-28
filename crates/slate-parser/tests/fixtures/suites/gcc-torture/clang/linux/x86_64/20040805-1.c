/* { dg-require-stack-size "0x12000" } */

void abort(void);
void exit(int);

#if __INT_MAX__ < 32768
int main() { exit(0); }
#else
int a[2] = {2, 3};

static int __attribute__((noinline)) bar(int x, void *b) {
  a[0]++;
  return x;
}

static int __attribute__((noinline)) foo(int x) {
  char buf[0x10000];
  int  y = a[0];
  a[1]   = y;
  x      = bar(x, buf);
  y      = bar(y, buf);
  return x + y;
}

int main() {
  if (foo(100) != 102)
    abort();
  exit(0);
}
#endif



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
// DEFAULT-NEXT:     global %2 a: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(3)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%11 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @bar(%4 x: i32, %5 b: ptr<void>) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%2), const<i32>(0));
// DEFAULT-NEXT:         let %13: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%12)));
// DEFAULT-NEXT:         let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%12)), read<i32>(%14));
// DEFAULT-NEXT:         return read<i32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @foo(%7 x: i32) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 buf: array<i8, 65536> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %9 y: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%2), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%2), const<i32>(1))), read<i32>(%9));
// DEFAULT-NEXT:         write<i32>(%7, call<i32, signature=fn(i32, ptr<void>) -> i32>(%3, read<i32>(%7), pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(65536)>(%8))));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<void>) -> i32>(%3, read<i32>(%7), pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(65536)>(%8)));
// DEFAULT-NEXT:         write<i32>(%9, call<i32, signature=fn(i32, ptr<void>) -> i32>(%3, read<i32>(%9), pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(65536)>(%8))));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<void>) -> i32>(%3, read<i32>(%9), pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(65536)>(%8)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%7), read<i32>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%6, const<i32>(100)), const<i32>(102))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
