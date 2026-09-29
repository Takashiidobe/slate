// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17

struct tree_decl
{
  unsigned in_system_header_flag:1;
};
union tree_node
{
  struct tree_decl decl;
};
typedef union tree_node *tree;
static int
redeclaration_error_message (olddecl)
     tree olddecl;
{
  if (({olddecl;})->decl.in_system_header_flag)
    ;
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
// DEFAULT-NEXT:     type @type[[TYPE_tree_decl:[0-9]+]] tree_decl = struct {
// DEFAULT-NEXT:         field0 in_system_header_flag: u32 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_tree_node:[0-9]+]] tree_node = union {
// DEFAULT-NEXT:         field0 decl: @type[[TYPE_tree_decl]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_tree:[0-9]+]] tree = ptr<@type[[TYPE_tree_node]]>;
// DEFAULT-NEXT:     fn %[[VALUE_redeclaration_error_message:[0-9]+]] @redeclaration_error_message(%[[VALUE_olddecl:[0-9]+]] olddecl: ptr<@type[[TYPE_tree_node]]>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<@type[[TYPE_tree_node]]> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE0]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_olddecl]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE0]])))))), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
