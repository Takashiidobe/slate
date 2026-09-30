/* Bombed with a segfault on powerpc-linux.  doloop.c generated wrong
   loop count.  */
void abort(void);

void foo(unsigned long *start, unsigned long *end) {
  unsigned long *temp = end - 1;

  while (end > start)
    *end-- = *temp--;
}

int main(void) {
  unsigned long a[5];
  int           start, end, k;

  for (start = 0; start < 5; start++)
    for (end = 0; end < 5; end++) {
      for (k = 0; k < 5; k++)
        a[k] = k;

      foo(a + start, a + end);

      for (k = 0; k <= start; k++)
        if (a[k] != k)
          abort();

      for (k = start + 1; k <= end; k++)
        if (a[k] != k - 1)
          abort();

      for (k = end + 1; k < 5; k++)
        if (a[k] != k)
          abort();
    }

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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_start:[0-9]+]] start: ptr<u64>, %[[VALUE_end:[0-9]+]] end: ptr<u64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_temp:[0-9]+]] temp: ptr<u64> [storage=automatic] = ptr_offset<ptr<u64>, subtract=true, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_end]]), const<i32>(1));
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] gt<ptr<u64>>(read<ptr<u64>>(%[[VALUE_end]]), read<ptr<u64>>(%[[VALUE_start]]))
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: ptr<u64> [synthetic] = read<ptr<u64>>(%[[VALUE_temp]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=true, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<u64>>(%[[VALUE_temp]], read<ptr<u64>>(%[[VALUE2]]));
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: ptr<u64> [synthetic] = read<ptr<u64>>(%[[VALUE_end]]);
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=true, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<u64>>(%[[VALUE_end]], read<ptr<u64>>(%[[VALUE4]]));
// DEFAULT-NEXT:             write<u64>(deref(read<ptr<u64>>(%[[VALUE3]])), read<u64>(deref(read<ptr<u64>>(%[[VALUE1]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<u64, 5> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_start_2:[0-9]+]] start: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_end_2:[0-9]+]] end: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_start_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_start_2]]), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_start_2]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_start_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_end_2]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_end_2]]), const<i32>(5))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_end_2]]);
// DEFAULT-NEXT:                         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_end_2]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             for %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_k]], const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%[[VALUE_k]]), const<i32>(5))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// DEFAULT-NEXT:                                     let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(5)>(%[[VALUE_a]]), read<i32>(%[[VALUE_k]]))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_k]]))));
// DEFAULT-NEXT:                             call<void, signature=fn(ptr<u64>, ptr<u64>) -> void>(%[[VALUE_foo]], ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(5)>(%[[VALUE_a]]), read<i32>(%[[VALUE_start_2]])), ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(5)>(%[[VALUE_a]]), read<i32>(%[[VALUE_end_2]])));
// DEFAULT-NEXT:                             for %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_k]], const<i32>(0));
// DEFAULT-NEXT:                                 condition: le<i32>(read<i32>(%[[VALUE_k]]), read<i32>(%[[VALUE_start_2]]))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// DEFAULT-NEXT:                                     let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE15]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(5)>(%[[VALUE_a]]), read<i32>(%[[VALUE_k]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_k]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             for %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_k]], add<i32, overflow=ub>(read<i32>(%[[VALUE_start_2]]), const<i32>(1)));
// DEFAULT-NEXT:                                 condition: le<i32>(read<i32>(%[[VALUE_k]]), read<i32>(%[[VALUE_end_2]]))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// DEFAULT-NEXT:                                     let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(5)>(%[[VALUE_a]]), read<i32>(%[[VALUE_k]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_k]]), const<i32>(1)))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             for %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_k]], add<i32, overflow=ub>(read<i32>(%[[VALUE_end_2]]), const<i32>(1)));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%[[VALUE_k]]), const<i32>(5))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// DEFAULT-NEXT:                                     let %[[VALUE22:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE21]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE22]]));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(5)>(%[[VALUE_a]]), read<i32>(%[[VALUE_k]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_k]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
