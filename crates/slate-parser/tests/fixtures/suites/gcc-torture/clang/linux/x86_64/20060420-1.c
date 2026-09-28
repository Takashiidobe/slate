extern void abort(void);

typedef float v4flt __attribute__((vector_size(16)));

void __attribute__((noinline)) foo(float *dst, float **src, int a, int n) {
  int      i, j;
  int      z = sizeof(v4flt) / sizeof(float);
  unsigned m = sizeof(v4flt) - 1;

  for (j = 0; j < n && (((unsigned long)dst + j) & m); ++j) {
    float t = src[0][j];
    for (i = 1; i < a; ++i)
      t += src[i][j];
    dst[j] = t;
  }

  for (; j < (n - (4 * z - 1)); j += 4 * z) {
    v4flt t0 = *(v4flt *)(src[0] + j + 0 * z);
    v4flt t1 = *(v4flt *)(src[0] + j + 1 * z);
    v4flt t2 = *(v4flt *)(src[0] + j + 2 * z);
    v4flt t3 = *(v4flt *)(src[0] + j + 3 * z);
    for (i = 1; i < a; ++i) {
      t0 += *(v4flt *)(src[i] + j + 0 * z);
      t1 += *(v4flt *)(src[i] + j + 1 * z);
      t2 += *(v4flt *)(src[i] + j + 2 * z);
      t3 += *(v4flt *)(src[i] + j + 3 * z);
    }
    *(v4flt *)(dst + j + 0 * z) = t0;
    *(v4flt *)(dst + j + 1 * z) = t1;
    *(v4flt *)(dst + j + 2 * z) = t2;
    *(v4flt *)(dst + j + 3 * z) = t3;
  }
  for (; j < n; ++j) {
    float t = src[0][j];
    for (i = 1; i < a; ++i)
      t += src[i][j];
    dst[j] = t;
  }
}

float buffer[64];

