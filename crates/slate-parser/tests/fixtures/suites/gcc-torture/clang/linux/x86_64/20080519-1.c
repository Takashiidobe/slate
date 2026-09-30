extern void abort(void);

typedef unsigned long HARD_REG_SET[2];
HARD_REG_SET          reg_class_contents[2];

struct du_chain {
  struct du_chain *next_use;
  int              cl;
};

void __attribute__((noinline)) merge_overlapping_regs(HARD_REG_SET *p) {
  if ((*p)[0] != -1 || (*p)[1] != -1)
    abort();
}

void __attribute__((noinline)) regrename_optimize(struct du_chain *this) {
  HARD_REG_SET     this_unavailable;
  unsigned long   *scan_fp_;
  int              n_uses;
  struct du_chain *last;

  this_unavailable[0] = 0;
  this_unavailable[1] = 0;

  n_uses = 0;
  for (last = this; last->next_use; last = last->next_use) {
    scan_fp_ = reg_class_contents[last->cl];
    n_uses++;
    this_unavailable[0] |= ~scan_fp_[0];
    this_unavailable[1] |= ~scan_fp_[1];
  }
  if (n_uses < 1)
    return;

  scan_fp_             = reg_class_contents[last->cl];
  this_unavailable[0] |= ~scan_fp_[0];
  this_unavailable[1] |= ~scan_fp_[1];

  merge_overlapping_regs(&this_unavailable);
}

