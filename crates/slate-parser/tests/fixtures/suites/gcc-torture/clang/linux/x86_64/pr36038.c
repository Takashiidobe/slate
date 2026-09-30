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
// DEFAULT-NEXT:     global %[[VALUE_list:[0-9]+]] list: array<i64, 10> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_expect:[0-9]+]] expect: array<i64, 10> [storage=static] [align=16] = aggregate<array<i64, 10>, zero_fill=false>(index0 = widen<i64, reason=assign>(const<i32>(0)), index1 = widen<i64, reason=assign>(const<i32>(1)), index2 = widen<i64, reason=assign>(const<i32>(2)), index3 = widen<i64, reason=assign>(const<i32>(3)), index4 = widen<i64, reason=assign>(const<i32>(4)), index5 = widen<i64, reason=assign>(const<i32>(4)), index6 = widen<i64, reason=assign>(const<i32>(5)), index7 = widen<i64, reason=assign>(const<i32>(6)), index8 = widen<i64, reason=assign>(const<i32>(7)), index9 = widen<i64, reason=assign>(const<i32>(9))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_stack_base:[0-9]+]] stack_base: ptr<i64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_indices:[0-9]+]] indices: array<i32, 10> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_markstack_ptr:[0-9]+]] markstack_ptr: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_doit:[0-9]+]] @doit() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_src:[0-9]+]] src: ptr<i64> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_dst:[0-9]+]] dst: ptr<i64> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_sp:[0-9]+]] sp: ptr<i64> [storage=automatic] = ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%[[VALUE_stack_base]]), const<i32>(5));
// DEFAULT-NEXT:         let %[[VALUE_diff:[0-9]+]] diff: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %[[VALUE_shift:[0-9]+]] shift: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_count:[0-9]+]] count: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_shift]], sub<i32, overflow=ub>(read<i32>(%[[VALUE_diff]]), sub<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_markstack_ptr]]), neg<i32, overflow=ub>(const<i32>(1))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_markstack_ptr]]), neg<i32, overflow=ub>(const<i32>(2))))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], truncate<i32, reason=assign, fits=unknown>(add<i64, overflow=ub>(sub<i64, overflow=ub>(ptr_diff<i64, element=i64, same_array=required, overflow=ub>(read<ptr<i64>>(%[[VALUE_sp]]), read<ptr<i64>>(%[[VALUE_stack_base]])), widen<i64, reason=usual_arith>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_markstack_ptr]]), neg<i32, overflow=ub>(const<i32>(1))))))), widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:         write<ptr<i64>>(%[[VALUE_src]], read<ptr<i64>>(%[[VALUE_sp]]));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<i64> [synthetic] = read<ptr<i64>>(%[[VALUE_sp]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i64> [synthetic] = ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%[[VALUE0]]), read<i32>(%[[VALUE_shift]]));
// DEFAULT-NEXT:         write<ptr<i64>>(%[[VALUE_sp]], read<ptr<i64>>(%[[VALUE1]]));
// DEFAULT-NEXT:         write<ptr<i64>>(%[[VALUE_dst]], read<ptr<i64>>(%[[VALUE1]]));
// DEFAULT-NEXT:         while %[[VALUE2:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE4]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: ptr<i64> [synthetic] = read<ptr<i64>>(%[[VALUE_src]]);
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: ptr<i64> [synthetic] = ptr_offset<ptr<i64>, subtract=true, element=i64, overflow=ub>(read<ptr<i64>>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i64>>(%[[VALUE_src]], read<ptr<i64>>(%[[VALUE6]]));
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: ptr<i64> [synthetic] = read<ptr<i64>>(%[[VALUE_dst]]);
// DEFAULT-NEXT:             let %[[VALUE8:[0-9]+]]: ptr<i64> [synthetic] = ptr_offset<ptr<i64>, subtract=true, element=i64, overflow=ub>(read<ptr<i64>>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i64>>(%[[VALUE_dst]], read<ptr<i64>>(%[[VALUE8]]));
// DEFAULT-NEXT:             write<i64>(deref(read<ptr<i64>>(%[[VALUE7]])), read<i64>(deref(read<ptr<i64>>(%[[VALUE5]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcmp:[0-9]+]] @__builtin_memcmp(%[[VALUE9:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE10:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE11:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(10)>(%[[VALUE_list]]), read<i32>(%[[VALUE_i]]))), widen<i64, reason=assign>(read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_markstack_ptr]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_indices]]), const<i32>(9)));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_markstack_ptr]]), neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_markstack_ptr]]), neg<i32, overflow=ub>(const<i32>(2)))), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i64>>(%[[VALUE_stack_base]], ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(10)>(%[[VALUE_list]]), const<i32>(2)));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_doit]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i64>, length=Some(10)>(%[[VALUE_expect]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i64>, length=Some(10)>(%[[VALUE_list]])), const<u64>(80)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
