struct PMC {
  unsigned flags;
};

typedef struct Pcc_cell {
  struct PMC *p;
  long        bla;
  long        type;
} Pcc_cell;

extern void abort();
extern void Parrot_gc_mark_PMC_alive_fun(int *interp, struct PMC *pmc)
    __attribute__((noinline));

void Parrot_gc_mark_PMC_alive_fun(int *interp, struct PMC *pmc) { abort(); }

static void mark_cell(int *interp, Pcc_cell *c) __attribute__((__nonnull__(1)))
__attribute__((__nonnull__(2))) __attribute__((noinline));

static void mark_cell(int *interp, Pcc_cell *c) {
  if (c->type == 4 && c->p && !(c->p->flags & (1 << 18)))
    Parrot_gc_mark_PMC_alive_fun(interp, c->p);
}

void foo(int *interp, Pcc_cell *c);

void foo(int *interp, Pcc_cell *c) { mark_cell(interp, c); }

int main() {
  int      i;
  Pcc_cell c;
  c.p    = 0;
  c.bla  = 42;
  c.type = 4;
  foo(&i, &c);
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
// DEFAULT-NEXT:     type @type0 PMC = struct {
// DEFAULT-NEXT:         field0 flags: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 Pcc_cell = struct {
// DEFAULT-NEXT:         field0 p: ptr<@type0>;
// DEFAULT-NEXT:         field1 bla: i64;
// DEFAULT-NEXT:         field2 type: i64;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type2 Pcc_cell = @type1;
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @Parrot_gc_mark_PMC_alive_fun(%5 interp: ptr<i32>, %6 pmc: ptr<@type0>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @mark_cell(%8 interp: ptr<i32>, %9 c: ptr<@type1>) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(eq<i64>(read<i64>(field2(deref(read<ptr<@type1>>(%9)))), widen<i64, reason=usual_arith>(const<i32>(4))), ne<ptr<@type0>>(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%9)))), null<ptr<@type0>>)), not<bool>(ne<u32>(and<u32>(read<u32>(field0(deref(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%9))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(18)))), const<u32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i32>, ptr<@type0>) -> void>(%4, read<ptr<i32>>(%8), read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%9)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @foo(%11 interp: ptr<i32>, %12 c: ptr<@type1>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<@type1>) -> void>(%7, read<ptr<i32>>(%11), read<ptr<@type1>>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %15 c: @type1 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type0>>(field0(%15), null<ptr<@type0>>);
// DEFAULT-NEXT:         write<i64>(field1(%15), widen<i64, reason=assign>(const<i32>(42)));
// DEFAULT-NEXT:         write<i64>(field2(%15), widen<i64, reason=assign>(const<i32>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<@type1>) -> void>(%10, addr_of<ptr<i32>>(%14), addr_of<ptr<@type1>>(%15));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
