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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 s: ptr<@type[[TYPE_A]]>;
// DEFAULT-NEXT:         field1 t: ptr<@type[[TYPE_A]]>;
// DEFAULT-NEXT:         field2 u: u32;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_A_2:[0-9]+]] A = @type[[TYPE_A]];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE_A]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_A]]>, %[[VALUE_y:[0-9]+]] y: ptr<@type[[TYPE_A]]>, %[[VALUE_z:[0-9]+]] z: ptr<@type[[TYPE_A]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         while %[[VALUE1:[0-9]+]]
// DEFAULT-SAME: logical_and<bool>(ne<ptr<@type[[TYPE_A]]>>(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_y]]),
// DEFAULT-SAME: null<ptr<@type[[TYPE_A]]>>),
// DEFAULT-SAME: eq<ptr<@type[[TYPE_A]]>>(read<ptr<@type[[TYPE_A]]>>(field1(deref(conditional<ptr<@type[[TYPE_A]]>>(logical_and<bool>(logical_and<bool>(ne<ptr<@type[[TYPE_A]]>>(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_y]]),
// DEFAULT-SAME: null<ptr<@type[[TYPE_A]]>>),
// DEFAULT-SAME: ne<ptr<@type[[TYPE_A]]>>(read<ptr<@type[[TYPE_A]]>>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_y]])))),
// DEFAULT-SAME: null<ptr<@type[[TYPE_A]]>>)),
// DEFAULT-SAME: ne<u32>(read<u32>(field2(deref(read<ptr<@type[[TYPE_A]]>>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_y]]))))))), const<u32>(0))),
// DEFAULT-SAME: read<ptr<@type[[TYPE_A]]>>(%[[VALUE_y]]),
// DEFAULT-SAME: read<ptr<@type[[TYPE_A]]>>(%[[VALUE_z]]))))),
// DEFAULT-SAME: read<ptr<@type[[TYPE_A]]>>(field1(deref(conditional<ptr<@type[[TYPE_A]]>>(logical_and<bool>(logical_and<bool>(ne<ptr<@type[[TYPE_A]]>>(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_x]]),
// DEFAULT-SAME: null<ptr<@type[[TYPE_A]]>>),
// DEFAULT-SAME: ne<ptr<@type[[TYPE_A]]>>(read<ptr<@type[[TYPE_A]]>>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_x]])))),
// DEFAULT-SAME: null<ptr<@type[[TYPE_A]]>>)),
// DEFAULT-SAME: ne<u32>(read<u32>(field2(deref(read<ptr<@type[[TYPE_A]]>>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_x]]))))))), const<u32>(0))),
// DEFAULT-SAME: read<ptr<@type[[TYPE_A]]>>(%[[VALUE_x]]),
// DEFAULT-SAME: read<ptr<@type[[TYPE_A]]>>(%[[VALUE_z]])))))))
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_A]]>>(%[[VALUE_y]], read<ptr<@type[[TYPE_A]]>>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_y]])))));
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_A]]>>(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_y]]), null<ptr<@type[[TYPE_A]]>>)
// DEFAULT-NEXT:             call<void, signature=fn(ptr<@type[[TYPE_A]]>) -> void>(%[[VALUE_bar]], read<ptr<@type[[TYPE_A]]>>(%[[VALUE_y]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
