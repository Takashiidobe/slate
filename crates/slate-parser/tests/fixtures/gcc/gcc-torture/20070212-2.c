int f(int k, int i1, int j1) {
  int *f1;
  if (k)
    f1 = &i1;
  else
    f1 = &j1;
  i1 = 0;
  return *f1;
}

extern void abort(void);

int main() {
  if (f(1, 1, 2) != 0)
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
// DEFAULT-NEXT:     fn %0 @f(%1 k: i32, %2 i1: i32, %3 j1: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 f1: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             write<ptr<i32>>(%4, addr_of<ptr<i32>>(%2));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<ptr<i32>>(%4, addr_of<ptr<i32>>(%3));
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32>>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, i32) -> i32>(%0, const<i32>(1), const<i32>(1), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
