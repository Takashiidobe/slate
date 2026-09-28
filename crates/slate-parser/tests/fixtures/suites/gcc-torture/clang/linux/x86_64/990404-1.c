void abort(void);
void exit(int);

int x[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main() {
  int niterations = 0, i;

  for (;;) {
    int i, mi, max;
    max = 0;
    for (i = 0; i < 10; i++) {
      if (x[i] > max) {
        max = x[i];
        mi  = i;
      }
    }
    if (max == 0)
      break;
    x[mi] = 0;
    niterations++;
    if (niterations > 10)
      abort();
  }

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
// DEFAULT-NEXT:     global %2 x: array<i32, 10> [storage=static] [align=16] = aggregate<array<i32, 10>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2), index3 = const<i32>(3), index4 = const<i32>(4), index5 = const<i32>(5), index6 = const<i32>(6), index7 = const<i32>(7), index8 = const<i32>(8), index9 = const<i32>(9)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 niterations: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %6 i: i32 [storage=automatic];
// DEFAULT-NEXT:                     let %7 mi: i32 [storage=automatic];
// DEFAULT-NEXT:                     let %8 max: i32 [storage=automatic];
// DEFAULT-NEXT:                     write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:                     for %11
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%6), const<i32>(10))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %12: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                             let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%6, read<i32>(%13));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if gt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%2), read<i32>(%6)))), read<i32>(%8))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         write<i32>(%8, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%2), read<i32>(%6)))));
// DEFAULT-NEXT:                                         write<i32>(%7, read<i32>(%6));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:                         break %10;
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%2), read<i32>(%7))), const<i32>(0));
// DEFAULT-NEXT:                     let %14: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                     let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%4, read<i32>(%15));
// DEFAULT-NEXT:                     if gt<i32>(read<i32>(%4), const<i32>(10))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
