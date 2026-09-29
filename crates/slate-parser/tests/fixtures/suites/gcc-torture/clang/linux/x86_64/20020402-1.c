/* derived from PR c/2100 */

extern void abort();
extern void exit(int);

#define SMALL_N  2
#define NUM_ELEM 4

int main(void) {
  int listElem[NUM_ELEM] = {30, 2, 10, 5};
  int listSmall[SMALL_N];
  int i, j;
  int posGreatest = -1, greatest = -1;

  for (i = 0; i < SMALL_N; i++) {
    listSmall[i] = listElem[i];
    if (listElem[i] > greatest) {
      posGreatest = i;
      greatest    = listElem[i];
    }
  }

  for (i = SMALL_N; i < NUM_ELEM; i++) {
    if (listElem[i] < greatest) {
      listSmall[posGreatest] = listElem[i];
      posGreatest            = 0;
      greatest               = listSmall[0];
      for (j = 1; j < SMALL_N; j++)
        if (listSmall[j] > greatest) {
          posGreatest = j;
          greatest    = listSmall[j];
        }
    }
  }

  if (listSmall[0] != 5 || listSmall[1] != 2)
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_listElem:[0-9]+]] listElem: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(30), index1 = const<i32>(2), index2 = const<i32>(10), index3 = const<i32>(5));
// DEFAULT-NEXT:         let %[[VALUE_listSmall:[0-9]+]] listSmall: array<i32, 2> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_posGreatest:[0-9]+]] posGreatest: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_greatest:[0-9]+]] greatest: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_listSmall]]), read<i32>(%[[VALUE_i]]))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_listElem]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                     if gt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_listElem]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_greatest]]))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_posGreatest]], read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_greatest]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_listElem]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(2));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_listElem]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_greatest]]))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_listSmall]]), read<i32>(%[[VALUE_posGreatest]]))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_listElem]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_posGreatest]], const<i32>(0));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_greatest]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_listSmall]]), const<i32>(0)))));
// DEFAULT-NEXT:                             for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_j]], const<i32>(1));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(2))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                                     let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if gt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_listSmall]]), read<i32>(%[[VALUE_j]])))), read<i32>(%[[VALUE_greatest]]))
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_posGreatest]], read<i32>(%[[VALUE_j]]));
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_greatest]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_listSmall]]), read<i32>(%[[VALUE_j]])))));
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_listSmall]]), const<i32>(0)))), const<i32>(5)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_listSmall]]), const<i32>(1)))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
