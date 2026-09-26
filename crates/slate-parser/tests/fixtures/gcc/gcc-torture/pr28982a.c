/* PR rtl-optimization/28982.  Function foo() does the equivalent of:

     float tmp_results[NVARS];
     for (int i = 0; i < NVARS; i++)
       {
         int inc = incs[i];
         float *ptr = ptrs[i], result = 0;
         for (int j = 0; j < n; j++)
           result += *ptr, ptr += inc;
         tmp_results[i] = result;
       }
     memcpy (results, tmp_results, sizeof (results));

   but without the outermost loop.  The idea is to create high register
   pressure and ensure that some INC and PTR variables are spilled.

   On ARM targets, sequences like "result += *ptr, ptr += inc" can
   usually be implemented using (mem (post_modify ...)), and we do
   indeed create such MEMs before reload for this testcase.  However,
   (post_modify ...) is not a valid address for coprocessor loads, so
   for -mfloat-abi=softfp, reload reloads the POST_MODIFY into a base
   register.  GCC did not deal correctly with cases where the base and
   index of the POST_MODIFY are themselves reloaded.  */
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

void __attribute__((noinline)) foo(int n) {
  int   MULTI(DECLAREI);
  float MULTI(DECLAREF);
  while (n--)
    MULTI(LOOP);
  MULTI(COPYOUT);
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
// DEFAULT-NEXT:     global %0 ptrs: array<ptr<f32>, 20> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 results: array<f32, 20> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 incs: array<i32, 20> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %65 input: array<f32, 80> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 n: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 inc0: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(0))));
// DEFAULT-NEXT:         let %6 inc1: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(1))));
// DEFAULT-NEXT:         let %7 inc2: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(2))));
// DEFAULT-NEXT:         let %8 inc3: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(3))));
// DEFAULT-NEXT:         let %9 inc4: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(4))));
// DEFAULT-NEXT:         let %10 inc5: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(5))));
// DEFAULT-NEXT:         let %11 inc6: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(6))));
// DEFAULT-NEXT:         let %12 inc7: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(7))));
// DEFAULT-NEXT:         let %13 inc8: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(8))));
// DEFAULT-NEXT:         let %14 inc9: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(9))));
// DEFAULT-NEXT:         let %15 inc10: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(10))));
// DEFAULT-NEXT:         let %16 inc11: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(11))));
// DEFAULT-NEXT:         let %17 inc12: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(12))));
// DEFAULT-NEXT:         let %18 inc13: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(13))));
// DEFAULT-NEXT:         let %19 inc14: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(14))));
// DEFAULT-NEXT:         let %20 inc15: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(15))));
// DEFAULT-NEXT:         let %21 inc16: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(16))));
// DEFAULT-NEXT:         let %22 inc17: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(17))));
// DEFAULT-NEXT:         let %23 inc18: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(18))));
// DEFAULT-NEXT:         let %24 inc19: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), const<i32>(19))));
// DEFAULT-NEXT:         let %25 ptr0: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(0))));
// DEFAULT-NEXT:         let %26 result0: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %27 ptr1: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(1))));
// DEFAULT-NEXT:         let %28 result1: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %29 ptr2: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(2))));
// DEFAULT-NEXT:         let %30 result2: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %31 ptr3: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(3))));
// DEFAULT-NEXT:         let %32 result3: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %33 ptr4: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(4))));
// DEFAULT-NEXT:         let %34 result4: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %35 ptr5: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(5))));
// DEFAULT-NEXT:         let %36 result5: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %37 ptr6: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(6))));
// DEFAULT-NEXT:         let %38 result6: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %39 ptr7: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(7))));
// DEFAULT-NEXT:         let %40 result7: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %41 ptr8: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(8))));
// DEFAULT-NEXT:         let %42 result8: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %43 ptr9: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(9))));
// DEFAULT-NEXT:         let %44 result9: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %45 ptr10: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(10))));
// DEFAULT-NEXT:         let %46 result10: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %47 ptr11: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(11))));
// DEFAULT-NEXT:         let %48 result11: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %49 ptr12: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(12))));
// DEFAULT-NEXT:         let %50 result12: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %51 ptr13: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(13))));
// DEFAULT-NEXT:         let %52 result13: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %53 ptr14: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(14))));
// DEFAULT-NEXT:         let %54 result14: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %55 ptr15: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(15))));
// DEFAULT-NEXT:         let %56 result15: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %57 ptr16: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(16))));
// DEFAULT-NEXT:         let %58 result16: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %59 ptr17: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(17))));
// DEFAULT-NEXT:         let %60 result17: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %61 ptr18: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(18))));
// DEFAULT-NEXT:         let %62 result18: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         let %63 ptr19: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), const<i32>(19))));
// DEFAULT-NEXT:         let %64 result19: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:         while %68 {
// DEFAULT-NEXT:             let %72: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:             let %73: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%72), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%4, read<i32>(%73));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%72), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             let %74: f32 [synthetic] = read<f32>(%26);
// DEFAULT-NEXT:             let %75: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%74), read<f32>(deref(read<ptr<f32>>(%25))));
// DEFAULT-NEXT:             write<f32>(%26, read<f32>(%75));
// DEFAULT-NEXT:             let %76: ptr<f32> [synthetic] = read<ptr<f32>>(%25);
// DEFAULT-NEXT:             let %77: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%76), read<i32>(%5));
// DEFAULT-NEXT:             write<ptr<f32>>(%25, read<ptr<f32>>(%77));
// DEFAULT-NEXT:             let %78: f32 [synthetic] = read<f32>(%28);
// DEFAULT-NEXT:             let %79: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%78), read<f32>(deref(read<ptr<f32>>(%27))));
// DEFAULT-NEXT:             write<f32>(%28, read<f32>(%79));
// DEFAULT-NEXT:             let %80: ptr<f32> [synthetic] = read<ptr<f32>>(%27);
// DEFAULT-NEXT:             let %81: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%80), read<i32>(%6));
// DEFAULT-NEXT:             write<ptr<f32>>(%27, read<ptr<f32>>(%81));
// DEFAULT-NEXT:             let %82: f32 [synthetic] = read<f32>(%30);
// DEFAULT-NEXT:             let %83: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%82), read<f32>(deref(read<ptr<f32>>(%29))));
// DEFAULT-NEXT:             write<f32>(%30, read<f32>(%83));
// DEFAULT-NEXT:             let %84: ptr<f32> [synthetic] = read<ptr<f32>>(%29);
// DEFAULT-NEXT:             let %85: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%84), read<i32>(%7));
// DEFAULT-NEXT:             write<ptr<f32>>(%29, read<ptr<f32>>(%85));
// DEFAULT-NEXT:             let %86: f32 [synthetic] = read<f32>(%32);
// DEFAULT-NEXT:             let %87: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%86), read<f32>(deref(read<ptr<f32>>(%31))));
// DEFAULT-NEXT:             write<f32>(%32, read<f32>(%87));
// DEFAULT-NEXT:             let %88: ptr<f32> [synthetic] = read<ptr<f32>>(%31);
// DEFAULT-NEXT:             let %89: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%88), read<i32>(%8));
// DEFAULT-NEXT:             write<ptr<f32>>(%31, read<ptr<f32>>(%89));
// DEFAULT-NEXT:             let %90: f32 [synthetic] = read<f32>(%34);
// DEFAULT-NEXT:             let %91: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%90), read<f32>(deref(read<ptr<f32>>(%33))));
// DEFAULT-NEXT:             write<f32>(%34, read<f32>(%91));
// DEFAULT-NEXT:             let %92: ptr<f32> [synthetic] = read<ptr<f32>>(%33);
// DEFAULT-NEXT:             let %93: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%92), read<i32>(%9));
// DEFAULT-NEXT:             write<ptr<f32>>(%33, read<ptr<f32>>(%93));
// DEFAULT-NEXT:             let %94: f32 [synthetic] = read<f32>(%36);
// DEFAULT-NEXT:             let %95: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%94), read<f32>(deref(read<ptr<f32>>(%35))));
// DEFAULT-NEXT:             write<f32>(%36, read<f32>(%95));
// DEFAULT-NEXT:             let %96: ptr<f32> [synthetic] = read<ptr<f32>>(%35);
// DEFAULT-NEXT:             let %97: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%96), read<i32>(%10));
// DEFAULT-NEXT:             write<ptr<f32>>(%35, read<ptr<f32>>(%97));
// DEFAULT-NEXT:             let %98: f32 [synthetic] = read<f32>(%38);
// DEFAULT-NEXT:             let %99: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%98), read<f32>(deref(read<ptr<f32>>(%37))));
// DEFAULT-NEXT:             write<f32>(%38, read<f32>(%99));
// DEFAULT-NEXT:             let %100: ptr<f32> [synthetic] = read<ptr<f32>>(%37);
// DEFAULT-NEXT:             let %101: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%100), read<i32>(%11));
// DEFAULT-NEXT:             write<ptr<f32>>(%37, read<ptr<f32>>(%101));
// DEFAULT-NEXT:             let %102: f32 [synthetic] = read<f32>(%40);
// DEFAULT-NEXT:             let %103: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%102), read<f32>(deref(read<ptr<f32>>(%39))));
// DEFAULT-NEXT:             write<f32>(%40, read<f32>(%103));
// DEFAULT-NEXT:             let %104: ptr<f32> [synthetic] = read<ptr<f32>>(%39);
// DEFAULT-NEXT:             let %105: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%104), read<i32>(%12));
// DEFAULT-NEXT:             write<ptr<f32>>(%39, read<ptr<f32>>(%105));
// DEFAULT-NEXT:             let %106: f32 [synthetic] = read<f32>(%42);
// DEFAULT-NEXT:             let %107: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%106), read<f32>(deref(read<ptr<f32>>(%41))));
// DEFAULT-NEXT:             write<f32>(%42, read<f32>(%107));
// DEFAULT-NEXT:             let %108: ptr<f32> [synthetic] = read<ptr<f32>>(%41);
// DEFAULT-NEXT:             let %109: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%108), read<i32>(%13));
// DEFAULT-NEXT:             write<ptr<f32>>(%41, read<ptr<f32>>(%109));
// DEFAULT-NEXT:             let %110: f32 [synthetic] = read<f32>(%44);
// DEFAULT-NEXT:             let %111: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%110), read<f32>(deref(read<ptr<f32>>(%43))));
// DEFAULT-NEXT:             write<f32>(%44, read<f32>(%111));
// DEFAULT-NEXT:             let %112: ptr<f32> [synthetic] = read<ptr<f32>>(%43);
// DEFAULT-NEXT:             let %113: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%112), read<i32>(%14));
// DEFAULT-NEXT:             write<ptr<f32>>(%43, read<ptr<f32>>(%113));
// DEFAULT-NEXT:             let %114: f32 [synthetic] = read<f32>(%46);
// DEFAULT-NEXT:             let %115: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%114), read<f32>(deref(read<ptr<f32>>(%45))));
// DEFAULT-NEXT:             write<f32>(%46, read<f32>(%115));
// DEFAULT-NEXT:             let %116: ptr<f32> [synthetic] = read<ptr<f32>>(%45);
// DEFAULT-NEXT:             let %117: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%116), read<i32>(%15));
// DEFAULT-NEXT:             write<ptr<f32>>(%45, read<ptr<f32>>(%117));
// DEFAULT-NEXT:             let %118: f32 [synthetic] = read<f32>(%48);
// DEFAULT-NEXT:             let %119: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%118), read<f32>(deref(read<ptr<f32>>(%47))));
// DEFAULT-NEXT:             write<f32>(%48, read<f32>(%119));
// DEFAULT-NEXT:             let %120: ptr<f32> [synthetic] = read<ptr<f32>>(%47);
// DEFAULT-NEXT:             let %121: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%120), read<i32>(%16));
// DEFAULT-NEXT:             write<ptr<f32>>(%47, read<ptr<f32>>(%121));
// DEFAULT-NEXT:             let %122: f32 [synthetic] = read<f32>(%50);
// DEFAULT-NEXT:             let %123: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%122), read<f32>(deref(read<ptr<f32>>(%49))));
// DEFAULT-NEXT:             write<f32>(%50, read<f32>(%123));
// DEFAULT-NEXT:             let %124: ptr<f32> [synthetic] = read<ptr<f32>>(%49);
// DEFAULT-NEXT:             let %125: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%124), read<i32>(%17));
// DEFAULT-NEXT:             write<ptr<f32>>(%49, read<ptr<f32>>(%125));
// DEFAULT-NEXT:             let %126: f32 [synthetic] = read<f32>(%52);
// DEFAULT-NEXT:             let %127: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%126), read<f32>(deref(read<ptr<f32>>(%51))));
// DEFAULT-NEXT:             write<f32>(%52, read<f32>(%127));
// DEFAULT-NEXT:             let %128: ptr<f32> [synthetic] = read<ptr<f32>>(%51);
// DEFAULT-NEXT:             let %129: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%128), read<i32>(%18));
// DEFAULT-NEXT:             write<ptr<f32>>(%51, read<ptr<f32>>(%129));
// DEFAULT-NEXT:             let %130: f32 [synthetic] = read<f32>(%54);
// DEFAULT-NEXT:             let %131: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%130), read<f32>(deref(read<ptr<f32>>(%53))));
// DEFAULT-NEXT:             write<f32>(%54, read<f32>(%131));
// DEFAULT-NEXT:             let %132: ptr<f32> [synthetic] = read<ptr<f32>>(%53);
// DEFAULT-NEXT:             let %133: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%132), read<i32>(%19));
// DEFAULT-NEXT:             write<ptr<f32>>(%53, read<ptr<f32>>(%133));
// DEFAULT-NEXT:             let %134: f32 [synthetic] = read<f32>(%56);
// DEFAULT-NEXT:             let %135: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%134), read<f32>(deref(read<ptr<f32>>(%55))));
// DEFAULT-NEXT:             write<f32>(%56, read<f32>(%135));
// DEFAULT-NEXT:             let %136: ptr<f32> [synthetic] = read<ptr<f32>>(%55);
// DEFAULT-NEXT:             let %137: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%136), read<i32>(%20));
// DEFAULT-NEXT:             write<ptr<f32>>(%55, read<ptr<f32>>(%137));
// DEFAULT-NEXT:             let %138: f32 [synthetic] = read<f32>(%58);
// DEFAULT-NEXT:             let %139: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%138), read<f32>(deref(read<ptr<f32>>(%57))));
// DEFAULT-NEXT:             write<f32>(%58, read<f32>(%139));
// DEFAULT-NEXT:             let %140: ptr<f32> [synthetic] = read<ptr<f32>>(%57);
// DEFAULT-NEXT:             let %141: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%140), read<i32>(%21));
// DEFAULT-NEXT:             write<ptr<f32>>(%57, read<ptr<f32>>(%141));
// DEFAULT-NEXT:             let %142: f32 [synthetic] = read<f32>(%60);
// DEFAULT-NEXT:             let %143: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%142), read<f32>(deref(read<ptr<f32>>(%59))));
// DEFAULT-NEXT:             write<f32>(%60, read<f32>(%143));
// DEFAULT-NEXT:             let %144: ptr<f32> [synthetic] = read<ptr<f32>>(%59);
// DEFAULT-NEXT:             let %145: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%144), read<i32>(%22));
// DEFAULT-NEXT:             write<ptr<f32>>(%59, read<ptr<f32>>(%145));
// DEFAULT-NEXT:             let %146: f32 [synthetic] = read<f32>(%62);
// DEFAULT-NEXT:             let %147: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%146), read<f32>(deref(read<ptr<f32>>(%61))));
// DEFAULT-NEXT:             write<f32>(%62, read<f32>(%147));
// DEFAULT-NEXT:             let %148: ptr<f32> [synthetic] = read<ptr<f32>>(%61);
// DEFAULT-NEXT:             let %149: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%148), read<i32>(%23));
// DEFAULT-NEXT:             write<ptr<f32>>(%61, read<ptr<f32>>(%149));
// DEFAULT-NEXT:             let %150: f32 [synthetic] = read<f32>(%64);
// DEFAULT-NEXT:             let %151: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%150), read<f32>(deref(read<ptr<f32>>(%63))));
// DEFAULT-NEXT:             write<f32>(%64, read<f32>(%151));
// DEFAULT-NEXT:             let %152: ptr<f32> [synthetic] = read<ptr<f32>>(%63);
// DEFAULT-NEXT:             let %153: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%152), read<i32>(%24));
// DEFAULT-NEXT:             write<ptr<f32>>(%63, read<ptr<f32>>(%153));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(0))), read<f32>(%26));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(1))), read<f32>(%28));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(2))), read<f32>(%30));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(3))), read<f32>(%32));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(4))), read<f32>(%34));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(5))), read<f32>(%36));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(6))), read<f32>(%38));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(7))), read<f32>(%40));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(8))), read<f32>(%42));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(9))), read<f32>(%44));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(10))), read<f32>(%46));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(11))), read<f32>(%48));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(12))), read<f32>(%50));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(13))), read<f32>(%52));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(14))), read<f32>(%54));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(15))), read<f32>(%56));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(16))), read<f32>(%58));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(17))), read<f32>(%60));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(18))), read<f32>(%62));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), const<i32>(19))), read<f32>(%64));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %67 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %69
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%67, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%67), const<i32>(20))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %154: i32 [synthetic] = read<i32>(%67);
// DEFAULT-NEXT:                 let %155: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%154), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%67, read<i32>(%155));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%0), read<i32>(%67))), ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(80)>(%65), read<i32>(%67)));
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%2), read<i32>(%67))), read<i32>(%67));
// DEFAULT-NEXT:         for %70
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%67, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%67), mul<i32, overflow=ub>(const<i32>(4), const<i32>(20)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %156: i32 [synthetic] = read<i32>(%67);
// DEFAULT-NEXT:                 let %157: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%156), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%67, read<i32>(%157));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(80)>(%65), read<i32>(%67))), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%67)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(4));
// DEFAULT-NEXT:         for %71
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%67, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%67), const<i32>(20))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %158: i32 [synthetic] = read<i32>(%67);
// DEFAULT-NEXT:                 let %159: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%158), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%67, read<i32>(%159));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%1), read<i32>(%67)))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%67), const<i32>(4)), add<i32, overflow=ub>(const<i32>(4), const<i32>(1))), const<i32>(2))))
// DEFAULT-NEXT:                     return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