int main(void) {
  int    i;
  float *dst, *src[2];
  char  *cptr;

  cptr    = (char *)buffer;
  cptr   += (-(long int)buffer & (16 * sizeof(float) - 1));
  dst     = (float *)cptr;
  src[0]  = dst + 16;
  src[1]  = dst + 32;
  for (i = 0; i < 16; ++i) {
    src[0][i] = (float)i + 11 * (float)i;
    src[1][i] = (float)i + 12 * (float)i;
  }
  foo(dst, src, 2, 16);
  for (i = 0; i < 16; ++i) {
    float e = (float)i + 11 * (float)i + (float)i + 12 * (float)i;
    if (dst[i] != e)
      abort();
  }
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
// DEFAULT-NEXT:     type @type0 v4flt = vector<f32, 4>;
// DEFAULT-NEXT:     global %17 buffer: array<f32, 64> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @foo(%3 dst: ptr<f32>, %4 src: ptr<ptr<f32>>, %5 a: i32, %6 n: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %9 z: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(div<u64, by_zero=ub>(const<u64>(16), const<u64>(4))));
// DEFAULT-NEXT:         let %10 m: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         for %24
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:             condition: logical_and<bool>(lt<i32>(read<i32>(%8), read<i32>(%6)), ne<u64>(and<u64>(add<u64, overflow=wrap>(ptr_to_int<u64, reason=explicit>(read<ptr<f32>>(%3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%8)))), widen<u64, reason=usual_arith>(read<u32>(%10))), const<u64>(0)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %32: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%33));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %11 t: f32 [storage=automatic] = read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%4), const<i32>(0)))), read<i32>(%8))));
// DEFAULT-NEXT:                     for %25
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%7), read<i32>(%5))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %34: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                             let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%7, read<i32>(%35));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             let %36: f32 [synthetic] = read<f32>(%11);
// DEFAULT-NEXT:                             let %37: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%36), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%4), read<i32>(%7)))), read<i32>(%8)))));
// DEFAULT-NEXT:                             write<f32>(%11, read<f32>(%37));
// DEFAULT-NEXT:                     write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%3), read<i32>(%8))), read<f32>(%11));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %26
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), sub<i32, overflow=ub>(read<i32>(%6), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), read<i32>(%9)), const<i32>(1))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %38: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %39: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%38), mul<i32, overflow=ub>(const<i32>(4), read<i32>(%9)));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%39));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %12 t0: vector<f32, 4> [storage=automatic] = read<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%4), const<i32>(0)))), read<i32>(%8)), mul<i32, overflow=ub>(const<i32>(0), read<i32>(%9))))));
// DEFAULT-NEXT:                     let %13 t1: vector<f32, 4> [storage=automatic] = read<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%4), const<i32>(0)))), read<i32>(%8)), mul<i32, overflow=ub>(const<i32>(1), read<i32>(%9))))));
// DEFAULT-NEXT:                     let %14 t2: vector<f32, 4> [storage=automatic] = read<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%4), const<i32>(0)))), read<i32>(%8)), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%9))))));
// DEFAULT-NEXT:                     let %15 t3: vector<f32, 4> [storage=automatic] = read<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%4), const<i32>(0)))), read<i32>(%8)), mul<i32, overflow=ub>(const<i32>(3), read<i32>(%9))))));
// DEFAULT-NEXT:                     for %27
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%7), read<i32>(%5))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %40: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                             let %41: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%40), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%7, read<i32>(%41));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %42: vector<f32, 4> [synthetic] = read<vector<f32, 4>>(%12);
// DEFAULT-NEXT:                                 let %43: vector<f32, 4> [synthetic] = add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%42), read<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%4), read<i32>(%7)))), read<i32>(%8)), mul<i32, overflow=ub>(const<i32>(0), read<i32>(%9)))))));
// DEFAULT-NEXT:                                 write<vector<f32, 4>>(%12, read<vector<f32, 4>>(%43));
// DEFAULT-NEXT:                                 let %44: vector<f32, 4> [synthetic] = read<vector<f32, 4>>(%13);
// DEFAULT-NEXT:                                 let %45: vector<f32, 4> [synthetic] = add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%44), read<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%4), read<i32>(%7)))), read<i32>(%8)), mul<i32, overflow=ub>(const<i32>(1), read<i32>(%9)))))));
// DEFAULT-NEXT:                                 write<vector<f32, 4>>(%13, read<vector<f32, 4>>(%45));
// DEFAULT-NEXT:                                 let %46: vector<f32, 4> [synthetic] = read<vector<f32, 4>>(%14);
// DEFAULT-NEXT:                                 let %47: vector<f32, 4> [synthetic] = add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%46), read<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%4), read<i32>(%7)))), read<i32>(%8)), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%9)))))));
// DEFAULT-NEXT:                                 write<vector<f32, 4>>(%14, read<vector<f32, 4>>(%47));
// DEFAULT-NEXT:                                 let %48: vector<f32, 4> [synthetic] = read<vector<f32, 4>>(%15);
// DEFAULT-NEXT:                                 let %49: vector<f32, 4> [synthetic] = add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%48), read<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%4), read<i32>(%7)))), read<i32>(%8)), mul<i32, overflow=ub>(const<i32>(3), read<i32>(%9)))))));
// DEFAULT-NEXT:                                 write<vector<f32, 4>>(%15, read<vector<f32, 4>>(%49));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     write<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%3), read<i32>(%8)), mul<i32, overflow=ub>(const<i32>(0), read<i32>(%9))))), read<vector<f32, 4>>(%12));
// DEFAULT-NEXT:                     write<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%3), read<i32>(%8)), mul<i32, overflow=ub>(const<i32>(1), read<i32>(%9))))), read<vector<f32, 4>>(%13));
// DEFAULT-NEXT:                     write<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%3), read<i32>(%8)), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%9))))), read<vector<f32, 4>>(%14));
// DEFAULT-NEXT:                     write<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%3), read<i32>(%8)), mul<i32, overflow=ub>(const<i32>(3), read<i32>(%9))))), read<vector<f32, 4>>(%15));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %28
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), read<i32>(%6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %50: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %51: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%50), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%51));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %16 t: f32 [storage=automatic] = read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%4), const<i32>(0)))), read<i32>(%8))));
// DEFAULT-NEXT:                     for %29
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%7), read<i32>(%5))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %52: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                             let %53: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%52), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%7, read<i32>(%53));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             let %54: f32 [synthetic] = read<f32>(%16);
// DEFAULT-NEXT:                             let %55: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%54), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%4), read<i32>(%7)))), read<i32>(%8)))));
// DEFAULT-NEXT:                             write<f32>(%16, read<f32>(%55));
// DEFAULT-NEXT:                     write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%3), read<i32>(%8))), read<f32>(%16));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %19 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %20 dst: ptr<f32> [storage=automatic];
// DEFAULT-NEXT:         let %21 src: array<ptr<f32>, 2> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %22 cptr: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%22, pointer_cast<ptr<i8>, reason=explicit>(array_decay<ptr<f32>, length=Some(64)>(%17)));
// DEFAULT-NEXT:         let %56: ptr<i8> [synthetic] = read<ptr<i8>>(%22);
// DEFAULT-NEXT:         let %57: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%56), and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(neg<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(array_decay<ptr<f32>, length=Some(64)>(%17)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%22, read<ptr<i8>>(%57));
// DEFAULT-NEXT:         write<ptr<f32>>(%20, pointer_cast<ptr<f32>, reason=explicit>(read<ptr<i8>>(%22)));
// DEFAULT-NEXT:         write<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(2)>(%21), const<i32>(0))), ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%20), const<i32>(16)));
// DEFAULT-NEXT:         write<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(2)>(%21), const<i32>(1))), ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%20), const<i32>(32)));
// DEFAULT-NEXT:         for %30
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%19), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %58: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %59: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%58), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%59));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(2)>(%21), const<i32>(0)))), read<i32>(%19))), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%19)), mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(11)), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%19)))));
// DEFAULT-NEXT:                     write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(2)>(%21), const<i32>(1)))), read<i32>(%19))), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%19)), mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(12)), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%19)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f32>, ptr<ptr<f32>>, i32, i32) -> void>(%2, read<ptr<f32>>(%20), array_decay<ptr<ptr<f32>>, length=Some(2)>(%21), const<i32>(2), const<i32>(16));
// DEFAULT-NEXT:         for %31
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%19), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %60: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %61: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%60), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%61));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %23 e: f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%19)), mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(11)), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%19)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%19))), mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(12)), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%19))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%20), read<i32>(%19)))), read<f32>(%23))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
