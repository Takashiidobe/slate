struct PMC {
  unsigned flags;
};

typedef struct Pcc_cell {
  struct PMC *p;
  long        bla;
  long        type;
} Pcc_cell;

int gi;
int cond;

extern void abort();
extern void never_ever(int interp, struct PMC *pmc)
    __attribute__((noinline, noclone));

void never_ever(int interp, struct PMC *pmc) { abort(); }

static void mark_cell(int *interp, Pcc_cell *c) __attribute__((__nonnull__(1)));

static void mark_cell(int *interp, Pcc_cell *c) {
  if (!cond)
    return;

  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 18)))
    never_ever(gi + 1, c->p);
  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 17)))
    never_ever(gi + 2, c->p);
  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 16)))
    never_ever(gi + 3, c->p);
  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 15)))
    never_ever(gi + 4, c->p);
  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 14)))
    never_ever(gi + 5, c->p);
  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 13)))
    never_ever(gi + 6, c->p);
  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 12)))
    never_ever(gi + 7, c->p);
  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 11)))
    never_ever(gi + 8, c->p);
  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 10)))
    never_ever(gi + 9, c->p);
}

static void foo(int *interp, Pcc_cell *c) { mark_cell(interp, c); }

static struct Pcc_cell *__attribute__((noinline, noclone)) getnull(void) {
  return (struct Pcc_cell *)0;
}

int main() {
  int i;

  cond = 1;
  for (i = 0; i < 100; i++)
    foo(&gi, getnull());
  return 0;
}

void bar_1(int *interp, Pcc_cell *c) {
  c->bla += 1;
  mark_cell(interp, c);
}

