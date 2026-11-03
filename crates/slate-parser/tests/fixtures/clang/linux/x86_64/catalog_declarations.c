// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES ALL
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-PREFIX-ARGS ALL --all-declarations
#include "inputs/catalog_declarations.h"

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
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN ALL
// ALL: module {
// ALL-NEXT:     target "x86_64-unknown-linux-gnu" {
// ALL-NEXT:         endian = little;
// ALL-NEXT:         pointer [size=8, align=8];
// ALL-NEXT:         stack_alignment = 16;
// ALL-NEXT:         long_double = f80;
// ALL-NEXT:         storage bool [size=1, align=1];
// ALL-NEXT:         storage i8, u8 [size=1, align=1];
// ALL-NEXT:         storage i16, u16 [size=2, align=2];
// ALL-NEXT:         storage i32, u32 [size=4, align=4];
// ALL-NEXT:         storage i64, u64 [size=8, align=8];
// ALL-NEXT:         storage i128, u128 [size=16, align=16];
// ALL-NEXT:         storage bf16 [size=2, align=2];
// ALL-NEXT:         storage f16 [size=2, align=2];
// ALL-NEXT:         storage f32 [size=4, align=4];
// ALL-NEXT:         storage f64 [size=8, align=8];
// ALL-NEXT:         storage f80 [size=16, align=16];
// ALL-NEXT:         storage f128 [size=16, align=16];
// ALL-NEXT:         storage d32 [size=4, align=4];
// ALL-NEXT:         storage d64 [size=8, align=8];
// ALL-NEXT:         storage d128 [size=16, align=16];
// ALL-NEXT:     }
// ALL-NEXT:     type @type[[TYPE___NSConstantString_tag:[0-9]+]] __NSConstantString_tag = struct {
// ALL-NEXT:         field0 isa: ptr<const i32>;
// ALL-NEXT:         field1 flags: i32;
// ALL-NEXT:         field2 str: ptr<const i8>;
// ALL-NEXT:         field3 length: i64;
// ALL-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// ALL-NEXT:     type @type[[TYPE___NSConstantString:[0-9]+]] __NSConstantString = @type[[TYPE___NSConstantString_tag]];
// ALL-NEXT:     type @type[[TYPE_CatalogRecord:[0-9]+]] CatalogRecord = struct {
// ALL-NEXT:         field0 value: i32;
// ALL-NEXT:     } [size=4, align=4, offsets=[0]];
// ALL-NEXT:     type @type[[TYPE_CatalogRecord_2:[0-9]+]] CatalogRecord = @type[[TYPE_CatalogRecord]];
// ALL-NEXT:     fn %[[VALUE_catalog_call:[0-9]+]] @catalog_call(%[[VALUE_record:[0-9]+]] record: ptr<@type[[TYPE_CatalogRecord]]> [c="CatalogRecord *"]) -> i32 [linkage=external] [c="int(CatalogRecord *)"];
// ALL-NEXT: }
// SLATE-FILECHECK-END ALL
