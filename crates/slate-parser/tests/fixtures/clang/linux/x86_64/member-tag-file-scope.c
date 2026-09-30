#include "member-tag-file-scope.h"

struct member_tag_easy {
  int x;
};
struct member_tag_nested {
  int y;
};

void use(struct member_tag_easy *data, struct member_tag_nested *nested) {
  member_tag_enter(data);
  member_tag_nested_enter(nested);
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
// DEFAULT-NEXT:     type @type[[TYPE_member_tag_guard:[0-9]+]] member_tag_guard = struct {
// DEFAULT-NEXT:         field0 data: ptr<@type[[TYPE_member_tag_easy:[0-9]+]]>;
// DEFAULT-NEXT:         field1 outer: @type[[TYPE_member_tag_outer:[0-9]+]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_member_tag_easy]] member_tag_easy = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_member_tag_outer]] member_tag_outer = struct {
// DEFAULT-NEXT:         field0 nested: ptr<@type[[TYPE_member_tag_nested:[0-9]+]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_member_tag_nested]] member_tag_nested = struct {
// DEFAULT-NEXT:         field0 y: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_member_tag_enter:[0-9]+]] @member_tag_enter(%[[VALUE_data:[0-9]+]] data: ptr<@type[[TYPE_member_tag_easy]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_member_tag_nested_enter:[0-9]+]] @member_tag_nested_enter(%[[VALUE_nested:[0-9]+]] nested: ptr<@type[[TYPE_member_tag_nested]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE_data_2:[0-9]+]] data: ptr<@type[[TYPE_member_tag_easy]]>, %[[VALUE_nested_2:[0-9]+]] nested: ptr<@type[[TYPE_member_tag_nested]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_member_tag_easy]]>) -> void>(%[[VALUE_member_tag_enter]], read<ptr<@type[[TYPE_member_tag_easy]]>>(%[[VALUE_data_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_member_tag_nested]]>) -> void>(%[[VALUE_member_tag_nested_enter]], read<ptr<@type[[TYPE_member_tag_nested]]>>(%[[VALUE_nested_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
