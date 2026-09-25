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
// DEFAULT-NEXT:     type @type0 PMC = struct {
// DEFAULT-NEXT:         field0 flags: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 Pcc_cell = struct {
// DEFAULT-NEXT:         field0 p: ptr<@type0>;
// DEFAULT-NEXT:         field1 bla: i64;
// DEFAULT-NEXT:         field2 type: i64;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type2 Pcc_cell = @type1;
// DEFAULT-NEXT:     global %3 gi: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 cond: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %5 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @never_ever(%7 interp: i32, %8 pmc: ptr<@type0>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @mark_cell(%10 interp: ptr<i32>, %11 c: ptr<@type1>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%4), const<i32>(0)))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type1>>(read<ptr<@type1>>(%11), null<ptr<@type1>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type1>>(%11)))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type0>>(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))), null<ptr<@type0>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(18)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type0>) -> void>(%6, add<i32, overflow=ub>(read<i32>(%3), const<i32>(1)), read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type1>>(read<ptr<@type1>>(%11), null<ptr<@type1>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type1>>(%11)))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type0>>(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))), null<ptr<@type0>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type0>) -> void>(%6, add<i32, overflow=ub>(read<i32>(%3), const<i32>(2)), read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type1>>(read<ptr<@type1>>(%11), null<ptr<@type1>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type1>>(%11)))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type0>>(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))), null<ptr<@type0>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type0>) -> void>(%6, add<i32, overflow=ub>(read<i32>(%3), const<i32>(3)), read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type1>>(read<ptr<@type1>>(%11), null<ptr<@type1>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type1>>(%11)))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type0>>(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))), null<ptr<@type0>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type0>) -> void>(%6, add<i32, overflow=ub>(read<i32>(%3), const<i32>(4)), read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type1>>(read<ptr<@type1>>(%11), null<ptr<@type1>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type1>>(%11)))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type0>>(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))), null<ptr<@type0>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(14)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type0>) -> void>(%6, add<i32, overflow=ub>(read<i32>(%3), const<i32>(5)), read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type1>>(read<ptr<@type1>>(%11), null<ptr<@type1>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type1>>(%11)))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type0>>(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))), null<ptr<@type0>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(13)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type0>) -> void>(%6, add<i32, overflow=ub>(read<i32>(%3), const<i32>(6)), read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type1>>(read<ptr<@type1>>(%11), null<ptr<@type1>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type1>>(%11)))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type0>>(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))), null<ptr<@type0>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(12)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type0>) -> void>(%6, add<i32, overflow=ub>(read<i32>(%3), const<i32>(7)), read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type1>>(read<ptr<@type1>>(%11), null<ptr<@type1>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type1>>(%11)))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type0>>(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))), null<ptr<@type0>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type0>) -> void>(%6, add<i32, overflow=ub>(read<i32>(%3), const<i32>(8)), read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<@type1>>(read<ptr<@type1>>(%11), null<ptr<@type1>>), eq<i64>(read<i64>(field2(deref(read<ptr<@type1>>(%11)))), widen<i64, reason=usual_arith>(const<i32>(4)))), ne<ptr<@type0>>(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))), null<ptr<@type0>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(10)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<@type0>) -> void>(%6, add<i32, overflow=ub>(read<i32>(%3), const<i32>(9)), read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%11)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @foo(%13 interp: ptr<i32>, %14 c: ptr<@type1>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<@type1>) -> void>(%9, read<ptr<i32>>(%13), read<ptr<@type1>>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @getnull() -> ptr<@type1> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<@type1>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %17 i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%4, const<i32>(1));
// DEFAULT-NEXT:         for %28
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%17, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%17), const<i32>(100))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32>(%30));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<i32>, ptr<@type1>) -> void>(%12, addr_of<ptr<i32>>(%3), call<ptr<@type1>, signature=fn() -> ptr<@type1>>(%15));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @bar_1(%19 interp: ptr<i32>, %20 c: ptr<@type1>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %31: ptr<@type1> [synthetic] = read<ptr<@type1>>(%20);
// DEFAULT-NEXT:         let %32: i64 [synthetic] = read<i64>(field1(deref(read<ptr<@type1>>(%31))));
// DEFAULT-NEXT:         let %33: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%32), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(field1(deref(read<ptr<@type1>>(%31))), read<i64>(%33));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<@type1>) -> void>(%9, read<ptr<i32>>(%19), read<ptr<@type1>>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @bar_2(%22 interp: ptr<i32>, %23 c: ptr<@type1>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %34: ptr<@type1> [synthetic] = read<ptr<@type1>>(%23);
// DEFAULT-NEXT:         let %35: i64 [synthetic] = read<i64>(field1(deref(read<ptr<@type1>>(%34))));
// DEFAULT-NEXT:         let %36: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%35), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(field1(deref(read<ptr<@type1>>(%34))), read<i64>(%36));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<@type1>) -> void>(%9, read<ptr<i32>>(%22), read<ptr<@type1>>(%23));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
