void abort(void);
void exit(int);

int a[2] = {2, 0};

void foo(int *sp, int cnt) {
  int *p, *top;

  top  = sp;
  sp  -= cnt;

  for (p = sp; p <= top; p++)
    if (*p < 2)
      exit(0);
}

int main() {
  foo(a + 1, 1);
  abort();
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
// DEFAULT-NEXT:     global %2 a: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @foo(%4 sp: ptr<i32>, %5 cnt: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 p: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %7 top: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%7, read<ptr<i32>>(%4));
// DEFAULT-NEXT:         let %11: ptr<i32> [synthetic] = read<ptr<i32>>(%4);
// DEFAULT-NEXT:         let %12: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%11), read<i32>(%5));
// DEFAULT-NEXT:         write<ptr<i32>>(%4, read<ptr<i32>>(%12));
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<ptr<i32>>(%6, read<ptr<i32>>(%4));
// DEFAULT-NEXT:             condition: le<ptr<i32>>(read<ptr<i32>>(%6), read<ptr<i32>>(%7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %13: ptr<i32> [synthetic] = read<ptr<i32>>(%6);
// DEFAULT-NEXT:                 let %14: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%13), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%6, read<ptr<i32>>(%14));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if lt<i32>(read<i32>(deref(read<ptr<i32>>(%6))), const<i32>(2))
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%3, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%2), const<i32>(1)), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
