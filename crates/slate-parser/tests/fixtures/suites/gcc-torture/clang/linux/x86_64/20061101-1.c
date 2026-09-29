/* PR rtl-optimization/28970 */
/* Origin: Peter Bergner <bergner@vnet.ibm.com> */
/* { dg-require-effective-target int32plus } */

extern void abort(void);

int tar(int i) {
  if (i != 36863)
    abort();

  return -1;
}

void bug(int q, int bcount) {
  int j     = 0;
  int outgo = 0;

  while (j != -1) {
    outgo++;
    if (outgo > q - 1)
      outgo = q - 1;
    j = tar(outgo * bcount);
  }
}

int main(void) {
  bug(5, 36863);
  return 0;
}



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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_tar:[0-9]+]] @tar(%[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(36863))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bug:[0-9]+]] @bug(%[[VALUE_q:[0-9]+]] q: i32, %[[VALUE_bcount:[0-9]+]] bcount: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_outgo:[0-9]+]] outgo: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_j]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_outgo]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_outgo]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 if gt<i32>(read<i32>(%[[VALUE_outgo]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_q]]), const<i32>(1)))
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_outgo]], sub<i32, overflow=ub>(read<i32>(%[[VALUE_q]]), const<i32>(1)));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_tar]], mul<i32, overflow=ub>(read<i32>(%[[VALUE_outgo]]), read<i32>(%[[VALUE_bcount]]))));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32) -> i32>(%[[VALUE_tar]], mul<i32, overflow=ub>(read<i32>(%[[VALUE_outgo]]), read<i32>(%[[VALUE_bcount]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bug]], const<i32>(5), const<i32>(36863));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
