// SLATE-FILECHECK-DEFINES DEFAULT

/* This testcase ICEd when 2 different successors of a basic block
   were successfully threaded and try_forward_edges was not expecting
   that.  */

typedef struct A
{
  struct A *s, *t;
  unsigned int u;
} A;

void bar (A *);

void
foo (A *x, A *y, A *z)
{
  while (y
	 && (((y && y->t && y->t->u) ? y : z)->t
	     == ((x && x->t && x->t->u) ? x : z)->t))
    y = y->s;

  if (y)
    bar (y);
}

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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 s: ptr<@type0>;
// DEFAULT-NEXT:         field1 t: ptr<@type0>;
// DEFAULT-NEXT:         field2 u: u32;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type1 A = @type0;
// DEFAULT-NEXT:     fn %2 @bar(%7 <unnamed>: ptr<@type0>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 x: ptr<@type0>, %5 y: ptr<@type0>, %6 z: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         while %8 logical_and<bool>(ne<ptr<@type0>>(read<ptr<@type0>>(%5), null<ptr<@type0>>), eq<ptr<@type0>>(read<ptr<@type0>>(field1(deref(conditional<ptr<@type0>>(logical_and<bool>(logical_and<bool>(ne<ptr<@type0>>(read<ptr<@type0>>(%5), null<ptr<@type0>>), ne<ptr<@type0>>(read<ptr<@type0>>(field1(deref(read<ptr<@type0>>(%5)))), null<ptr<@type0>>)), ne<u32>(read<u32>(field2(deref(read<ptr<@type0>>(field1(deref(read<ptr<@type0>>(%5))))))), const<u32>(0))), read<ptr<@type0>>(%5), read<ptr<@type0>>(%6))))), read<ptr<@type0>>(field1(deref(conditional<ptr<@type0>>(logical_and<bool>(logical_and<bool>(ne<ptr<@type0>>(read<ptr<@type0>>(%4), null<ptr<@type0>>), ne<ptr<@type0>>(read<ptr<@type0>>(field1(deref(read<ptr<@type0>>(%4)))), null<ptr<@type0>>)), ne<u32>(read<u32>(field2(deref(read<ptr<@type0>>(field1(deref(read<ptr<@type0>>(%4))))))), const<u32>(0))), read<ptr<@type0>>(%4), read<ptr<@type0>>(%6)))))))
// DEFAULT-NEXT:             write<ptr<@type0>>(%5, read<ptr<@type0>>(field0(deref(read<ptr<@type0>>(%5)))));
// DEFAULT-NEXT:         if ne<ptr<@type0>>(read<ptr<@type0>>(%5), null<ptr<@type0>>)
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type0>) -> void>(%2, read<ptr<@type0>>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
