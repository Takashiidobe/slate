// { dg-do compile }
// { dg-options "-Wstrict-aliasing=2 -fstrict-aliasing" }

// Copyright (C) 2002 Free Software Foundation, Inc.
// Contributed by Nathan Sidwell 29 Sep 2002 <nathan@codesourcery.com>

// 8083. warn about odd casts

typedef int YYSTYPE;
typedef struct tDefEntry 
{
  unsigned t;
  
} tDefEntry;
struct incomplete;


YYSTYPE
 addSibMacro(
         YYSTYPE  list )
 {
     tDefEntry** ppT   = (tDefEntry**)&list; // { dg-warning "type-punned pointer will" }
 
     struct incomplete *p = (struct incomplete *)&list; // { dg-warning "type-punning to incomplete" }
     
     return list;
 }


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
// DEFAULT-NEXT:     type @type[[TYPE_YYSTYPE:[0-9]+]] YYSTYPE = i32;
// DEFAULT-NEXT:     type @type[[TYPE_tDefEntry:[0-9]+]] tDefEntry = struct {
// DEFAULT-NEXT:         field0 t: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_tDefEntry_2:[0-9]+]] tDefEntry = @type[[TYPE_tDefEntry]];
// DEFAULT-NEXT:     type @type[[TYPE_incomplete:[0-9]+]] incomplete = struct incomplete;
// DEFAULT-NEXT:     fn %[[VALUE_addSibMacro:[0-9]+]] @addSibMacro(%[[VALUE_list:[0-9]+]] list: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ppT:[0-9]+]] ppT: ptr<ptr<@type[[TYPE_tDefEntry]]>> [storage=automatic] = pointer_cast<ptr<ptr<@type[[TYPE_tDefEntry]]>>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_list]]));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_incomplete]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_incomplete]]>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_list]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_list]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
