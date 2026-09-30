// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17

struct z_candidate { struct z_candidate *next;int viable;};
int pedantic;

static struct z_candidate *
splice_viable (cands)
     struct z_candidate *cands;
{
  struct z_candidate **p = &cands;

  for (; *p; )
    {
      if (pedantic ? (*p)->viable == 1 : (*p)->viable)
        p = &((*p)->next);
      else
        *p = (*p)->next;
    }

  return cands;
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
// DEFAULT-NEXT:     type @type[[TYPE_z_candidate:[0-9]+]] z_candidate = struct {
// DEFAULT-NEXT:         field0 next: ptr<@type[[TYPE_z_candidate]]>;
// DEFAULT-NEXT:         field1 viable: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_pedantic:[0-9]+]] pedantic: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_splice_viable:[0-9]+]] @splice_viable(%[[VALUE_cands:[0-9]+]] cands: ptr<@type[[TYPE_z_candidate]]>) -> ptr<@type[[TYPE_z_candidate]]> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<ptr<@type[[TYPE_z_candidate]]>> [storage=automatic] = addr_of<ptr<ptr<@type[[TYPE_z_candidate]]>>>(%[[VALUE_cands]]);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<ptr<@type[[TYPE_z_candidate]]>>(read<ptr<@type[[TYPE_z_candidate]]>>(deref(read<ptr<ptr<@type[[TYPE_z_candidate]]>>>(%[[VALUE_p]]))), null<ptr<@type[[TYPE_z_candidate]]>>)
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(conditional<i32>(ne<i32>(read<i32>(%[[VALUE_pedantic]]), const<i32>(0)), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_z_candidate]]>>(deref(read<ptr<ptr<@type[[TYPE_z_candidate]]>>>(%[[VALUE_p]])))))), const<i32>(1))), read<i32>(field1(deref(read<ptr<@type[[TYPE_z_candidate]]>>(deref(read<ptr<ptr<@type[[TYPE_z_candidate]]>>>(%[[VALUE_p]]))))))), const<i32>(0))
// DEFAULT-NEXT:                         write<ptr<ptr<@type[[TYPE_z_candidate]]>>>(%[[VALUE_p]], addr_of<ptr<ptr<@type[[TYPE_z_candidate]]>>>(field0(deref(read<ptr<@type[[TYPE_z_candidate]]>>(deref(read<ptr<ptr<@type[[TYPE_z_candidate]]>>>(%[[VALUE_p]])))))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_z_candidate]]>>(deref(read<ptr<ptr<@type[[TYPE_z_candidate]]>>>(%[[VALUE_p]])), read<ptr<@type[[TYPE_z_candidate]]>>(field0(deref(read<ptr<@type[[TYPE_z_candidate]]>>(deref(read<ptr<ptr<@type[[TYPE_z_candidate]]>>>(%[[VALUE_p]])))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_z_candidate]]>>(%[[VALUE_cands]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
