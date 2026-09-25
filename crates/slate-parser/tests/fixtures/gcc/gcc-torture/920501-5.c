/* { dg-require-effective-target label_values } */

void abort(void);
void exit(int);

int x(int i) {
  void *j[] = {&&x, &&y, &&z};
  goto *j[i];
x:
  return 2;
y:
  return 3;
z:
  return 5;
}
int main(void) {
  if (x(0) != 2 || x(1) != 3 || x(2) != 5)
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
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @x(%6 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 j: array<ptr<void>, 3> [storage=automatic] = aggregate<array<ptr<void>, 3>, zero_fill=false>(index0 = label_addr<ptr<void>>(%3), index1 = label_addr<ptr<void>>(%4), index2 = label_addr<ptr<void>>(%5));
// DEFAULT-NEXT:         goto *read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(3)>(%7), read<i32>(%6))));
// DEFAULT-NEXT:         label %3 x:
// DEFAULT-NEXT:             return const<i32>(2);
// DEFAULT-NEXT:         label %4 y:
// DEFAULT-NEXT:             return const<i32>(3);
// DEFAULT-NEXT:         label %5 z:
// DEFAULT-NEXT:             return const<i32>(5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%2, const<i32>(0)), const<i32>(2))
// DEFAULT-NEXT:             write<bool>(%10, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%10, ne<i32>(call<i32, signature=fn(i32) -> i32>(%2, const<i32>(1)), const<i32>(3)));
// DEFAULT-NEXT:         let %11: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%10)
// DEFAULT-NEXT:             write<bool>(%11, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%11, ne<i32>(call<i32, signature=fn(i32) -> i32>(%2, const<i32>(2)), const<i32>(5)));
// DEFAULT-NEXT:         if read<bool>(%11)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
