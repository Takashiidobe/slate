// SLATE-FILECHECK-DEFINES DEFAULT

 int count;

 int func(int *valp) {
   int val, locked = 0;

   while ((val = *valp) != 0) {
     if (count) {
       if (count)
         locked = 1;
       else
         locked = 1;

     if (!locked)
       continue;
     }

     if (!count)
       count--;

     break;
   }

   return val;
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
// DEFAULT-NEXT:     global %0 count: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @func(%2 valp: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 val: i32 [storage=automatic];
// DEFAULT-NEXT:         let %4 locked: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %5 {
// DEFAULT-NEXT:             write<i32>(%3, read<i32>(deref(read<ptr<i32>>(%2))));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(deref(read<ptr<i32>>(%2))), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:                             write<i32>(%4, const<i32>(1));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<i32>(%4, const<i32>(1));
// DEFAULT-NEXT:                         if not<bool>(ne<i32>(read<i32>(%4), const<i32>(0)))
// DEFAULT-NEXT:                             continue %5;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%0), const<i32>(0)))
// DEFAULT-NEXT:                     let %6: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                     let %7: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%6), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%0, read<i32>(%7));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
