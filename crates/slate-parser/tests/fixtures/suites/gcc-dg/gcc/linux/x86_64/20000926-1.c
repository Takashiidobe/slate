/* Copyright (C) 2000  Free Software Foundation.
   by William Cohen  <wcohen@redhat.com>  */

/* { dg-do compile } */
/* { dg-options "" } */
#include <limits.h>

struct PDATA
{
    unsigned int  Dummy:(sizeof(int)*CHAR_BIT);
    const char*   PName;
};

typedef struct PDATA    P_DATA;

struct PLAYBOOK {
        const char * BookName;
        P_DATA       Play[0];
};

struct PLAYBOOK playbook  =
{
  "BookName",
  {
    { 1, "PName0" }, /* { dg-warning "(excess elements)|(near initialization)" } */
  }
};

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_PDATA:[0-9]+]] PDATA = struct {
// DEFAULT-NEXT:         field0 Dummy: u32 : 32;
// DEFAULT-NEXT:         field1 PName: ptr<const i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[Some(0), None], bit_units=[(0, 4)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_P_DATA:[0-9]+]] P_DATA = @type[[TYPE_PDATA]];
// DEFAULT-NEXT:     type @type[[TYPE_PLAYBOOK:[0-9]+]] PLAYBOOK = struct {
// DEFAULT-NEXT:         field0 BookName: ptr<const i8>;
// DEFAULT-NEXT:         field1 Play: array<@type[[TYPE_PDATA]], 0>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([66, 111, 111, 107, 78, 97, 109, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_playbook:[0-9]+]] playbook: @type[[TYPE_PLAYBOOK]] [storage=static] = aggregate<@type[[TYPE_PLAYBOOK]], zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str]])), field1 = aggregate<array<@type[[TYPE_PDATA]], 0>, zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
