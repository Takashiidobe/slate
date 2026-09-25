extern void abort(void);

void __attribute__((noinline, noreturn)) vec_assert_fail(void) { abort(); }

struct ggc_root_tab {
  void *base;
};

typedef struct deferred_access_check {
} VEC_deferred_access_check_gc;

typedef struct deferred_access {
  VEC_deferred_access_check_gc *deferred_access_checks;
  int                           deferring_access_checks_kind;
} deferred_access;

typedef struct VEC_deferred_access_base {
  unsigned        num;
  deferred_access vec[1];
} VEC_deferred_access_base;

static __inline__ deferred_access *
VEC_deferred_access_base_last(VEC_deferred_access_base *vec_) {
  (void)((vec_ && vec_->num) ? 0 : (vec_assert_fail(), 0));
  return &vec_->vec[vec_->num - 1];
}

static __inline__ void
VEC_deferred_access_base_pop(VEC_deferred_access_base *vec_) {
  (void)((vec_->num) ? 0 : (vec_assert_fail(), 0));
  --vec_->num;
}

void __attribute__((noinline))
perform_access_checks(VEC_deferred_access_check_gc *p) {
  abort();
}

typedef struct VEC_deferred_access_gc {
  VEC_deferred_access_base base;
} VEC_deferred_access_gc;

static VEC_deferred_access_gc *deferred_access_stack;
static unsigned                deferred_access_no_check;

const struct ggc_root_tab gt_pch_rs_gt_cp_semantics_h[] = {
    {&deferred_access_no_check}};

void __attribute__((noinline)) pop_to_parent_deferring_access_checks(void) {
  if (deferred_access_no_check)
    deferred_access_no_check--;
  else {
    VEC_deferred_access_check_gc *checks;
    deferred_access              *ptr;
    checks = (VEC_deferred_access_base_last(
                  deferred_access_stack ? &deferred_access_stack->base : 0))
                 ->deferred_access_checks;
    VEC_deferred_access_base_pop(
        deferred_access_stack ? &deferred_access_stack->base : 0);
    ptr = VEC_deferred_access_base_last(
        deferred_access_stack ? &deferred_access_stack->base : 0);
    if (ptr->deferring_access_checks_kind == 0)
      perform_access_checks(checks);
  }
}

