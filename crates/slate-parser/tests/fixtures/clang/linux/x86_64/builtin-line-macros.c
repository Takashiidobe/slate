#include "builtin-line-macros-headers/header.h"

enum header_enum header_enum_holder;

int main_line = __LINE__;

#define WRAP(x) x
int line_via_macro = WRAP(__LINE__);

#line 100 "renamed.c"
int line_after_line_directive = __LINE__;
const char *file_after_line_directive = __FILE__;
int line_after_line_directive2 = __LINE__;

int counter0 = __COUNTER__;
int counter1 = __COUNTER__;

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
// DEFAULT-NEXT:     type @type[[TYPE_header_enum:[0-9]+]] header_enum = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_HEADER_LINE_VALUE:[0-9]+]] HEADER_LINE_VALUE = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_header_enum_holder:[0-9]+]] header_enum_holder: @type[[TYPE_header_enum]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_main_line:[0-9]+]] main_line: i32 [storage=static] = const<i32>(5) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_line_via_macro:[0-9]+]] line_via_macro: i32 [storage=static] = const<i32>(8) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_line_after_line_directive:[0-9]+]] line_after_line_directive: i32 [storage=static] = const<i32>(100) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([114, 101, 110, 97, 109, 101, 100, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_file_after_line_directive:[0-9]+]] file_after_line_directive: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_line_after_line_directive2:[0-9]+]] line_after_line_directive2: i32 [storage=static] = const<i32>(102) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_counter0:[0-9]+]] counter0: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_counter1:[0-9]+]] counter1: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
