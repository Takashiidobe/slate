extern void abort(void);
void        foo(int *p) {
  int x;
  int y;
  x  = *p;
  *p = 0;
  y  = *p;
  if (x != y)
    return;
  abort();
}

int main() {
  int a = 1;
  foo(&a);
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
// DEFAULT-NEXT:     fn %1 @foo(%2 p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 x: i32 [storage=automatic];
// DEFAULT-NEXT:         let %4 y: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(deref(read<ptr<i32>>(%2))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%2)), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(deref(read<ptr<i32>>(%2))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), read<i32>(%4))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 a: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%1, addr_of<ptr<i32>>(%6));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
