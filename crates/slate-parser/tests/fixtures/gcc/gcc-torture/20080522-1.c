/* This testcase is to make sure we have i in referenced vars and that we
   properly compute aliasing for the loads and stores.  */

extern void abort(void);

static int  i;
static int *p = &i;

int __attribute__((noinline)) foo(int *q) {
  *p = 1;
  *q = 2;
  return *p;
}

int __attribute__((noinline)) bar(int *q) {
  *q = 2;
  *p = 1;
  return *q;
}

int main() {
  int j = 0;

  if (foo(&i) != 2)
    abort();
  if (bar(&i) != 1)
    abort();
  if (foo(&j) != 1)
    abort();
  if (j != 2)
    abort();
  if (bar(&j) != 2)
    abort();
  if (j != 2)
    abort();

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
// DEFAULT-NEXT:     global %1 i: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %2 p: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%1) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 q: ptr<i32>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%2)), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%4)), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32>>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar(%6 q: ptr<i32>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%6)), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%2)), const<i32>(1));
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32>>(%6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<i32>) -> i32>(%3, addr_of<ptr<i32>>(%1)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<i32>) -> i32>(%5, addr_of<ptr<i32>>(%1)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<i32>) -> i32>(%3, addr_of<ptr<i32>>(%8)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<i32>) -> i32>(%5, addr_of<ptr<i32>>(%8)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