int main() {
  deferred_access_stack = __builtin_malloc(sizeof(VEC_deferred_access_gc) +
                                           sizeof(deferred_access) * 8);
  deferred_access_stack->base.num                                 = 2;
  deferred_access_stack->base.vec[0].deferring_access_checks_kind = 1;
  pop_to_parent_deferring_access_checks();
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
// DEFAULT-NEXT:     type @type0 ggc_root_tab = struct {
// DEFAULT-NEXT:         field0 base: ptr<void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 deferred_access_check = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type2 VEC_deferred_access_check_gc = @type1;
// DEFAULT-NEXT:     type @type3 deferred_access = struct {
// DEFAULT-NEXT:         field0 deferred_access_checks: ptr<@type1>;
// DEFAULT-NEXT:         field1 deferring_access_checks_kind: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type4 deferred_access = @type3;
// DEFAULT-NEXT:     type @type5 VEC_deferred_access_base = struct {
// DEFAULT-NEXT:         field0 num: u32;
// DEFAULT-NEXT:         field1 vec: array<@type3, 1>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type6 VEC_deferred_access_base = @type5;
// DEFAULT-NEXT:     type @type7 VEC_deferred_access_gc = struct {
// DEFAULT-NEXT:         field0 base: @type5;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type8 VEC_deferred_access_gc = @type7;
// DEFAULT-NEXT:     global %17 deferred_access_stack: ptr<@type7> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %18 deferred_access_no_check: u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %19 gt_pch_rs_gt_cp_semantics_h: array<@type0, 1> [storage=static] [const] = aggregate<array<@type0, 1>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<u32>>(%18)))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @vec_assert_fail() -> void [linkage=external] [inline=never] [definition=emitted] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @VEC_deferred_access_base_last(%10 vec_: ptr<@type5>) -> ptr<@type3> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %24: i32 [synthetic];
// DEFAULT-NEXT:         if logical_and<bool>(ne<ptr<@type5>>(read<ptr<@type5>>(%10), null<ptr<@type5>>), ne<u32>(read<u32>(field0(deref(read<ptr<@type5>>(%10)))), const<u32>(0)))
// DEFAULT-NEXT:             write<i32>(%24, const<i32>(0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:             write<i32>(%24, const<i32>(0));
// DEFAULT-NEXT:         return addr_of<ptr<@type3>>(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<@type3>, length=Some(1)>(field1(deref(read<ptr<@type5>>(%10)))), sub<u32, overflow=wrap>(read<u32>(field0(deref(read<ptr<@type5>>(%10)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @VEC_deferred_access_base_pop(%12 vec_: ptr<@type5>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %25: i32 [synthetic];
// DEFAULT-NEXT:         if ne<u32>(read<u32>(field0(deref(read<ptr<@type5>>(%12)))), const<u32>(0))
// DEFAULT-NEXT:             write<i32>(%25, const<i32>(0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:             write<i32>(%25, const<i32>(0));
// DEFAULT-NEXT:         let %26: ptr<@type5> [synthetic] = read<ptr<@type5>>(%12);
// DEFAULT-NEXT:         let %27: u32 [synthetic] = read<u32>(field0(deref(read<ptr<@type5>>(%26))));
// DEFAULT-NEXT:         let %28: u32 [synthetic] = sub<u32, overflow=wrap>(read<u32>(%27), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(field0(deref(read<ptr<@type5>>(%26))), read<u32>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @perform_access_checks(%14 p: ptr<@type1>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @pop_to_parent_deferring_access_checks() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%18), const<u32>(0))
// DEFAULT-NEXT:             let %29: u32 [synthetic] = read<u32>(%18);
// DEFAULT-NEXT:             let %30: u32 [synthetic] = sub<u32, overflow=wrap>(read<u32>(%29), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<u32>(%18, read<u32>(%30));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %21 checks: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:                 let %22 ptr: ptr<@type3> [storage=automatic];
// DEFAULT-NEXT:                 write<ptr<@type1>>(%21, read<ptr<@type1>>(field0(deref(call<ptr<@type3>, signature=fn(ptr<@type5>) -> ptr<@type3>>(%9, conditional<ptr<@type5>>(ne<ptr<@type7>>(read<ptr<@type7>>(%17), null<ptr<@type7>>), addr_of<ptr<@type5>>(field0(deref(read<ptr<@type7>>(%17)))), null<ptr<@type5>>))))));
// DEFAULT-NEXT:                 read<ptr<@type1>>(field0(deref(call<ptr<@type3>, signature=fn(ptr<@type5>) -> ptr<@type3>>(%9, conditional<ptr<@type5>>(ne<ptr<@type7>>(read<ptr<@type7>>(%17), null<ptr<@type7>>), addr_of<ptr<@type5>>(field0(deref(read<ptr<@type7>>(%17)))), null<ptr<@type5>>)))));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type5>) -> void>(%11, conditional<ptr<@type5>>(ne<ptr<@type7>>(read<ptr<@type7>>(%17), null<ptr<@type7>>), addr_of<ptr<@type5>>(field0(deref(read<ptr<@type7>>(%17)))), null<ptr<@type5>>));
// DEFAULT-NEXT:                 write<ptr<@type3>>(%22, call<ptr<@type3>, signature=fn(ptr<@type5>) -> ptr<@type3>>(%9, conditional<ptr<@type5>>(ne<ptr<@type7>>(read<ptr<@type7>>(%17), null<ptr<@type7>>), addr_of<ptr<@type5>>(field0(deref(read<ptr<@type7>>(%17)))), null<ptr<@type5>>)));
// DEFAULT-NEXT:                 call<ptr<@type3>, signature=fn(ptr<@type5>) -> ptr<@type3>>(%9, conditional<ptr<@type5>>(ne<ptr<@type7>>(read<ptr<@type7>>(%17), null<ptr<@type7>>), addr_of<ptr<@type5>>(field0(deref(read<ptr<@type7>>(%17)))), null<ptr<@type5>>));
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(field1(deref(read<ptr<@type3>>(%22)))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type1>) -> void>(%13, read<ptr<@type1>>(%21));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<@type7>>(%17, pointer_cast<ptr<@type7>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(__builtin_malloc, add<u64, overflow=wrap>(const<u64>(24), mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))))));
// DEFAULT-NEXT:         pointer_cast<ptr<@type7>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(__builtin_malloc, add<u64, overflow=wrap>(const<u64>(24), mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))));
// DEFAULT-NEXT:         write<u32>(field0(field0(deref(read<ptr<@type7>>(%17)))), reinterpret<u32, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<i32>(field1(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<@type3>, length=Some(1)>(field1(field0(deref(read<ptr<@type7>>(%17))))), const<i32>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%20);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
