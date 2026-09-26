/* { dg-require-stack-size "0x80100" } */

/* Like pr28982a.c, but with the spill slots outside the range of
   a single sp-based load on ARM.  This test tests for cases where
   the addresses in the base and index reloads require further reloads.  */
#define NITER 4
#define NVARS 20
#define MULTI(X)                                                               \
  X(0), X(1), X(2), X(3), X(4), X(5), X(6), X(7), X(8), X(9), X(10), X(11),    \
      X(12), X(13), X(14), X(15), X(16), X(17), X(18), X(19)

#define DECLAREI(INDEX) inc##INDEX = incs[INDEX]
#define DECLAREF(INDEX) *ptr##INDEX = ptrs[INDEX], result##INDEX = 0
#define LOOP(INDEX)     result##INDEX += *ptr##INDEX, ptr##INDEX += inc##INDEX
#define COPYOUT(INDEX)  results[INDEX] = result##INDEX

float *ptrs[NVARS];
float  results[NVARS];
int    incs[NVARS];

struct big {
  int i[0x10000];
};
void __attribute__((noinline)) bar(struct big b) { incs[0] += b.i[0]; }

void __attribute__((noinline)) foo(int n) {
  struct big b = {};
  int        MULTI(DECLAREI);
  float      MULTI(DECLAREF);
  while (n--)
    MULTI(LOOP);
  MULTI(COPYOUT);
  bar(b);
}

float input[NITER * NVARS];

