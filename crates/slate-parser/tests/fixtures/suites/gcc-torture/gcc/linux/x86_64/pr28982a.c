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
// DEFAULT-NEXT:     global %[[VALUE_ptrs:[0-9]+]] ptrs: array<ptr<f32>, 20> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_results:[0-9]+]] results: array<f32, 20> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_incs:[0-9]+]] incs: array<i32, 20> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_input:[0-9]+]] input: array<f32, 80> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_inc0:[0-9]+]] inc0: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE_inc1:[0-9]+]] inc1: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_inc2:[0-9]+]] inc2: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(2))));
// DEFAULT-NEXT:         let %[[VALUE_inc3:[0-9]+]] inc3: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(3))));
// DEFAULT-NEXT:         let %[[VALUE_inc4:[0-9]+]] inc4: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(4))));
// DEFAULT-NEXT:         let %[[VALUE_inc5:[0-9]+]] inc5: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(5))));
// DEFAULT-NEXT:         let %[[VALUE_inc6:[0-9]+]] inc6: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(6))));
// DEFAULT-NEXT:         let %[[VALUE_inc7:[0-9]+]] inc7: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(7))));
// DEFAULT-NEXT:         let %[[VALUE_inc8:[0-9]+]] inc8: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(8))));
// DEFAULT-NEXT:         let %[[VALUE_inc9:[0-9]+]] inc9: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(9))));
// DEFAULT-NEXT:         let %[[VALUE_inc10:[0-9]+]] inc10: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(10))));
// DEFAULT-NEXT:         let %[[VALUE_inc11:[0-9]+]] inc11: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(11))));
// DEFAULT-NEXT:         let %[[VALUE_inc12:[0-9]+]] inc12: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(12))));
// DEFAULT-NEXT:         let %[[VALUE_inc13:[0-9]+]] inc13: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(13))));
// DEFAULT-NEXT:         let %[[VALUE_inc14:[0-9]+]] inc14: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(14))));
// DEFAULT-NEXT:         let %[[VALUE_inc15:[0-9]+]] inc15: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(15))));
// DEFAULT-NEXT:         let %[[VALUE_inc16:[0-9]+]] inc16: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(16))));
// DEFAULT-NEXT:         let %[[VALUE_inc17:[0-9]+]] inc17: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(17))));
// DEFAULT-NEXT:         let %[[VALUE_inc18:[0-9]+]] inc18: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(18))));
// DEFAULT-NEXT:         let %[[VALUE_inc19:[0-9]+]] inc19: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), const<i32>(19))));
// DEFAULT-NEXT:         let %[[VALUE_ptr0:[0-9]+]] ptr0: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE_result0:[0-9]+]] result0: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr1:[0-9]+]] ptr1: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_result1:[0-9]+]] result1: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr2:[0-9]+]] ptr2: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(2))));
// DEFAULT-NEXT:         let %[[VALUE_result2:[0-9]+]] result2: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr3:[0-9]+]] ptr3: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(3))));
// DEFAULT-NEXT:         let %[[VALUE_result3:[0-9]+]] result3: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr4:[0-9]+]] ptr4: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(4))));
// DEFAULT-NEXT:         let %[[VALUE_result4:[0-9]+]] result4: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr5:[0-9]+]] ptr5: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(5))));
// DEFAULT-NEXT:         let %[[VALUE_result5:[0-9]+]] result5: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr6:[0-9]+]] ptr6: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(6))));
// DEFAULT-NEXT:         let %[[VALUE_result6:[0-9]+]] result6: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr7:[0-9]+]] ptr7: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(7))));
// DEFAULT-NEXT:         let %[[VALUE_result7:[0-9]+]] result7: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr8:[0-9]+]] ptr8: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(8))));
// DEFAULT-NEXT:         let %[[VALUE_result8:[0-9]+]] result8: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr9:[0-9]+]] ptr9: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(9))));
// DEFAULT-NEXT:         let %[[VALUE_result9:[0-9]+]] result9: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr10:[0-9]+]] ptr10: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(10))));
// DEFAULT-NEXT:         let %[[VALUE_result10:[0-9]+]] result10: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr11:[0-9]+]] ptr11: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(11))));
// DEFAULT-NEXT:         let %[[VALUE_result11:[0-9]+]] result11: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr12:[0-9]+]] ptr12: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(12))));
// DEFAULT-NEXT:         let %[[VALUE_result12:[0-9]+]] result12: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr13:[0-9]+]] ptr13: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(13))));
// DEFAULT-NEXT:         let %[[VALUE_result13:[0-9]+]] result13: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr14:[0-9]+]] ptr14: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(14))));
// DEFAULT-NEXT:         let %[[VALUE_result14:[0-9]+]] result14: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr15:[0-9]+]] ptr15: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(15))));
// DEFAULT-NEXT:         let %[[VALUE_result15:[0-9]+]] result15: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr16:[0-9]+]] ptr16: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(16))));
// DEFAULT-NEXT:         let %[[VALUE_result16:[0-9]+]] result16: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr17:[0-9]+]] ptr17: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(17))));
// DEFAULT-NEXT:         let %[[VALUE_result17:[0-9]+]] result17: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr18:[0-9]+]] ptr18: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(18))));
// DEFAULT-NEXT:         let %[[VALUE_result18:[0-9]+]] result18: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ptr19:[0-9]+]] ptr19: ptr<f32> [storage=automatic] = read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), const<i32>(19))));
// DEFAULT-NEXT:         let %[[VALUE_result19:[0-9]+]] result19: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE1]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result0]]);
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE3]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr0]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result0]], read<f32>(%[[VALUE4]]));
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr0]]);
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE5]]), read<i32>(%[[VALUE_inc0]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr0]], read<ptr<f32>>(%[[VALUE6]]));
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result1]]);
// DEFAULT-NEXT:             let %[[VALUE8:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE7]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr1]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result1]], read<f32>(%[[VALUE8]]));
// DEFAULT-NEXT:             let %[[VALUE9:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr1]]);
// DEFAULT-NEXT:             let %[[VALUE10:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE9]]), read<i32>(%[[VALUE_inc1]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr1]], read<ptr<f32>>(%[[VALUE10]]));
// DEFAULT-NEXT:             let %[[VALUE11:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result2]]);
// DEFAULT-NEXT:             let %[[VALUE12:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE11]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr2]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result2]], read<f32>(%[[VALUE12]]));
// DEFAULT-NEXT:             let %[[VALUE13:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr2]]);
// DEFAULT-NEXT:             let %[[VALUE14:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE13]]), read<i32>(%[[VALUE_inc2]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr2]], read<ptr<f32>>(%[[VALUE14]]));
// DEFAULT-NEXT:             let %[[VALUE15:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result3]]);
// DEFAULT-NEXT:             let %[[VALUE16:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE15]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr3]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result3]], read<f32>(%[[VALUE16]]));
// DEFAULT-NEXT:             let %[[VALUE17:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr3]]);
// DEFAULT-NEXT:             let %[[VALUE18:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE17]]), read<i32>(%[[VALUE_inc3]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr3]], read<ptr<f32>>(%[[VALUE18]]));
// DEFAULT-NEXT:             let %[[VALUE19:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result4]]);
// DEFAULT-NEXT:             let %[[VALUE20:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE19]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr4]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result4]], read<f32>(%[[VALUE20]]));
// DEFAULT-NEXT:             let %[[VALUE21:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr4]]);
// DEFAULT-NEXT:             let %[[VALUE22:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE21]]), read<i32>(%[[VALUE_inc4]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr4]], read<ptr<f32>>(%[[VALUE22]]));
// DEFAULT-NEXT:             let %[[VALUE23:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result5]]);
// DEFAULT-NEXT:             let %[[VALUE24:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE23]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr5]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result5]], read<f32>(%[[VALUE24]]));
// DEFAULT-NEXT:             let %[[VALUE25:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr5]]);
// DEFAULT-NEXT:             let %[[VALUE26:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE25]]), read<i32>(%[[VALUE_inc5]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr5]], read<ptr<f32>>(%[[VALUE26]]));
// DEFAULT-NEXT:             let %[[VALUE27:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result6]]);
// DEFAULT-NEXT:             let %[[VALUE28:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE27]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr6]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result6]], read<f32>(%[[VALUE28]]));
// DEFAULT-NEXT:             let %[[VALUE29:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr6]]);
// DEFAULT-NEXT:             let %[[VALUE30:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE29]]), read<i32>(%[[VALUE_inc6]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr6]], read<ptr<f32>>(%[[VALUE30]]));
// DEFAULT-NEXT:             let %[[VALUE31:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result7]]);
// DEFAULT-NEXT:             let %[[VALUE32:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE31]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr7]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result7]], read<f32>(%[[VALUE32]]));
// DEFAULT-NEXT:             let %[[VALUE33:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr7]]);
// DEFAULT-NEXT:             let %[[VALUE34:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE33]]), read<i32>(%[[VALUE_inc7]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr7]], read<ptr<f32>>(%[[VALUE34]]));
// DEFAULT-NEXT:             let %[[VALUE35:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result8]]);
// DEFAULT-NEXT:             let %[[VALUE36:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE35]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr8]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result8]], read<f32>(%[[VALUE36]]));
// DEFAULT-NEXT:             let %[[VALUE37:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr8]]);
// DEFAULT-NEXT:             let %[[VALUE38:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE37]]), read<i32>(%[[VALUE_inc8]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr8]], read<ptr<f32>>(%[[VALUE38]]));
// DEFAULT-NEXT:             let %[[VALUE39:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result9]]);
// DEFAULT-NEXT:             let %[[VALUE40:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE39]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr9]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result9]], read<f32>(%[[VALUE40]]));
// DEFAULT-NEXT:             let %[[VALUE41:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr9]]);
// DEFAULT-NEXT:             let %[[VALUE42:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE41]]), read<i32>(%[[VALUE_inc9]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr9]], read<ptr<f32>>(%[[VALUE42]]));
// DEFAULT-NEXT:             let %[[VALUE43:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result10]]);
// DEFAULT-NEXT:             let %[[VALUE44:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE43]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr10]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result10]], read<f32>(%[[VALUE44]]));
// DEFAULT-NEXT:             let %[[VALUE45:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr10]]);
// DEFAULT-NEXT:             let %[[VALUE46:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE45]]), read<i32>(%[[VALUE_inc10]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr10]], read<ptr<f32>>(%[[VALUE46]]));
// DEFAULT-NEXT:             let %[[VALUE47:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result11]]);
// DEFAULT-NEXT:             let %[[VALUE48:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE47]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr11]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result11]], read<f32>(%[[VALUE48]]));
// DEFAULT-NEXT:             let %[[VALUE49:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr11]]);
// DEFAULT-NEXT:             let %[[VALUE50:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE49]]), read<i32>(%[[VALUE_inc11]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr11]], read<ptr<f32>>(%[[VALUE50]]));
// DEFAULT-NEXT:             let %[[VALUE51:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result12]]);
// DEFAULT-NEXT:             let %[[VALUE52:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE51]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr12]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result12]], read<f32>(%[[VALUE52]]));
// DEFAULT-NEXT:             let %[[VALUE53:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr12]]);
// DEFAULT-NEXT:             let %[[VALUE54:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE53]]), read<i32>(%[[VALUE_inc12]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr12]], read<ptr<f32>>(%[[VALUE54]]));
// DEFAULT-NEXT:             let %[[VALUE55:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result13]]);
// DEFAULT-NEXT:             let %[[VALUE56:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE55]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr13]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result13]], read<f32>(%[[VALUE56]]));
// DEFAULT-NEXT:             let %[[VALUE57:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr13]]);
// DEFAULT-NEXT:             let %[[VALUE58:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE57]]), read<i32>(%[[VALUE_inc13]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr13]], read<ptr<f32>>(%[[VALUE58]]));
// DEFAULT-NEXT:             let %[[VALUE59:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result14]]);
// DEFAULT-NEXT:             let %[[VALUE60:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE59]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr14]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result14]], read<f32>(%[[VALUE60]]));
// DEFAULT-NEXT:             let %[[VALUE61:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr14]]);
// DEFAULT-NEXT:             let %[[VALUE62:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE61]]), read<i32>(%[[VALUE_inc14]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr14]], read<ptr<f32>>(%[[VALUE62]]));
// DEFAULT-NEXT:             let %[[VALUE63:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result15]]);
// DEFAULT-NEXT:             let %[[VALUE64:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE63]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr15]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result15]], read<f32>(%[[VALUE64]]));
// DEFAULT-NEXT:             let %[[VALUE65:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr15]]);
// DEFAULT-NEXT:             let %[[VALUE66:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE65]]), read<i32>(%[[VALUE_inc15]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr15]], read<ptr<f32>>(%[[VALUE66]]));
// DEFAULT-NEXT:             let %[[VALUE67:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result16]]);
// DEFAULT-NEXT:             let %[[VALUE68:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE67]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr16]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result16]], read<f32>(%[[VALUE68]]));
// DEFAULT-NEXT:             let %[[VALUE69:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr16]]);
// DEFAULT-NEXT:             let %[[VALUE70:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE69]]), read<i32>(%[[VALUE_inc16]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr16]], read<ptr<f32>>(%[[VALUE70]]));
// DEFAULT-NEXT:             let %[[VALUE71:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result17]]);
// DEFAULT-NEXT:             let %[[VALUE72:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE71]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr17]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result17]], read<f32>(%[[VALUE72]]));
// DEFAULT-NEXT:             let %[[VALUE73:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr17]]);
// DEFAULT-NEXT:             let %[[VALUE74:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE73]]), read<i32>(%[[VALUE_inc17]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr17]], read<ptr<f32>>(%[[VALUE74]]));
// DEFAULT-NEXT:             let %[[VALUE75:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result18]]);
// DEFAULT-NEXT:             let %[[VALUE76:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE75]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr18]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result18]], read<f32>(%[[VALUE76]]));
// DEFAULT-NEXT:             let %[[VALUE77:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr18]]);
// DEFAULT-NEXT:             let %[[VALUE78:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE77]]), read<i32>(%[[VALUE_inc18]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr18]], read<ptr<f32>>(%[[VALUE78]]));
// DEFAULT-NEXT:             let %[[VALUE79:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_result19]]);
// DEFAULT-NEXT:             let %[[VALUE80:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE79]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_ptr19]]))));
// DEFAULT-NEXT:             write<f32>(%[[VALUE_result19]], read<f32>(%[[VALUE80]]));
// DEFAULT-NEXT:             let %[[VALUE81:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_ptr19]]);
// DEFAULT-NEXT:             let %[[VALUE82:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE81]]), read<i32>(%[[VALUE_inc19]]));
// DEFAULT-NEXT:             write<ptr<f32>>(%[[VALUE_ptr19]], read<ptr<f32>>(%[[VALUE82]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(0))), read<f32>(%[[VALUE_result0]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(1))), read<f32>(%[[VALUE_result1]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(2))), read<f32>(%[[VALUE_result2]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(3))), read<f32>(%[[VALUE_result3]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(4))), read<f32>(%[[VALUE_result4]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(5))), read<f32>(%[[VALUE_result5]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(6))), read<f32>(%[[VALUE_result6]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(7))), read<f32>(%[[VALUE_result7]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(8))), read<f32>(%[[VALUE_result8]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(9))), read<f32>(%[[VALUE_result9]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(10))), read<f32>(%[[VALUE_result10]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(11))), read<f32>(%[[VALUE_result11]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(12))), read<f32>(%[[VALUE_result12]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(13))), read<f32>(%[[VALUE_result13]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(14))), read<f32>(%[[VALUE_result14]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(15))), read<f32>(%[[VALUE_result15]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(16))), read<f32>(%[[VALUE_result16]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(17))), read<f32>(%[[VALUE_result17]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(18))), read<f32>(%[[VALUE_result18]]));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), const<i32>(19))), read<f32>(%[[VALUE_result19]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE83:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(20))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE84:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE85:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE84]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE85]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(20)>(%[[VALUE_ptrs]]), read<i32>(%[[VALUE_i]]))), ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(80)>(%[[VALUE_input]]), read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%[[VALUE_incs]]), read<i32>(%[[VALUE_i]]))), read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:         for %[[VALUE86:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), mul<i32, overflow=ub>(const<i32>(4), const<i32>(20)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE87:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE88:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE87]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE88]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(80)>(%[[VALUE_input]]), read<i32>(%[[VALUE_i]]))), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_foo]], const<i32>(4));
// DEFAULT-NEXT:         for %[[VALUE89:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(20))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE90:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE91:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE90]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE91]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(20)>(%[[VALUE_results]]), read<i32>(%[[VALUE_i]])))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(4)), add<i32, overflow=ub>(const<i32>(4), const<i32>(1))), const<i32>(2))))
// DEFAULT-NEXT:                     return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
