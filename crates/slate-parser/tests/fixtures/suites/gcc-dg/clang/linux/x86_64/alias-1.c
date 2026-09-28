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
// DEFAULT-NEXT:     type @type0 YYSTYPE = i32;
// DEFAULT-NEXT:     type @type1 tDefEntry = struct {
// DEFAULT-NEXT:         field0 t: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type2 tDefEntry = @type1;
// DEFAULT-NEXT:     type @type3 incomplete = struct incomplete;
// DEFAULT-NEXT:     fn %4 @addSibMacro(%5 list: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 ppT: ptr<ptr<@type1>> [storage=automatic] = pointer_cast<ptr<ptr<@type1>>, reason=explicit>(addr_of<ptr<i32>>(%5));
// DEFAULT-NEXT:         let %7 p: ptr<@type3> [storage=automatic] = pointer_cast<ptr<@type3>, reason=explicit>(addr_of<ptr<i32>>(%5));
// DEFAULT-NEXT:         return read<i32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
