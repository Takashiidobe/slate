void abort(void);
void exit(int);

int ops[13] = {11, 12, 46, 3, 2, 2, 3, 2, 1, 3, 2, 1, 2};

int correct[13] = {46, 12, 11, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1};

int num = 13;

int main() {
  int i;

  for (i = 0; i < num; i++) {
    int j;

    for (j = num - 1; j > i; j--) {
      if (ops[j - 1] < ops[j]) {
        int op     = ops[j];
        ops[j]     = ops[j - 1];
        ops[j - 1] = op;
      }
    }
  }

  for (i = 0; i < num; i++)
    if (ops[i] != correct[i])
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
// DEFAULT-NEXT:     global %2 ops: array<i32, 13> [storage=static] [align=16] = aggregate<array<i32, 13>, zero_fill=false>(index0 = const<i32>(11), index1 = const<i32>(12), index2 = const<i32>(46), index3 = const<i32>(3), index4 = const<i32>(2), index5 = const<i32>(2), index6 = const<i32>(3), index7 = const<i32>(2), index8 = const<i32>(1), index9 = const<i32>(3), index10 = const<i32>(2), index11 = const<i32>(1), index12 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %3 correct: array<i32, 13> [storage=static] [align=16] = aggregate<array<i32, 13>, zero_fill=false>(index0 = const<i32>(46), index1 = const<i32>(12), index2 = const<i32>(11), index3 = const<i32>(3), index4 = const<i32>(3), index5 = const<i32>(3), index6 = const<i32>(2), index7 = const<i32>(2), index8 = const<i32>(2), index9 = const<i32>(2), index10 = const<i32>(2), index11 = const<i32>(1), index12 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %4 num: i32 [storage=static] = const<i32>(13) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%6), read<i32>(%4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%14));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %7 j: i32 [storage=automatic];
// DEFAULT-NEXT:                     for %11
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%7, sub<i32, overflow=ub>(read<i32>(%4), const<i32>(1)));
// DEFAULT-NEXT:                         condition: gt<i32>(read<i32>(%7), read<i32>(%6))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %15: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                             let %16: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%7, read<i32>(%16));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if lt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(13)>(%2), sub<i32, overflow=ub>(read<i32>(%7), const<i32>(1))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(13)>(%2), read<i32>(%7)))))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %8 op: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(13)>(%2), read<i32>(%7))));
// DEFAULT-NEXT:                                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(13)>(%2), read<i32>(%7))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(13)>(%2), sub<i32, overflow=ub>(read<i32>(%7), const<i32>(1))))));
// DEFAULT-NEXT:                                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(13)>(%2), sub<i32, overflow=ub>(read<i32>(%7), const<i32>(1)))), read<i32>(%8));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%6), read<i32>(%4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%18));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(13)>(%2), read<i32>(%6)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(13)>(%3), read<i32>(%6)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
