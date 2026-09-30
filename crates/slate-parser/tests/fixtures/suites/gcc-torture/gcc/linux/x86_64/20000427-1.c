// SLATE-FILECHECK-DEFINES DEFAULT

int lwidth;                                                                   
int lheight;                                                                  
int FindNearestPowerOf2 (int);
void ConvertFor3dDriver (int requirePO2, int maxAspect)       
{                                                     
  int oldw = lwidth, oldh = lheight;                      

  lheight = FindNearestPowerOf2 (lheight);            
  while (lwidth/lheight > maxAspect) lheight += lheight;              
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
// DEFAULT-NEXT:     global %[[VALUE_lwidth:[0-9]+]] lwidth: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_lheight:[0-9]+]] lheight: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_FindNearestPowerOf2:[0-9]+]] @FindNearestPowerOf2(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ConvertFor3dDriver:[0-9]+]] @ConvertFor3dDriver(%[[VALUE_requirePO2:[0-9]+]] requirePO2: i32, %[[VALUE_maxAspect:[0-9]+]] maxAspect: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_oldw:[0-9]+]] oldw: i32 [storage=automatic] = read<i32>(%[[VALUE_lwidth]]);
// DEFAULT-NEXT:         let %[[VALUE_oldh:[0-9]+]] oldh: i32 [storage=automatic] = read<i32>(%[[VALUE_lheight]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_lheight]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_FindNearestPowerOf2]], read<i32>(%[[VALUE_lheight]])));
// DEFAULT-NEXT:         while %[[VALUE1:[0-9]+]] gt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_lwidth]]), read<i32>(%[[VALUE_lheight]])), read<i32>(%[[VALUE_maxAspect]]))
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_lheight]]);
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), read<i32>(%[[VALUE_lheight]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_lheight]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
