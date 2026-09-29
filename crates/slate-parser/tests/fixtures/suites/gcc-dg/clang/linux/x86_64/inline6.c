/* { dg-do compile } */
/* { dg-options "-std=gnu89" } */
static int i;
extern int j;
extern inline int func1 (void) {
  return i++;  /* { dg-warning "static" } */
}
extern inline int func2 (void) {
  return j++;
}
inline int func3 (void) 
{
  return i++;
}

// SLATE-FILECHECK-STD DEFAULT gnu89
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
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     extern %[[VALUE_j:[0-9]+]] j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_func1:[0-9]+]] @func1() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE0]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func2:[0-9]+]] @func2() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func3:[0-9]+]] @func3() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
