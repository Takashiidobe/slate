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
// DEFAULT-NEXT:     global %[[VALUE_ops:[0-9]+]] ops: array<i32, 13> [storage=static] [align=16] = aggregate<array<i32, 13>, zero_fill=false>(index0 = const<i32>(11), index1 = const<i32>(12), index2 = const<i32>(46), index3 = const<i32>(3), index4 = const<i32>(2), index5 = const<i32>(2), index6 = const<i32>(3), index7 = const<i32>(2), index8 = const<i32>(1), index9 = const<i32>(3), index10 = const<i32>(2), index11 = const<i32>(1), index12 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_correct:[0-9]+]] correct: array<i32, 13> [storage=static] [align=16] = aggregate<array<i32, 13>, zero_fill=false>(index0 = const<i32>(46), index1 = const<i32>(12), index2 = const<i32>(11), index3 = const<i32>(3), index4 = const<i32>(3), index5 = const<i32>(3), index6 = const<i32>(2), index7 = const<i32>(2), index8 = const<i32>(2), index9 = const<i32>(2), index10 = const<i32>(2), index11 = const<i32>(1), index12 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_num:[0-9]+]] num: i32 [storage=static] = const<i32>(13) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_num]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:                     for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], sub<i32, overflow=ub>(read<i32>(%[[VALUE_num]]), const<i32>(1)));
// DEFAULT-NEXT:                         condition: gt<i32>(read<i32>(%[[VALUE_j]]), read<i32>(%[[VALUE_i]]))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                             let %[[VALUE6:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if lt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(13)>(%[[VALUE_ops]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_j]]), const<i32>(1))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(13)>(%[[VALUE_ops]]), read<i32>(%[[VALUE_j]])))))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %[[VALUE_op:[0-9]+]] op: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(13)>(%[[VALUE_ops]]), read<i32>(%[[VALUE_j]]))));
// DEFAULT-NEXT:                                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(13)>(%[[VALUE_ops]]), read<i32>(%[[VALUE_j]]))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(13)>(%[[VALUE_ops]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_j]]), const<i32>(1))))));
// DEFAULT-NEXT:                                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(13)>(%[[VALUE_ops]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_j]]), const<i32>(1)))), read<i32>(%[[VALUE_op]]));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_num]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(13)>(%[[VALUE_ops]]), read<i32>(%[[VALUE_i]])))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(13)>(%[[VALUE_correct]]), read<i32>(%[[VALUE_i]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
