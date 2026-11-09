/* { dg-do compile } */

int
div_and_round_double (lden_orig, hden_orig)
     int lden_orig, hden_orig;
{
  int quo[4];
  register int i;
  unsigned int  work;
  register unsigned int  carry = 0;
  int  lden = lden_orig;
  int  hden = hden_orig;
  neg_double (&lden, &hden);
  for (i = 4 - 1; i >= 0; i--)
    {
      quo[i] = work / (unsigned int ) lden;
      carry = work % (unsigned int ) lden;
    }
  return 0;
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     fn %[[VALUE_neg_double:[0-9]+]] @neg_double(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_div_and_round_double:[0-9]+]] @div_and_round_double(%[[VALUE_lden_orig:[0-9]+]] lden_orig: i32, %[[VALUE_hden_orig:[0-9]+]] hden_orig: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_quo:[0-9]+]] quo: array<i32, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_work:[0-9]+]] work: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_carry:[0-9]+]] carry: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_lden:[0-9]+]] lden: i32 [storage=automatic] = read<i32>(%[[VALUE_lden_orig]]);
// DEFAULT-NEXT:         let %[[VALUE_hden:[0-9]+]] hden: i32 [storage=automatic] = read<i32>(%[[VALUE_hden_orig]]);
// DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_neg_double]], addr_of<ptr<i32>>(%[[VALUE_lden]]), addr_of<ptr<i32>>(%[[VALUE_hden]]));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], sub<i32, overflow=ub>(const<i32>(4), const<i32>(1)));
// DEFAULT-NEXT:             condition: ge<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_quo]]), read<i32>(%[[VALUE_i]]))), reinterpret<i32, reason=assign, fits=unknown>(div<u32, by_zero=ub>(read<u32>(%[[VALUE_work]]), reinterpret<u32, reason=explicit, fits=unknown>(read<i32>(%[[VALUE_lden]])))));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_carry]], rem<u32, by_zero=ub>(read<u32>(%[[VALUE_work]]), reinterpret<u32, reason=explicit, fits=unknown>(read<i32>(%[[VALUE_lden]]))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
