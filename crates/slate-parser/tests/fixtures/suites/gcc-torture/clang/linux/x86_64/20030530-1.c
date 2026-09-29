// SLATE-FILECHECK-DEFINES DEFAULT

union tree_node;
typedef union tree_node *tree;
struct tree_common
{
  tree type;
  unsigned lang_flag_0 : 1;
};
union tree_node
{
  struct tree_common common;
};
void bar (tree);
static void
java_check_regular_methods (tree class_decl)
{
  int saw_constructor = class_decl->common.type->common.lang_flag_0;
  tree class = class_decl->common.type;
  for (;;)
    {
      if (class)
        if (class_decl->common.type)
          bar (class);
    }
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
// DEFAULT-NEXT:     type @type[[TYPE_tree_node:[0-9]+]] tree_node = union {
// DEFAULT-NEXT:         field0 common: @type[[TYPE_tree_common:[0-9]+]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_tree:[0-9]+]] tree = ptr<@type[[TYPE_tree_node]]>;
// DEFAULT-NEXT:     type @type[[TYPE_tree_common]] tree_common = struct {
// DEFAULT-NEXT:         field0 type: ptr<@type[[TYPE_tree_node]]>;
// DEFAULT-NEXT:         field1 lang_flag_0: u32 : 1;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 1)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE_tree_node]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_java_check_regular_methods:[0-9]+]] @java_check_regular_methods(%[[VALUE_class_decl:[0-9]+]] class_decl: ptr<@type[[TYPE_tree_node]]>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_saw_constructor:[0-9]+]] saw_constructor: i32 [storage=automatic] = reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=8..9, bits=0..1>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(field0(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_class_decl]]))))))))));
// DEFAULT-NEXT:         let %[[VALUE_class:[0-9]+]] class: ptr<@type[[TYPE_tree_node]]> [storage=automatic] = read<ptr<@type[[TYPE_tree_node]]>>(field0(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_class_decl]])))));
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<ptr<@type[[TYPE_tree_node]]>>(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_class]]), null<ptr<@type[[TYPE_tree_node]]>>)
// DEFAULT-NEXT:                         if ne<ptr<@type[[TYPE_tree_node]]>>(read<ptr<@type[[TYPE_tree_node]]>>(field0(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_class_decl]]))))), null<ptr<@type[[TYPE_tree_node]]>>)
// DEFAULT-NEXT:                             call<void, signature=fn(ptr<@type[[TYPE_tree_node]]>) -> void>(%[[VALUE_bar]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_class]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
