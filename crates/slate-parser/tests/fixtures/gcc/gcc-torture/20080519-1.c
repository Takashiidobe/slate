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
// DEFAULT-NEXT:     type @type0 HARD_REG_SET = array<u64, 2>;
// DEFAULT-NEXT:     type @type1 du_chain = struct {
// DEFAULT-NEXT:         field0 next_use: ptr<@type1>;
// DEFAULT-NEXT:         field1 cl: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %2 reg_class_contents: array<array<u64, 2>, 2> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @merge_overlapping_regs(%5 p: ptr<array<u64, 2>>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(deref(read<ptr<array<u64, 2>>>(%5))), const<i32>(0)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))), ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(deref(read<ptr<array<u64, 2>>>(%5))), const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @regrename_optimize(%7 this: ptr<@type1>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 this_unavailable: array<u64, 2> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %9 scan_fp_: ptr<u64> [storage=automatic];
// DEFAULT-NEXT:         let %10 n_uses: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 last: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%8), const<i32>(0))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%8), const<i32>(1))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<ptr<@type1>>(%11, read<ptr<@type1>>(%7));
// DEFAULT-NEXT:             condition: ne<ptr<@type1>>(read<ptr<@type1>>(field0(deref(read<ptr<@type1>>(%11)))), null<ptr<@type1>>)
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<ptr<@type1>>(%11, read<ptr<@type1>>(field0(deref(read<ptr<@type1>>(%11)))));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<u64>>(%9, array_decay<ptr<u64>, length=Some(2)>(deref(ptr_offset<ptr<array<u64, 2>>, subtract=false, element=array<u64, 2>, overflow=ub>(array_decay<ptr<array<u64, 2>>, length=Some(2)>(%2), read<i32>(field1(deref(read<ptr<@type1>>(%11))))))));
// DEFAULT-NEXT:                     let %16: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                     let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%10, read<i32>(%17));
// DEFAULT-NEXT:                     let %18: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%8), const<i32>(0));
// DEFAULT-NEXT:                     let %19: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%18)));
// DEFAULT-NEXT:                     let %20: u64 [synthetic] = or<u64>(read<u64>(%19), not<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%9), const<i32>(0))))));
// DEFAULT-NEXT:                     write<u64>(deref(read<ptr<u64>>(%18)), read<u64>(%20));
// DEFAULT-NEXT:                     let %21: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%8), const<i32>(1));
// DEFAULT-NEXT:                     let %22: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%21)));
// DEFAULT-NEXT:                     let %23: u64 [synthetic] = or<u64>(read<u64>(%22), not<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%9), const<i32>(1))))));
// DEFAULT-NEXT:                     write<u64>(deref(read<ptr<u64>>(%21)), read<u64>(%23));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%10), const<i32>(1))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         write<ptr<u64>>(%9, array_decay<ptr<u64>, length=Some(2)>(deref(ptr_offset<ptr<array<u64, 2>>, subtract=false, element=array<u64, 2>, overflow=ub>(array_decay<ptr<array<u64, 2>>, length=Some(2)>(%2), read<i32>(field1(deref(read<ptr<@type1>>(%11))))))));
// DEFAULT-NEXT:         let %24: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%8), const<i32>(0));
// DEFAULT-NEXT:         let %25: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%24)));
// DEFAULT-NEXT:         let %26: u64 [synthetic] = or<u64>(read<u64>(%25), not<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%9), const<i32>(0))))));
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%24)), read<u64>(%26));
// DEFAULT-NEXT:         let %27: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%8), const<i32>(1));
// DEFAULT-NEXT:         let %28: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%27)));
// DEFAULT-NEXT:         let %29: u64 [synthetic] = or<u64>(read<u64>(%28), not<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%9), const<i32>(1))))));
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%27)), read<u64>(%29));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<array<u64, 2>>) -> void>(%4, addr_of<ptr<array<u64, 2>>>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 du1: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = null<ptr<@type1>>, field1 = const<i32>(0));
// DEFAULT-NEXT:         let %14 du0: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = addr_of<ptr<@type1>>(%13), field1 = const<i32>(1));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(deref(ptr_offset<ptr<array<u64, 2>>, subtract=false, element=array<u64, 2>, overflow=ub>(array_decay<ptr<array<u64, 2>>, length=Some(2)>(%2), const<i32>(0)))), const<i32>(0))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(deref(ptr_offset<ptr<array<u64, 2>>, subtract=false, element=array<u64, 2>, overflow=ub>(array_decay<ptr<array<u64, 2>>, length=Some(2)>(%2), const<i32>(0)))), const<i32>(1))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(deref(ptr_offset<ptr<array<u64, 2>>, subtract=false, element=array<u64, 2>, overflow=ub>(array_decay<ptr<array<u64, 2>>, length=Some(2)>(%2), const<i32>(1)))), const<i32>(0))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(deref(ptr_offset<ptr<array<u64, 2>>, subtract=false, element=array<u64, 2>, overflow=ub>(array_decay<ptr<array<u64, 2>>, length=Some(2)>(%2), const<i32>(1)))), const<i32>(1))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>) -> void>(%6, addr_of<ptr<@type1>>(%14));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