void bar_2(int *interp, Pcc_cell *c) {
  c->bla += 2;
  mark_cell(interp, c);
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
// DEFAULT-NEXT:     type @type[[TYPE_PMC:[0-9]+]] PMC = struct {
// DEFAULT-NEXT:         field0 flags: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Pcc_cell:[0-9]+]] Pcc_cell = struct {
// DEFAULT-NEXT:         field0 p: ptr<@type[[TYPE_PMC]]>;
// DEFAULT-NEXT:         field1 bla: i64;
// DEFAULT-NEXT:         field2 type: i64;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_Pcc_cell_2:[0-9]+]] Pcc_cell = @type[[TYPE_Pcc_cell]];
// DEFAULT-NEXT:     global %[[VALUE_gi:[0-9]+]] gi: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cond:[0-9]+]] cond: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_never_ever:[0-9]+]] @never_ever(%[[VALUE_interp:[0-9]+]] interp: i32, %[[VALUE_pmc:[0-9]+]] pmc: ptr<@type[[TYPE_PMC]]>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_mark_cell:[0-9]+]] @mark_cell(%[[VALUE_interp_2:[0-9]+]] interp: ptr<i32>, %[[VALUE_c:[0-9]+]] c: ptr<@type[[TYPE_Pcc_cell]]>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_cond]]), const<i32>(0)))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type[[TYPE_Pcc_cell]]>>(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]), null<ptr<@type[[TYPE_Pcc_cell]]>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type[[TYPE_PMC]]>>(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), null<ptr<@type[[TYPE_PMC]]>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(18)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type[[TYPE_PMC]]>) -> void>(%[[VALUE_never_ever]], add<i32, overflow=ub>(read<i32>(%[[VALUE_gi]]), const<i32>(1)), read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type[[TYPE_Pcc_cell]]>>(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]), null<ptr<@type[[TYPE_Pcc_cell]]>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type[[TYPE_PMC]]>>(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), null<ptr<@type[[TYPE_PMC]]>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type[[TYPE_PMC]]>) -> void>(%[[VALUE_never_ever]], add<i32, overflow=ub>(read<i32>(%[[VALUE_gi]]), const<i32>(2)), read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type[[TYPE_Pcc_cell]]>>(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]), null<ptr<@type[[TYPE_Pcc_cell]]>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type[[TYPE_PMC]]>>(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), null<ptr<@type[[TYPE_PMC]]>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type[[TYPE_PMC]]>) -> void>(%[[VALUE_never_ever]], add<i32, overflow=ub>(read<i32>(%[[VALUE_gi]]), const<i32>(3)), read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type[[TYPE_Pcc_cell]]>>(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]), null<ptr<@type[[TYPE_Pcc_cell]]>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type[[TYPE_PMC]]>>(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), null<ptr<@type[[TYPE_PMC]]>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type[[TYPE_PMC]]>) -> void>(%[[VALUE_never_ever]], add<i32, overflow=ub>(read<i32>(%[[VALUE_gi]]), const<i32>(4)), read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type[[TYPE_Pcc_cell]]>>(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]), null<ptr<@type[[TYPE_Pcc_cell]]>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type[[TYPE_PMC]]>>(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), null<ptr<@type[[TYPE_PMC]]>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(14)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type[[TYPE_PMC]]>) -> void>(%[[VALUE_never_ever]], add<i32, overflow=ub>(read<i32>(%[[VALUE_gi]]), const<i32>(5)), read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type[[TYPE_Pcc_cell]]>>(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]), null<ptr<@type[[TYPE_Pcc_cell]]>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type[[TYPE_PMC]]>>(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), null<ptr<@type[[TYPE_PMC]]>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(13)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type[[TYPE_PMC]]>) -> void>(%[[VALUE_never_ever]], add<i32, overflow=ub>(read<i32>(%[[VALUE_gi]]), const<i32>(6)), read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type[[TYPE_Pcc_cell]]>>(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]), null<ptr<@type[[TYPE_Pcc_cell]]>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type[[TYPE_PMC]]>>(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), null<ptr<@type[[TYPE_PMC]]>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(12)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type[[TYPE_PMC]]>) -> void>(%[[VALUE_never_ever]], add<i32, overflow=ub>(read<i32>(%[[VALUE_gi]]), const<i32>(7)), read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type[[TYPE_Pcc_cell]]>>(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]), null<ptr<@type[[TYPE_Pcc_cell]]>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type[[TYPE_PMC]]>>(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), null<ptr<@type[[TYPE_PMC]]>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type[[TYPE_PMC]]>) -> void>(%[[VALUE_never_ever]], add<i32, overflow=ub>(read<i32>(%[[VALUE_gi]]), const<i32>(8)), read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type[[TYPE_Pcc_cell]]>>(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]), null<ptr<@type[[TYPE_Pcc_cell]]>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type[[TYPE_PMC]]>>(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))), null<ptr<@type[[TYPE_PMC]]>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]]))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(10)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type[[TYPE_PMC]]>) -> void>(%[[VALUE_never_ever]], add<i32, overflow=ub>(read<i32>(%[[VALUE_gi]]), const<i32>(9)), read<ptr<@type[[TYPE_PMC]]>>(field0(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_interp_3:[0-9]+]] interp: ptr<i32>, %[[VALUE_c_2:[0-9]+]] c: ptr<@type[[TYPE_Pcc_cell]]>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<@type[[TYPE_Pcc_cell]]>) -> void>(%[[VALUE_mark_cell]], read<ptr<i32>>(%[[VALUE_interp_3]]), read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_getnull:[0-9]+]] @getnull() -> ptr<@type[[TYPE_Pcc_cell]]> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<@type[[TYPE_Pcc_cell]]>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_cond]], const<i32>(1));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(100))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<i32>, ptr<@type[[TYPE_Pcc_cell]]>) -> void>(%[[VALUE_foo]], addr_of<ptr<i32>>(%[[VALUE_gi]]), call<ptr<@type[[TYPE_Pcc_cell]]>, signature=fn() -> ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_getnull]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar_1:[0-9]+]] @bar_1(%[[VALUE_interp_4:[0-9]+]] interp: ptr<i32>, %[[VALUE_c_3:[0-9]+]] c: ptr<@type[[TYPE_Pcc_cell]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<@type[[TYPE_Pcc_cell]]> [synthetic] = read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c_3]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i64 [synthetic] = read<i64>(field1(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE3]]))));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE4]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(field1(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE3]]))), read<i64>(%[[VALUE5]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<@type[[TYPE_Pcc_cell]]>) -> void>(%[[VALUE_mark_cell]], read<ptr<i32>>(%[[VALUE_interp_4]]), read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar_2:[0-9]+]] @bar_2(%[[VALUE_interp_5:[0-9]+]] interp: ptr<i32>, %[[VALUE_c_4:[0-9]+]] c: ptr<@type[[TYPE_Pcc_cell]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<@type[[TYPE_Pcc_cell]]> [synthetic] = read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c_4]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i64 [synthetic] = read<i64>(field1(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE6]]))));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE7]]), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(field1(deref(read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE6]]))), read<i64>(%[[VALUE8]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<@type[[TYPE_Pcc_cell]]>) -> void>(%[[VALUE_mark_cell]], read<ptr<i32>>(%[[VALUE_interp_5]]), read<ptr<@type[[TYPE_Pcc_cell]]>>(%[[VALUE_c_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
