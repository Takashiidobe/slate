/* { dg-do run { target *-wrs-vxworks } } */
/* { dg-require-effective-target tls } */
/* { dg-add-options tls } */

/* vxworks' TLS model requires no extra padding on the tls proxy
   objects.  */

__thread int i;
__thread int j;

extern int __tls__i;
extern int __tls__j;

int main ()
{
  int delta = ((char *)&__tls__j - (char *)&__tls__i);

  if (delta < 0)
    delta = -delta;
  
  return delta != 12;
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
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: i32 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE___tls__i:[0-9]+]] __tls__i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE___tls__j:[0-9]+]] __tls__j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_delta:[0-9]+]] delta: i32 [storage=automatic] = truncate<i32, reason=assign, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE___tls__j]])), pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE___tls__i]]))));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_delta]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_delta]], neg<i32, overflow=ub>(read<i32>(%[[VALUE_delta]])));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(read<i32>(%[[VALUE_delta]]), const<i32>(12)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
