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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 listElem: array<i32, 4> [storage=automatic] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(30), index1 = const<i32>(2), index2 = const<i32>(10), index3 = const<i32>(5));
// DEFAULT-NEXT:         let %4 listSmall: array<i32, 2> [storage=automatic];
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 posGreatest: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %8 greatest: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%14));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%4), read<i32>(%5))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%3), read<i32>(%5)))));
// DEFAULT-NEXT:                     if gt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%3), read<i32>(%5)))), read<i32>(%8))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%7, read<i32>(%5));
// DEFAULT-NEXT:                             write<i32>(%8, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%3), read<i32>(%5)))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(2));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%16));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if lt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%3), read<i32>(%5)))), read<i32>(%8))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%4), read<i32>(%7))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%3), read<i32>(%5)))));
// DEFAULT-NEXT:                             write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:                             write<i32>(%8, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%4), const<i32>(0)))));
// DEFAULT-NEXT:                             for %12
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%6, const<i32>(1));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%6), const<i32>(2))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %17: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                                     let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%6, read<i32>(%18));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if gt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%4), read<i32>(%6)))), read<i32>(%8))
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             write<i32>(%7, read<i32>(%6));
// DEFAULT-NEXT:                                             write<i32>(%8, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%4), read<i32>(%6)))));
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%4), const<i32>(0)))), const<i32>(5)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%4), const<i32>(1)))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
