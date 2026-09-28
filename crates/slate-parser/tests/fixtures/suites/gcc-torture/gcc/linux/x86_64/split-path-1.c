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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     fn %2 @malloc(%18 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @test() -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 Pels: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %8 xr: u8 [storage=automatic];
// DEFAULT-NEXT:         let %9 xg: u8 [storage=automatic];
// DEFAULT-NEXT:         let %10 xb: u8 [storage=automatic];
// DEFAULT-NEXT:         let %11 xc: u8 [storage=automatic];
// DEFAULT-NEXT:         let %12 xm: u8 [storage=automatic];
// DEFAULT-NEXT:         let %13 xy: u8 [storage=automatic];
// DEFAULT-NEXT:         let %14 xk: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %15 ReadPtr: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %16 EritePtr: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<u8>>(%15, pointer_cast<ptr<u8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(100)))))));
// DEFAULT-NEXT:         pointer_cast<ptr<u8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(100))))));
// DEFAULT-NEXT:         write<ptr<u8>>(%16, pointer_cast<ptr<u8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(100)))))));
// DEFAULT-NEXT:         pointer_cast<ptr<u8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(100))))));
// DEFAULT-NEXT:         for %19
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), const<i32>(100))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%22));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%15), read<i32>(%5))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(100), read<i32>(%5)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %20
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), const<i32>(24))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %23: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%24));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %25: ptr<u8> [synthetic] = read<ptr<u8>>(%15);
// DEFAULT-NEXT:                     let %26: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%25), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%15, read<ptr<u8>>(%26));
// DEFAULT-NEXT:                     write<u8>(%8, read<u8>(deref(read<ptr<u8>>(%25))));
// DEFAULT-NEXT:                     let %27: ptr<u8> [synthetic] = read<ptr<u8>>(%15);
// DEFAULT-NEXT:                     let %28: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%27), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%15, read<ptr<u8>>(%28));
// DEFAULT-NEXT:                     write<u8>(%9, read<u8>(deref(read<ptr<u8>>(%27))));
// DEFAULT-NEXT:                     let %29: ptr<u8> [synthetic] = read<ptr<u8>>(%15);
// DEFAULT-NEXT:                     let %30: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%29), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%15, read<ptr<u8>>(%30));
// DEFAULT-NEXT:                     write<u8>(%10, read<u8>(deref(read<ptr<u8>>(%29))));
// DEFAULT-NEXT:                     write<u8>(%11, reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(const<i32>(255), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%8)))))));
// DEFAULT-NEXT:                     write<u8>(%12, reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(const<i32>(255), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%9)))))));
// DEFAULT-NEXT:                     write<u8>(%13, reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(const<i32>(255), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%10)))))));
// DEFAULT-NEXT:                     if lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%11))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%12))))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<u8>(%14, reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(conditional<i32>(lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%11))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%13)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%11))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%13)))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<u8>(%14, reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(conditional<i32>(lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%12))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%13)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%12))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%13)))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     write<u8>(%11, reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%11))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%14)))))));
// DEFAULT-NEXT:                     write<u8>(%12, reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%12))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%14)))))));
// DEFAULT-NEXT:                     write<u8>(%13, reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%13))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%14)))))));
// DEFAULT-NEXT:                     let %31: ptr<u8> [synthetic] = read<ptr<u8>>(%16);
// DEFAULT-NEXT:                     let %32: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%31), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%16, read<ptr<u8>>(%32));
// DEFAULT-NEXT:                     write<u8>(deref(read<ptr<u8>>(%31)), read<u8>(%11));
// DEFAULT-NEXT:                     let %33: ptr<u8> [synthetic] = read<ptr<u8>>(%16);
// DEFAULT-NEXT:                     let %34: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%33), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%16, read<ptr<u8>>(%34));
// DEFAULT-NEXT:                     write<u8>(deref(read<ptr<u8>>(%33)), read<u8>(%12));
// DEFAULT-NEXT:                     let %35: ptr<u8> [synthetic] = read<ptr<u8>>(%16);
// DEFAULT-NEXT:                     let %36: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%35), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%16, read<ptr<u8>>(%36));
// DEFAULT-NEXT:                     write<u8>(deref(read<ptr<u8>>(%35)), read<u8>(%13));
// DEFAULT-NEXT:                     let %37: ptr<u8> [synthetic] = read<ptr<u8>>(%16);
// DEFAULT-NEXT:                     let %38: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%37), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%16, read<ptr<u8>>(%38));
// DEFAULT-NEXT:                     write<u8>(deref(read<ptr<u8>>(%37)), read<u8>(%14));
// DEFAULT-NEXT:                     let %39: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                     let %40: ptr<u8> [synthetic] = read<ptr<u8>>(%16);
// DEFAULT-NEXT:                     let %41: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=true, element=u8, overflow=ub>(read<ptr<u8>>(%40), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%16, read<ptr<u8>>(%41));
// DEFAULT-NEXT:                     let %42: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%39), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%41))))));
// DEFAULT-NEXT:                     write<i32>(%7, read<i32>(%42));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(read<i32>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn() -> u8>(%4))), const<i32>(196))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
