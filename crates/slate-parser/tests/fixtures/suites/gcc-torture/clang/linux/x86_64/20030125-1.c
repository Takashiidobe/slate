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
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_func:[0-9]+]] @func(%[[VALUE_valp:[0-9]+]] valp: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_val:[0-9]+]] val: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_locked:[0-9]+]] locked: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE_valp]])));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_val]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE1]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(0))
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_locked]], const<i32>(1));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_locked]], const<i32>(1));
// DEFAULT-NEXT:                         if not<bool>(ne<i32>(read<i32>(%[[VALUE_locked]]), const<i32>(0)))
// DEFAULT-NEXT:                             continue %[[VALUE0]];
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(0)))
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_val]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
