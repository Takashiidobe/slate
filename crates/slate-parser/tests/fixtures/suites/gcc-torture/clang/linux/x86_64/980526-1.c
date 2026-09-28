/* { dg-require-effective-target label_values } */

void abort(void);
void exit(int);

int expect_do1 = 1, expect_do2 = 2;

static int doit(int x) {
  __label__ lbl1;
  __label__ lbl2;
  static int   jtab_init = 0;
  static void *jtab[2];

  if (!jtab_init) {
    jtab[0]   = &&lbl1;
    jtab[1]   = &&lbl2;
    jtab_init = 1;
  }
  goto *jtab[x];
lbl1:
  return 1;
lbl2:
  return 2;
}

static void do1(void) {
  if (doit(0) != expect_do1)
    abort();
}

static void do2(void) {
  if (doit(1) != expect_do2)
    abort();
}

int main(void) { exit(0); }


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
// DEFAULT-NEXT:     global %2 expect_do1: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %3 expect_do2: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %8 jtab_init: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %9 jtab: array<ptr<void>, 2> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%13 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @doit(%7 x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%8), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(2)>(%9), const<i32>(0))), label_addr<ptr<void>>(%5));
// DEFAULT-NEXT:                 write<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(2)>(%9), const<i32>(1))), label_addr<ptr<void>>(%6));
// DEFAULT-NEXT:                 write<i32>(%8, const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         goto *read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(2)>(%9), read<i32>(%7))));
// DEFAULT-NEXT:         label %5 lbl1:
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         label %6 lbl2:
// DEFAULT-NEXT:             return const<i32>(2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @do1() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%4, const<i32>(0)), read<i32>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @do2() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%4, const<i32>(1)), read<i32>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
