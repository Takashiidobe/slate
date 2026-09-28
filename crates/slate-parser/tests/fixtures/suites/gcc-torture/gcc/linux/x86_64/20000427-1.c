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
// DEFAULT-NEXT:     global %0 lwidth: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 lheight: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @FindNearestPowerOf2(%8 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @ConvertFor3dDriver(%4 requirePO2: i32, %5 maxAspect: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 oldw: i32 [storage=automatic] = read<i32>(%0);
// DEFAULT-NEXT:         let %7 oldh: i32 [storage=automatic] = read<i32>(%1);
// DEFAULT-NEXT:         write<i32>(%1, call<i32, signature=fn(i32) -> i32>(%2, read<i32>(%1)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%2, read<i32>(%1));
// DEFAULT-NEXT:         while %9 gt<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%0), read<i32>(%1)), read<i32>(%5))
// DEFAULT-NEXT:             let %10: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:             let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), read<i32>(%1));
// DEFAULT-NEXT:             write<i32>(%1, read<i32>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