int main(void) {
  int i;

  for (i = 0; i < NVARS; i++)
    ptrs[i] = input + i, incs[i] = i;
  for (i = 0; i < NITER * NVARS; i++)
    input[i] = i;
  foo(NITER);
  for (i = 0; i < NVARS; i++)
    if (results[i] != i * NITER * (NITER + 1) / 2)
      return 1;
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
// DEFAULT-NEXT:     type @type0 big = struct {
// DEFAULT-NEXT:         field0 i: array<i32, 65536>;
// DEFAULT-NEXT:     } [size=262144, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %0 ptrs: array<ptr<f32>, 20> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 results: array<f32, 20> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 incs: array<i32, 20> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %69 input: array<f32, 80> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @bar(%5 b: @type0) -> void [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %76: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(0));
// DEFAULT-NEXT:         let %77: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%76)));
// DEFAULT-NEXT:         let %78: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%77), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(65536)>(field0(%5)), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%76)), read<i32>(%78));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @foo(%7 n: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 b: @type0 [storage=automatic] = aggregate<@type0, zero_fill=true>();
// DEFAULT-NEXT:         let %9 inc0: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(0))));
// DEFAULT-NEXT:         let %10 inc1: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(1))));
// DEFAULT-NEXT:         let %11 inc2: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(2))));
// DEFAULT-NEXT:         let %12 inc3: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(3))));
// DEFAULT-NEXT:         let %13 inc4: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(4))));
// DEFAULT-NEXT:         let %14 inc5: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(5))));
// DEFAULT-NEXT:         let %15 inc6: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(6))));
// DEFAULT-NEXT:         let %16 inc7: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(7))));
// DEFAULT-NEXT:         let %17 inc8: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(8))));
// DEFAULT-NEXT:         let %18 inc9: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(9))));
// DEFAULT-NEXT:         let %19 inc10: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(10))));
// DEFAULT-NEXT:         let %20 inc11: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(11))));
// DEFAULT-NEXT:         let %21 inc12: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(12))));
// DEFAULT-NEXT:         let %22 inc13: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(13))));
// DEFAULT-NEXT:         let %23 inc14: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(14))));
// DEFAULT-NEXT:         let %24 inc15: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(15))));
// DEFAULT-NEXT:         let %25 inc16: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(16))));
// DEFAULT-NEXT:         let %26 inc17: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(17))));
// DEFAULT-NEXT:         let %27 inc18: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(18))));
// DEFAULT-NEXT:         let %28 inc19: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(19))));
// DEFAULT-NEXT:         let %29 ptr0: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(0))));
// DEFAULT-NEXT:         let %30 result0: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %31 ptr1: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(1))));
// DEFAULT-NEXT:         let %32 result1: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %33 ptr2: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(2))));
// DEFAULT-NEXT:         let %34 result2: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %35 ptr3: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(3))));
// DEFAULT-NEXT:         let %36 result3: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %37 ptr4: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(4))));
// DEFAULT-NEXT:         let %38 result4: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %39 ptr5: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(5))));
// DEFAULT-NEXT:         let %40 result5: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %41 ptr6: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(6))));
// DEFAULT-NEXT:         let %42 result6: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %43 ptr7: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(7))));
// DEFAULT-NEXT:         let %44 result7: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %45 ptr8: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(8))));
// DEFAULT-NEXT:         let %46 result8: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %47 ptr9: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(9))));
// DEFAULT-NEXT:         let %48 result9: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %49 ptr10: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(10))));
// DEFAULT-NEXT:         let %50 result10: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %51 ptr11: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(11))));
// DEFAULT-NEXT:         let %52 result11: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %53 ptr12: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(12))));
// DEFAULT-NEXT:         let %54 result12: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %55 ptr13: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(13))));
// DEFAULT-NEXT:         let %56 result13: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %57 ptr14: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(14))));
// DEFAULT-NEXT:         let %58 result14: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %59 ptr15: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(15))));
// DEFAULT-NEXT:         let %60 result15: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %61 ptr16: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(16))));
// DEFAULT-NEXT:         let %62 result16: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %63 ptr17: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(17))));
// DEFAULT-NEXT:         let %64 result17: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %65 ptr18: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(18))));
// DEFAULT-NEXT:         let %66 result18: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %67 ptr19: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(19))));
// DEFAULT-NEXT:         let %68 result19: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         while %72 {
// DEFAULT-NEXT:             let %79: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:             let %80: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%79), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%7, read<i32>(%80));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%79), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             let %81: f32 [synthetic] = read<f32>(%30);
// DEFAULT-NEXT:             let %82: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%81), read<f32>(deref(read<ptr<f32>>(%29))));
// DEFAULT-NEXT:             write<f32>(%30, read<f32>(%82));
// DEFAULT-NEXT:             let %83: ptr<f32> [synthetic] = read<ptr<f32>>(%29);
// DEFAULT-NEXT:             let %84: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%83), read<i32>(%9));
// DEFAULT-NEXT:             write<ptr<f32>>(%29, read<ptr<f32>>(%84));
// DEFAULT-NEXT:             let %85: f32 [synthetic] = read<f32>(%32);
// DEFAULT-NEXT:             let %86: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%85), read<f32>(deref(read<ptr<f32>>(%31))));
// DEFAULT-NEXT:             write<f32>(%32, read<f32>(%86));
// DEFAULT-NEXT:             let %87: ptr<f32> [synthetic] = read<ptr<f32>>(%31);
// DEFAULT-NEXT:             let %88: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%87), read<i32>(%10));
// DEFAULT-NEXT:             write<ptr<f32>>(%31, read<ptr<f32>>(%88));
// DEFAULT-NEXT:             let %89: f32 [synthetic] = read<f32>(%34);
// DEFAULT-NEXT:             let %90: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%89), read<f32>(deref(read<ptr<f32>>(%33))));
// DEFAULT-NEXT:             write<f32>(%34, read<f32>(%90));
// DEFAULT-NEXT:             let %91: ptr<f32> [synthetic] = read<ptr<f32>>(%33);
// DEFAULT-NEXT:             let %92: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%91), read<i32>(%11));
// DEFAULT-NEXT:             write<ptr<f32>>(%33, read<ptr<f32>>(%92));
// DEFAULT-NEXT:             let %93: f32 [synthetic] = read<f32>(%36);
// DEFAULT-NEXT:             let %94: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%93), read<f32>(deref(read<ptr<f32>>(%35))));
// DEFAULT-NEXT:             write<f32>(%36, read<f32>(%94));
// DEFAULT-NEXT:             let %95: ptr<f32> [synthetic] = read<ptr<f32>>(%35);
// DEFAULT-NEXT:             let %96: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%95), read<i32>(%12));
// DEFAULT-NEXT:             write<ptr<f32>>(%35, read<ptr<f32>>(%96));
// DEFAULT-NEXT:             let %97: f32 [synthetic] = read<f32>(%38);
// DEFAULT-NEXT:             let %98: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%97), read<f32>(deref(read<ptr<f32>>(%37))));
// DEFAULT-NEXT:             write<f32>(%38, read<f32>(%98));
// DEFAULT-NEXT:             let %99: ptr<f32> [synthetic] = read<ptr<f32>>(%37);
// DEFAULT-NEXT:             let %100: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%99), read<i32>(%13));
// DEFAULT-NEXT:             write<ptr<f32>>(%37, read<ptr<f32>>(%100));
// DEFAULT-NEXT:             let %101: f32 [synthetic] = read<f32>(%40);
// DEFAULT-NEXT:             let %102: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%101), read<f32>(deref(read<ptr<f32>>(%39))));
// DEFAULT-NEXT:             write<f32>(%40, read<f32>(%102));
// DEFAULT-NEXT:             let %103: ptr<f32> [synthetic] = read<ptr<f32>>(%39);
// DEFAULT-NEXT:             let %104: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%103), read<i32>(%14));
// DEFAULT-NEXT:             write<ptr<f32>>(%39, read<ptr<f32>>(%104));
// DEFAULT-NEXT:             let %105: f32 [synthetic] = read<f32>(%42);
// DEFAULT-NEXT:             let %106: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%105), read<f32>(deref(read<ptr<f32>>(%41))));
// DEFAULT-NEXT:             write<f32>(%42, read<f32>(%106));
// DEFAULT-NEXT:             let %107: ptr<f32> [synthetic] = read<ptr<f32>>(%41);
// DEFAULT-NEXT:             let %108: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%107), read<i32>(%15));
// DEFAULT-NEXT:             write<ptr<f32>>(%41, read<ptr<f32>>(%108));
// DEFAULT-NEXT:             let %109: f32 [synthetic] = read<f32>(%44);
// DEFAULT-NEXT:             let %110: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%109), read<f32>(deref(read<ptr<f32>>(%43))));
// DEFAULT-NEXT:             write<f32>(%44, read<f32>(%110));
// DEFAULT-NEXT:             let %111: ptr<f32> [synthetic] = read<ptr<f32>>(%43);
// DEFAULT-NEXT:             let %112: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%111), read<i32>(%16));
// DEFAULT-NEXT:             write<ptr<f32>>(%43, read<ptr<f32>>(%112));
// DEFAULT-NEXT:             let %113: f32 [synthetic] = read<f32>(%46);
// DEFAULT-NEXT:             let %114: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%113), read<f32>(deref(read<ptr<f32>>(%45))));
// DEFAULT-NEXT:             write<f32>(%46, read<f32>(%114));
// DEFAULT-NEXT:             let %115: ptr<f32> [synthetic] = read<ptr<f32>>(%45);
// DEFAULT-NEXT:             let %116: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%115), read<i32>(%17));
// DEFAULT-NEXT:             write<ptr<f32>>(%45, read<ptr<f32>>(%116));
// DEFAULT-NEXT:             let %117: f32 [synthetic] = read<f32>(%48);
// DEFAULT-NEXT:             let %118: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%117), read<f32>(deref(read<ptr<f32>>(%47))));
// DEFAULT-NEXT:             write<f32>(%48, read<f32>(%118));
// DEFAULT-NEXT:             let %119: ptr<f32> [synthetic] = read<ptr<f32>>(%47);
// DEFAULT-NEXT:             let %120: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%119), read<i32>(%18));
// DEFAULT-NEXT:             write<ptr<f32>>(%47, read<ptr<f32>>(%120));
// DEFAULT-NEXT:             let %121: f32 [synthetic] = read<f32>(%50);
// DEFAULT-NEXT:             let %122: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%121), read<f32>(deref(read<ptr<f32>>(%49))));
// DEFAULT-NEXT:             write<f32>(%50, read<f32>(%122));
// DEFAULT-NEXT:             let %123: ptr<f32> [synthetic] = read<ptr<f32>>(%49);
// DEFAULT-NEXT:             let %124: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%123), read<i32>(%19));
// DEFAULT-NEXT:             write<ptr<f32>>(%49, read<ptr<f32>>(%124));
// DEFAULT-NEXT:             let %125: f32 [synthetic] = read<f32>(%52);
// DEFAULT-NEXT:             let %126: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%125), read<f32>(deref(read<ptr<f32>>(%51))));
// DEFAULT-NEXT:             write<f32>(%52, read<f32>(%126));
// DEFAULT-NEXT:             let %127: ptr<f32> [synthetic] = read<ptr<f32>>(%51);
// DEFAULT-NEXT:             let %128: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%127), read<i32>(%20));
// DEFAULT-NEXT:             write<ptr<f32>>(%51, read<ptr<f32>>(%128));
// DEFAULT-NEXT:             let %129: f32 [synthetic] = read<f32>(%54);
// DEFAULT-NEXT:             let %130: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%129), read<f32>(deref(read<ptr<f32>>(%53))));
// DEFAULT-NEXT:             write<f32>(%54, read<f32>(%130));
// DEFAULT-NEXT:             let %131: ptr<f32> [synthetic] = read<ptr<f32>>(%53);
// DEFAULT-NEXT:             let %132: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%131), read<i32>(%21));
// DEFAULT-NEXT:             write<ptr<f32>>(%53, read<ptr<f32>>(%132));
// DEFAULT-NEXT:             let %133: f32 [synthetic] = read<f32>(%56);
// DEFAULT-NEXT:             let %134: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%133), read<f32>(deref(read<ptr<f32>>(%55))));
// DEFAULT-NEXT:             write<f32>(%56, read<f32>(%134));
// DEFAULT-NEXT:             let %135: ptr<f32> [synthetic] = read<ptr<f32>>(%55);
// DEFAULT-NEXT:             let %136: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%135), read<i32>(%22));
// DEFAULT-NEXT:             write<ptr<f32>>(%55, read<ptr<f32>>(%136));
// DEFAULT-NEXT:             let %137: f32 [synthetic] = read<f32>(%58);
// DEFAULT-NEXT:             let %138: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%137), read<f32>(deref(read<ptr<f32>>(%57))));
// DEFAULT-NEXT:             write<f32>(%58, read<f32>(%138));
// DEFAULT-NEXT:             let %139: ptr<f32> [synthetic] = read<ptr<f32>>(%57);
// DEFAULT-NEXT:             let %140: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%139), read<i32>(%23));
// DEFAULT-NEXT:             write<ptr<f32>>(%57, read<ptr<f32>>(%140));
// DEFAULT-NEXT:             let %141: f32 [synthetic] = read<f32>(%60);
// DEFAULT-NEXT:             let %142: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%141), read<f32>(deref(read<ptr<f32>>(%59))));
// DEFAULT-NEXT:             write<f32>(%60, read<f32>(%142));
// DEFAULT-NEXT:             let %143: ptr<f32> [synthetic] = read<ptr<f32>>(%59);
// DEFAULT-NEXT:             let %144: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%143), read<i32>(%24));
// DEFAULT-NEXT:             write<ptr<f32>>(%59, read<ptr<f32>>(%144));
// DEFAULT-NEXT:             let %145: f32 [synthetic] = read<f32>(%62);
// DEFAULT-NEXT:             let %146: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%145), read<f32>(deref(read<ptr<f32>>(%61))));
// DEFAULT-NEXT:             write<f32>(%62, read<f32>(%146));
// DEFAULT-NEXT:             let %147: ptr<f32> [synthetic] = read<ptr<f32>>(%61);
// DEFAULT-NEXT:             let %148: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%147), read<i32>(%25));
// DEFAULT-NEXT:             write<ptr<f32>>(%61, read<ptr<f32>>(%148));
// DEFAULT-NEXT:             let %149: f32 [synthetic] = read<f32>(%64);
// DEFAULT-NEXT:             let %150: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%149), read<f32>(deref(read<ptr<f32>>(%63))));
// DEFAULT-NEXT:             write<f32>(%64, read<f32>(%150));
// DEFAULT-NEXT:             let %151: ptr<f32> [synthetic] = read<ptr<f32>>(%63);
// DEFAULT-NEXT:             let %152: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%151), read<i32>(%26));
// DEFAULT-NEXT:             write<ptr<f32>>(%63, read<ptr<f32>>(%152));
// DEFAULT-NEXT:             let %153: f32 [synthetic] = read<f32>(%66);
// DEFAULT-NEXT:             let %154: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%153), read<f32>(deref(read<ptr<f32>>(%65))));
// DEFAULT-NEXT:             write<f32>(%66, read<f32>(%154));
// DEFAULT-NEXT:             let %155: ptr<f32> [synthetic] = read<ptr<f32>>(%65);
// DEFAULT-NEXT:             let %156: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%155), read<i32>(%27));
// DEFAULT-NEXT:             write<ptr<f32>>(%65, read<ptr<f32>>(%156));
// DEFAULT-NEXT:             let %157: f32 [synthetic] = read<f32>(%68);
// DEFAULT-NEXT:             let %158: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%157), read<f32>(deref(read<ptr<f32>>(%67))));
// DEFAULT-NEXT:             write<f32>(%68, read<f32>(%158));
// DEFAULT-NEXT:             let %159: ptr<f32> [synthetic] = read<ptr<f32>>(%67);
// DEFAULT-NEXT:             let %160: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%159), read<i32>(%28));
// DEFAULT-NEXT:             write<ptr<f32>>(%67, read<ptr<f32>>(%160));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(0))), read<f32>(%30));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(1))), read<f32>(%32));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(2))), read<f32>(%34));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(3))), read<f32>(%36));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(4))), read<f32>(%38));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(5))), read<f32>(%40));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(6))), read<f32>(%42));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(7))), read<f32>(%44));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(8))), read<f32>(%46));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(9))), read<f32>(%48));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(10))), read<f32>(%50));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(11))), read<f32>(%52));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(12))), read<f32>(%54));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(13))), read<f32>(%56));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(14))), read<f32>(%58));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(15))), read<f32>(%60));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(16))), read<f32>(%62));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(17))), read<f32>(%64));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(18))), read<f32>(%66));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(19))), read<f32>(%68));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void, abi=sysv64(native_c) -> void>(%4, copy<@type0, reason=arg>(read<@type0>(%8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %70 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %71 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %73
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%71, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%71), const<i32>(20))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %161: i32 [synthetic] = read<i32>(%71);
// DEFAULT-NEXT:                 let %162: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%161), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%71, read<i32>(%162));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), read<i32>(%71))), ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(80)>(%69), read<i32>(%71)));
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), read<i32>(%71))), read<i32>(%71));
// DEFAULT-NEXT:         for %74
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%71, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%71), mul<i32, overflow=ub>(const<i32>(4), const<i32>(20)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %163: i32 [synthetic] = read<i32>(%71);
// DEFAULT-NEXT:                 let %164: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%163), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%71, read<i32>(%164));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(80)>(%69), read<i32>(%71))), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%71)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%6, const<i32>(4));
// DEFAULT-NEXT:         for %75
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%71, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%71), const<i32>(20))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %165: i32 [synthetic] = read<i32>(%71);
// DEFAULT-NEXT:                 let %166: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%165), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%71, read<i32>(%166));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), read<i32>(%71)))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%71), const<i32>(4)), add<i32, overflow=ub>(const<i32>(4), const<i32>(1))), const<i32>(2))))
// DEFAULT-NEXT:                     return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
