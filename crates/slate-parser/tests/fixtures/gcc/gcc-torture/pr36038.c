/* PR tree-optimization/36038 */

long long  list[10];
long long  expect[10] = {0, 1, 2, 3, 4, 4, 5, 6, 7, 9};
long long *stack_base;
int        indices[10];
int       *markstack_ptr;

void doit(void) {
  long long *src;
  long long *dst;
  long long *sp   = stack_base + 5;
  int        diff = 2;
  int        shift;
  int        count;

  shift = diff - (markstack_ptr[-1] - markstack_ptr[-2]);
  count = (sp - stack_base) - markstack_ptr[-1] + 2;
  src   = sp;
  dst   = (sp += shift);
  while (--count)
    *dst-- = *src--;
}

int main() {
  int i;
  for (i = 0; i < 10; i++)
    list[i] = i;

  markstack_ptr     = indices + 9;
  markstack_ptr[-1] = 2;
  markstack_ptr[-2] = 1;

  stack_base = list + 2;
  doit();
  if (__builtin_memcmp(expect, list, sizeof(list)))
    __builtin_abort();
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
// DEFAULT-NEXT:     global %0 list: array<i64, 10> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %1 expect: array<i64, 10> [storage=static] [align=16] = aggregate<array<i64, 10>, zero_fill=false>(index0 = widen<i64, reason=assign>(const<i32>(0)), index1 = widen<i64, reason=assign>(const<i32>(1)), index2 = widen<i64, reason=assign>(const<i32>(2)), index3 = widen<i64, reason=assign>(const<i32>(3)), index4 = widen<i64, reason=assign>(const<i32>(4)), index5 = widen<i64, reason=assign>(const<i32>(4)), index6 = widen<i64, reason=assign>(const<i32>(5)), index7 = widen<i64, reason=assign>(const<i32>(6)), index8 = widen<i64, reason=assign>(const<i32>(7)), index9 = widen<i64, reason=assign>(const<i32>(9))) [linkage=external];
// DEFAULT-NEXT:     global %2 stack_base: ptr<i64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 indices: array<i32, 10> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %4 markstack_ptr: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %5 @doit() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 src: ptr<i64> [storage=automatic];
// DEFAULT-NEXT:         let %7 dst: ptr<i64> [storage=automatic];
// DEFAULT-NEXT:         let %8 sp: ptr<i64> [storage=automatic] = ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%2), const<i32>(5));
// DEFAULT-NEXT:         let %9 diff: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %10 shift: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 count: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%10, sub<i32, overflow=ub>(read<i32>(%9), sub<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%4), neg<i32, overflow=ub>(const<i32>(1))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%4), neg<i32, overflow=ub>(const<i32>(2))))))));
// DEFAULT-NEXT:         write<i32>(%11, truncate<i32, reason=assign, fits=unknown>(add<i64, overflow=ub>(sub<i64, overflow=ub>(ptr_diff<i64, element=i64, same_array=required, overflow=ub>(read<ptr<i64>>(%8), read<ptr<i64>>(%2)), widen<i64, reason=usual_arith>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%4), neg<i32, overflow=ub>(const<i32>(1))))))), widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:         write<ptr<i64>>(%6, read<ptr<i64>>(%8));
// DEFAULT-NEXT:         let %16: ptr<i64> [synthetic] = read<ptr<i64>>(%8);
// DEFAULT-NEXT:         let %17: ptr<i64> [synthetic] = ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%16), read<i32>(%10));
// DEFAULT-NEXT:         write<ptr<i64>>(%8, read<ptr<i64>>(%17));
// DEFAULT-NEXT:         write<ptr<i64>>(%7, read<ptr<i64>>(%17));
// DEFAULT-NEXT:         while %14 {
// DEFAULT-NEXT:             let %18: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:             let %19: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%11, read<i32>(%19));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%19), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             let %20: ptr<i64> [synthetic] = read<ptr<i64>>(%6);
// DEFAULT-NEXT:             let %21: ptr<i64> [synthetic] = ptr_offset<ptr<i64>, subtract=true, element=i64, overflow=ub>(read<ptr<i64>>(%20), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i64>>(%6, read<ptr<i64>>(%21));
// DEFAULT-NEXT:             let %22: ptr<i64> [synthetic] = read<ptr<i64>>(%7);
// DEFAULT-NEXT:             let %23: ptr<i64> [synthetic] = ptr_offset<ptr<i64>, subtract=true, element=i64, overflow=ub>(read<ptr<i64>>(%22), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i64>>(%7, read<ptr<i64>>(%23));
// DEFAULT-NEXT:             write<i64>(deref(read<ptr<i64>>(%22)), read<i64>(deref(read<ptr<i64>>(%20))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%13, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%13), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%25));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(10)>(%0), read<i32>(%13))), widen<i64, reason=assign>(read<i32>(%13)));
// DEFAULT-NEXT:         write<ptr<i32>>(%4, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%3), const<i32>(9)));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%4), neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%4), neg<i32, overflow=ub>(const<i32>(2)))), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i64>>(%2, ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(10)>(%0), const<i32>(2)));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i64>, length=Some(10)>(%1)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i64>, length=Some(10)>(%0)), const<u64>(80)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
