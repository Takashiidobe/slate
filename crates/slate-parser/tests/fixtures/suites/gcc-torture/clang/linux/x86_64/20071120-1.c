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
// DEFAULT-NEXT:     type @type[[TYPE_ggc_root_tab:[0-9]+]] ggc_root_tab = struct {
// DEFAULT-NEXT:         field0 base: ptr<void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_deferred_access_check:[0-9]+]] deferred_access_check = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_VEC_deferred_access_check_gc:[0-9]+]] VEC_deferred_access_check_gc = @type[[TYPE_deferred_access_check]];
// DEFAULT-NEXT:     type @type[[TYPE_deferred_access:[0-9]+]] deferred_access = struct {
// DEFAULT-NEXT:         field0 deferred_access_checks: ptr<@type[[TYPE_deferred_access_check]]>;
// DEFAULT-NEXT:         field1 deferring_access_checks_kind: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_deferred_access_2:[0-9]+]] deferred_access = @type[[TYPE_deferred_access]];
// DEFAULT-NEXT:     type @type[[TYPE_VEC_deferred_access_base:[0-9]+]] VEC_deferred_access_base = struct {
// DEFAULT-NEXT:         field0 num: u32;
// DEFAULT-NEXT:         field1 vec: array<@type[[TYPE_deferred_access]], 1>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_VEC_deferred_access_base_2:[0-9]+]] VEC_deferred_access_base = @type[[TYPE_VEC_deferred_access_base]];
// DEFAULT-NEXT:     type @type[[TYPE_VEC_deferred_access_gc:[0-9]+]] VEC_deferred_access_gc = struct {
// DEFAULT-NEXT:         field0 base: @type[[TYPE_VEC_deferred_access_base]];
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_VEC_deferred_access_gc_2:[0-9]+]] VEC_deferred_access_gc = @type[[TYPE_VEC_deferred_access_gc]];
// DEFAULT-NEXT:     global %[[VALUE_deferred_access_stack:[0-9]+]] deferred_access_stack: ptr<@type[[TYPE_VEC_deferred_access_gc]]> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_deferred_access_no_check:[0-9]+]] deferred_access_no_check: u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_gt_pch_rs_gt_cp_semantics_h:[0-9]+]] gt_pch_rs_gt_cp_semantics_h: array<@type[[TYPE_ggc_root_tab]], 1> [storage=static] [const] = aggregate<array<@type[[TYPE_ggc_root_tab]], 1>, zero_fill=false>(index0 = aggregate<@type[[TYPE_ggc_root_tab]], zero_fill=false>(field0 = pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<u32>>(%[[VALUE_deferred_access_no_check]])))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_vec_assert_fail:[0-9]+]] @vec_assert_fail() -> void [linkage=external] [inline=never] [definition=emitted] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_VEC_deferred_access_base_last:[0-9]+]] @VEC_deferred_access_base_last(%[[VALUE_vec_:[0-9]+]] vec_: ptr<@type[[TYPE_VEC_deferred_access_base]]>) -> ptr<@type[[TYPE_deferred_access]]> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if logical_and<bool>(ne<ptr<@type[[TYPE_VEC_deferred_access_base]]>>(read<ptr<@type[[TYPE_VEC_deferred_access_base]]>>(%[[VALUE_vec_]]), null<ptr<@type[[TYPE_VEC_deferred_access_base]]>>), ne<u32>(read<u32>(field0(deref(read<ptr<@type[[TYPE_VEC_deferred_access_base]]>>(%[[VALUE_vec_]])))), const<u32>(0)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE0]], const<i32>(0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_vec_assert_fail]]);
// DEFAULT-NEXT:             write<i32>(%[[VALUE0]], const<i32>(0));
// DEFAULT-NEXT:         return addr_of<ptr<@type[[TYPE_deferred_access]]>>(deref(ptr_offset<ptr<@type[[TYPE_deferred_access]]>, subtract=false, element=@type[[TYPE_deferred_access]], overflow=ub>(array_decay<ptr<@type[[TYPE_deferred_access]]>, length=Some(1)>(field1(deref(read<ptr<@type[[TYPE_VEC_deferred_access_base]]>>(%[[VALUE_vec_]])))), sub<u32, overflow=wrap>(read<u32>(field0(deref(read<ptr<@type[[TYPE_VEC_deferred_access_base]]>>(%[[VALUE_vec_]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_VEC_deferred_access_base_pop:[0-9]+]] @VEC_deferred_access_base_pop(%[[VALUE_vec__2:[0-9]+]] vec_: ptr<@type[[TYPE_VEC_deferred_access_base]]>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if ne<u32>(read<u32>(field0(deref(read<ptr<@type[[TYPE_VEC_deferred_access_base]]>>(%[[VALUE_vec__2]])))), const<u32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE1]], const<i32>(0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_vec_assert_fail]]);
// DEFAULT-NEXT:             write<i32>(%[[VALUE1]], const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<@type[[TYPE_VEC_deferred_access_base]]> [synthetic] = read<ptr<@type[[TYPE_VEC_deferred_access_base]]>>(%[[VALUE_vec__2]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u32 [synthetic] = read<u32>(field0(deref(read<ptr<@type[[TYPE_VEC_deferred_access_base]]>>(%[[VALUE2]]))));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u32 [synthetic] = sub<u32, overflow=wrap>(read<u32>(%[[VALUE3]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(field0(deref(read<ptr<@type[[TYPE_VEC_deferred_access_base]]>>(%[[VALUE2]]))), read<u32>(%[[VALUE4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_perform_access_checks:[0-9]+]] @perform_access_checks(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_deferred_access_check]]>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pop_to_parent_deferring_access_checks:[0-9]+]] @pop_to_parent_deferring_access_checks() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_deferred_access_no_check]]), const<u32>(0))
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_deferred_access_no_check]]);
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: u32 [synthetic] = sub<u32, overflow=wrap>(read<u32>(%[[VALUE5]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<u32>(%[[VALUE_deferred_access_no_check]], read<u32>(%[[VALUE6]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_checks:[0-9]+]] checks: ptr<@type[[TYPE_deferred_access_check]]> [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_ptr:[0-9]+]] ptr: ptr<@type[[TYPE_deferred_access]]> [storage=automatic];
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_deferred_access_check]]>>(%[[VALUE_checks]], read<ptr<@type[[TYPE_deferred_access_check]]>>(field0(deref(call<ptr<@type[[TYPE_deferred_access]]>, signature=fn(ptr<@type[[TYPE_VEC_deferred_access_base]]>) -> ptr<@type[[TYPE_deferred_access]]>>(%[[VALUE_VEC_deferred_access_base_last]], conditional<ptr<@type[[TYPE_VEC_deferred_access_base]]>>(ne<ptr<@type[[TYPE_VEC_deferred_access_gc]]>>(read<ptr<@type[[TYPE_VEC_deferred_access_gc]]>>(%[[VALUE_deferred_access_stack]]), null<ptr<@type[[TYPE_VEC_deferred_access_gc]]>>), addr_of<ptr<@type[[TYPE_VEC_deferred_access_base]]>>(field0(deref(read<ptr<@type[[TYPE_VEC_deferred_access_gc]]>>(%[[VALUE_deferred_access_stack]])))), null<ptr<@type[[TYPE_VEC_deferred_access_base]]>>))))));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE_VEC_deferred_access_base]]>) -> void>(%[[VALUE_VEC_deferred_access_base_pop]], conditional<ptr<@type[[TYPE_VEC_deferred_access_base]]>>(ne<ptr<@type[[TYPE_VEC_deferred_access_gc]]>>(read<ptr<@type[[TYPE_VEC_deferred_access_gc]]>>(%[[VALUE_deferred_access_stack]]), null<ptr<@type[[TYPE_VEC_deferred_access_gc]]>>), addr_of<ptr<@type[[TYPE_VEC_deferred_access_base]]>>(field0(deref(read<ptr<@type[[TYPE_VEC_deferred_access_gc]]>>(%[[VALUE_deferred_access_stack]])))), null<ptr<@type[[TYPE_VEC_deferred_access_base]]>>));
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_deferred_access]]>>(%[[VALUE_ptr]], call<ptr<@type[[TYPE_deferred_access]]>, signature=fn(ptr<@type[[TYPE_VEC_deferred_access_base]]>) -> ptr<@type[[TYPE_deferred_access]]>>(%[[VALUE_VEC_deferred_access_base_last]], conditional<ptr<@type[[TYPE_VEC_deferred_access_base]]>>(ne<ptr<@type[[TYPE_VEC_deferred_access_gc]]>>(read<ptr<@type[[TYPE_VEC_deferred_access_gc]]>>(%[[VALUE_deferred_access_stack]]), null<ptr<@type[[TYPE_VEC_deferred_access_gc]]>>), addr_of<ptr<@type[[TYPE_VEC_deferred_access_base]]>>(field0(deref(read<ptr<@type[[TYPE_VEC_deferred_access_gc]]>>(%[[VALUE_deferred_access_stack]])))), null<ptr<@type[[TYPE_VEC_deferred_access_base]]>>)));
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_deferred_access]]>>(%[[VALUE_ptr]])))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type[[TYPE_deferred_access_check]]>) -> void>(%[[VALUE_perform_access_checks]], read<ptr<@type[[TYPE_deferred_access_check]]>>(%[[VALUE_checks]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_malloc:[0-9]+]] @__builtin_malloc(%[[VALUE7:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_VEC_deferred_access_gc]]>>(%[[VALUE_deferred_access_stack]], pointer_cast<ptr<@type[[TYPE_VEC_deferred_access_gc]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], add<u64, overflow=wrap>(const<u64>(24), mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))))));
// DEFAULT-NEXT:         write<u32>(field0(field0(deref(read<ptr<@type[[TYPE_VEC_deferred_access_gc]]>>(%[[VALUE_deferred_access_stack]])))), reinterpret<u32, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<i32>(field1(deref(ptr_offset<ptr<@type[[TYPE_deferred_access]]>, subtract=false, element=@type[[TYPE_deferred_access]], overflow=ub>(array_decay<ptr<@type[[TYPE_deferred_access]]>, length=Some(1)>(field1(field0(deref(read<ptr<@type[[TYPE_VEC_deferred_access_gc]]>>(%[[VALUE_deferred_access_stack]]))))), const<i32>(0)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_pop_to_parent_deferring_access_checks]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
