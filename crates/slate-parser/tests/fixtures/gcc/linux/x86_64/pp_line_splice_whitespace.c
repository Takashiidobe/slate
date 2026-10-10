// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT

#define SUM(a, b) a + \ 
	b
int sum = SUM(1, 2);
int spaced = 1 + \  
2;
int tabbed = 3 + \	
4;
int form_fed = 5 + \

6;
int vertical_tabbed = 7 + \

8;
int crlf = 9 + \ 
10;
const char text[] = "ab\ 
cd";
// hidden \ 
int hidden_by_comment;
int visible;

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
// DEFAULT-NEXT:     global %[[VALUE_sum:[0-9]+]] sum: i32 [storage=static] = add<i32>(const<i32>(1), const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_spaced:[0-9]+]] spaced: i32 [storage=static] = add<i32>(const<i32>(1), const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tabbed:[0-9]+]] tabbed: i32 [storage=static] = add<i32>(const<i32>(3), const<i32>(4)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_form_fed:[0-9]+]] form_fed: i32 [storage=static] = add<i32>(const<i32>(5), const<i32>(6)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_vertical_tabbed:[0-9]+]] vertical_tabbed: i32 [storage=static] = add<i32>(const<i32>(7), const<i32>(8)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_crlf:[0-9]+]] crlf: i32 [storage=static] = add<i32>(const<i32>(9), const<i32>(10)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_text:[0-9]+]] text: array<i8, 5> [storage=static] [const] = code_units<array<i8, 5>>([97, 98, 99, 100, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_visible:[0-9]+]] visible: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