int main() {
  struct du_chain du1      = {0, 0};
  struct du_chain du0      = {&du1, 1};
  reg_class_contents[0][0] = -1;
  reg_class_contents[0][1] = -1;
  reg_class_contents[1][0] = 0;
  reg_class_contents[1][1] = 0;
  regrename_optimize(&du0);
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
// DEFAULT-NEXT:     type @type[[TYPE_HARD_REG_SET:[0-9]+]] HARD_REG_SET = array<u64, 2>;
// DEFAULT-NEXT:     type @type[[TYPE_du_chain:[0-9]+]] du_chain = struct {
// DEFAULT-NEXT:         field0 next_use: ptr<@type[[TYPE_du_chain]]>;
// DEFAULT-NEXT:         field1 cl: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_reg_class_contents:[0-9]+]] reg_class_contents: array<array<u64, 2>, 2> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_merge_overlapping_regs:[0-9]+]] @merge_overlapping_regs(%[[VALUE_p:[0-9]+]] p: ptr<array<u64, 2>>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(deref(read<ptr<array<u64, 2>>>(%[[VALUE_p]]))), const<i32>(0)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(deref(read<ptr<array<u64, 2>>>(%[[VALUE_p]]))), const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_regrename_optimize:[0-9]+]] @regrename_optimize(%[[VALUE_this:[0-9]+]] this: ptr<@type[[TYPE_du_chain]]>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_this_unavailable:[0-9]+]] this_unavailable: array<u64, 2> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_scan_fp_:[0-9]+]] scan_fp_: ptr<u64> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n_uses:[0-9]+]] n_uses: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_last:[0-9]+]] last: ptr<@type[[TYPE_du_chain]]> [storage=automatic];
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%[[VALUE_this_unavailable]]), const<i32>(0))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%[[VALUE_this_unavailable]]), const<i32>(1))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_n_uses]], const<i32>(0));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_du_chain]]>>(%[[VALUE_last]], read<ptr<@type[[TYPE_du_chain]]>>(%[[VALUE_this]]));
// DEFAULT-NEXT:             condition: ne<ptr<@type[[TYPE_du_chain]]>>(read<ptr<@type[[TYPE_du_chain]]>>(field0(deref(read<ptr<@type[[TYPE_du_chain]]>>(%[[VALUE_last]])))), null<ptr<@type[[TYPE_du_chain]]>>)
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_du_chain]]>>(%[[VALUE_last]], read<ptr<@type[[TYPE_du_chain]]>>(field0(deref(read<ptr<@type[[TYPE_du_chain]]>>(%[[VALUE_last]])))));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<u64>>(%[[VALUE_scan_fp_]], array_decay<ptr<u64>, length=Some(2)>(deref(ptr_offset<ptr<array<u64, 2>>, subtract=false, element=array<u64, 2>, overflow=ub>(array_decay<ptr<array<u64, 2>>, length=Some(2)>(%[[VALUE_reg_class_contents]]), read<i32>(field1(deref(read<ptr<@type[[TYPE_du_chain]]>>(%[[VALUE_last]]))))))));
// DEFAULT-NEXT:                     let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_uses]]);
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_n_uses]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%[[VALUE_this_unavailable]]), const<i32>(0));
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%[[VALUE3]])));
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: u64 [synthetic] = or<u64>(read<u64>(%[[VALUE4]]), not<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_scan_fp_]]), const<i32>(0))))));
// DEFAULT-NEXT:                     write<u64>(deref(read<ptr<u64>>(%[[VALUE3]])), read<u64>(%[[VALUE5]]));
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%[[VALUE_this_unavailable]]), const<i32>(1));
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%[[VALUE6]])));
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: u64 [synthetic] = or<u64>(read<u64>(%[[VALUE7]]), not<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_scan_fp_]]), const<i32>(1))))));
// DEFAULT-NEXT:                     write<u64>(deref(read<ptr<u64>>(%[[VALUE6]])), read<u64>(%[[VALUE8]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_n_uses]]), const<i32>(1))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         write<ptr<u64>>(%[[VALUE_scan_fp_]], array_decay<ptr<u64>, length=Some(2)>(deref(ptr_offset<ptr<array<u64, 2>>, subtract=false, element=array<u64, 2>, overflow=ub>(array_decay<ptr<array<u64, 2>>, length=Some(2)>(%[[VALUE_reg_class_contents]]), read<i32>(field1(deref(read<ptr<@type[[TYPE_du_chain]]>>(%[[VALUE_last]]))))))));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%[[VALUE_this_unavailable]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%[[VALUE9]])));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: u64 [synthetic] = or<u64>(read<u64>(%[[VALUE10]]), not<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_scan_fp_]]), const<i32>(0))))));
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%[[VALUE9]])), read<u64>(%[[VALUE11]]));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%[[VALUE_this_unavailable]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%[[VALUE12]])));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: u64 [synthetic] = or<u64>(read<u64>(%[[VALUE13]]), not<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_scan_fp_]]), const<i32>(1))))));
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%[[VALUE12]])), read<u64>(%[[VALUE14]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<array<u64, 2>>) -> void>(%[[VALUE_merge_overlapping_regs]], addr_of<ptr<array<u64, 2>>>(%[[VALUE_this_unavailable]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_du1:[0-9]+]] du1: @type[[TYPE_du_chain]] [storage=automatic] = aggregate<@type[[TYPE_du_chain]], zero_fill=false>(field0 = null<ptr<@type[[TYPE_du_chain]]>>, field1 = const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_du0:[0-9]+]] du0: @type[[TYPE_du_chain]] [storage=automatic] = aggregate<@type[[TYPE_du_chain]], zero_fill=false>(field0 = addr_of<ptr<@type[[TYPE_du_chain]]>>(%[[VALUE_du1]]), field1 = const<i32>(1));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(deref(ptr_offset<ptr<array<u64, 2>>, subtract=false, element=array<u64, 2>, overflow=ub>(array_decay<ptr<array<u64, 2>>, length=Some(2)>(%[[VALUE_reg_class_contents]]), const<i32>(0)))), const<i32>(0))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(deref(ptr_offset<ptr<array<u64, 2>>, subtract=false, element=array<u64, 2>, overflow=ub>(array_decay<ptr<array<u64, 2>>, length=Some(2)>(%[[VALUE_reg_class_contents]]), const<i32>(0)))), const<i32>(1))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(deref(ptr_offset<ptr<array<u64, 2>>, subtract=false, element=array<u64, 2>, overflow=ub>(array_decay<ptr<array<u64, 2>>, length=Some(2)>(%[[VALUE_reg_class_contents]]), const<i32>(1)))), const<i32>(0))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(deref(ptr_offset<ptr<array<u64, 2>>, subtract=false, element=array<u64, 2>, overflow=ub>(array_decay<ptr<array<u64, 2>>, length=Some(2)>(%[[VALUE_reg_class_contents]]), const<i32>(1)))), const<i32>(1))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_du_chain]]>) -> void>(%[[VALUE_regrename_optimize]], addr_of<ptr<@type[[TYPE_du_chain]]>>(%[[VALUE_du0]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
