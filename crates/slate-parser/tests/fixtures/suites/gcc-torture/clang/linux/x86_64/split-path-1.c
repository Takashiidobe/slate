/* Note both PHI-OPT and the loop if conversion pass converts the inner if to be
 * branchless using min/max. */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define RGBMAX 255

unsigned char test() {
  int            i, Pels;
  int            sum = 0;
  unsigned char  xr, xg, xb;
  unsigned char  xc, xm, xy, xk = 0;
  unsigned char *ReadPtr, *EritePtr;

  ReadPtr  = (unsigned char *)malloc(sizeof(unsigned char) * 100);
  EritePtr = (unsigned char *)malloc(sizeof(unsigned char) * 100);

  for (i = 0; i < 100; i++) {
    ReadPtr[i] = 100 - i;
  }

  for (i = 0; i < 24; i++) {
    xr = *ReadPtr++;
    xg = *ReadPtr++;
    xb = *ReadPtr++;

    xc = (unsigned char)(RGBMAX - xr);
    xm = (unsigned char)(RGBMAX - xg);
    xy = (unsigned char)(RGBMAX - xb);

    if (xc < xm) {
      xk = (unsigned char)(xc < xy ? xc : xy);
    } else {
      xk = (unsigned char)(xm < xy ? xm : xy);
    }

    xc = (unsigned char)(xc - xk);
    xm = (unsigned char)(xm - xk);
    xy = (unsigned char)(xy - xk);

    *EritePtr++  = xc;
    *EritePtr++  = xm;
    *EritePtr++  = xy;
    *EritePtr++  = xk;
    sum         += *(--EritePtr);
  }
  return sum;
}

int main() {
  if (test() != 196)
    abort();

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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test() -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_Pels:[0-9]+]] Pels: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_sum:[0-9]+]] sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_xr:[0-9]+]] xr: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_xg:[0-9]+]] xg: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_xb:[0-9]+]] xb: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_xc:[0-9]+]] xc: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_xm:[0-9]+]] xm: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_xy:[0-9]+]] xy: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_xk:[0-9]+]] xk: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_ReadPtr:[0-9]+]] ReadPtr: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_EritePtr:[0-9]+]] EritePtr: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<u8>>(%[[VALUE_ReadPtr]], pointer_cast<ptr<u8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(100)))))));
// DEFAULT-NEXT:         write<ptr<u8>>(%[[VALUE_EritePtr]], pointer_cast<ptr<u8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(100)))))));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(100))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_ReadPtr]]), read<i32>(%[[VALUE_i]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(100), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(24))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_ReadPtr]]);
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%[[VALUE_ReadPtr]], read<ptr<u8>>(%[[VALUE7]]));
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_xr]], read<u8>(deref(read<ptr<u8>>(%[[VALUE6]]))));
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_ReadPtr]]);
// DEFAULT-NEXT:                     let %[[VALUE9:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%[[VALUE_ReadPtr]], read<ptr<u8>>(%[[VALUE9]]));
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_xg]], read<u8>(deref(read<ptr<u8>>(%[[VALUE8]]))));
// DEFAULT-NEXT:                     let %[[VALUE10:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_ReadPtr]]);
// DEFAULT-NEXT:                     let %[[VALUE11:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%[[VALUE_ReadPtr]], read<ptr<u8>>(%[[VALUE11]]));
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_xb]], read<u8>(deref(read<ptr<u8>>(%[[VALUE10]]))));
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_xc]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(const<i32>(255), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xr]])))))));
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_xm]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(const<i32>(255), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xg]])))))));
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_xy]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(const<i32>(255), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xb]])))))));
// DEFAULT-NEXT:                     if lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xc]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xm]]))))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<u8>(%[[VALUE_xk]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(conditional<i32>(lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xc]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xy]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xc]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xy]])))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<u8>(%[[VALUE_xk]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(conditional<i32>(lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xm]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xy]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xm]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xy]])))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_xc]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xc]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xk]])))))));
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_xm]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xm]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xk]])))))));
// DEFAULT-NEXT:                     write<u8>(%[[VALUE_xy]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xy]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_xk]])))))));
// DEFAULT-NEXT:                     let %[[VALUE12:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_EritePtr]]);
// DEFAULT-NEXT:                     let %[[VALUE13:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%[[VALUE_EritePtr]], read<ptr<u8>>(%[[VALUE13]]));
// DEFAULT-NEXT:                     write<u8>(deref(read<ptr<u8>>(%[[VALUE12]])), read<u8>(%[[VALUE_xc]]));
// DEFAULT-NEXT:                     let %[[VALUE14:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_EritePtr]]);
// DEFAULT-NEXT:                     let %[[VALUE15:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%[[VALUE_EritePtr]], read<ptr<u8>>(%[[VALUE15]]));
// DEFAULT-NEXT:                     write<u8>(deref(read<ptr<u8>>(%[[VALUE14]])), read<u8>(%[[VALUE_xm]]));
// DEFAULT-NEXT:                     let %[[VALUE16:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_EritePtr]]);
// DEFAULT-NEXT:                     let %[[VALUE17:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%[[VALUE_EritePtr]], read<ptr<u8>>(%[[VALUE17]]));
// DEFAULT-NEXT:                     write<u8>(deref(read<ptr<u8>>(%[[VALUE16]])), read<u8>(%[[VALUE_xy]]));
// DEFAULT-NEXT:                     let %[[VALUE18:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_EritePtr]]);
// DEFAULT-NEXT:                     let %[[VALUE19:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%[[VALUE_EritePtr]], read<ptr<u8>>(%[[VALUE19]]));
// DEFAULT-NEXT:                     write<u8>(deref(read<ptr<u8>>(%[[VALUE18]])), read<u8>(%[[VALUE_xk]]));
// DEFAULT-NEXT:                     let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_sum]]);
// DEFAULT-NEXT:                     let %[[VALUE21:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_EritePtr]]);
// DEFAULT-NEXT:                     let %[[VALUE22:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=true, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE21]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%[[VALUE_EritePtr]], read<ptr<u8>>(%[[VALUE22]]));
// DEFAULT-NEXT:                     let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%[[VALUE22]]))))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_sum]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(read<i32>(%[[VALUE_sum]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn() -> u8>(%[[VALUE_test]]))), const<i32>(196))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
