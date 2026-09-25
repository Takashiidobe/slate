double x[5][10] = {{10, 11, 12, 13, 14, 15, -1, -1, -1, -1},
                   {21, 22, 23, 24, 25, 26, -1, -1, -1, -1},
                   {32, 33, 34, 35, 36, 37, -1, -1, -1, -1},
                   {43, 44, 45, 46, 47, 48, -1, -1, -1, -1},
                   {54, 55, 56, 57, 58, 59, -1, -1, -1, -1}};
double tmp[5][6];

void __attribute__((noinline)) test(void) {
  int i, j;
  for (i = 0; i < 5; ++i) {
    tmp[i][0] = x[i][0];
    tmp[i][1] = x[i][1];
    tmp[i][2] = x[i][2];
    tmp[i][3] = x[i][3];
    tmp[i][4] = x[i][4];
    tmp[i][5] = x[i][5];
  }
}
extern void abort(void);
int         main() {
  int i, j;
  test();
  for (i = 0; i < 5; ++i)
    for (j = 0; j < 6; ++j)
      if (tmp[i][j] == -1)
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
// DEFAULT-NEXT:     global %0 x: array<array<f64, 10>, 5> [storage=static] = aggregate<array<array<f64, 10>, 5>, zero_fill=false>(index0 = aggregate<array<f64, 10>, zero_fill=false>(index0 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(10)), index1 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(11)), index2 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(12)), index3 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(13)), index4 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(14)), index5 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(15)), index6 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), index7 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), index8 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), index9 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), index1 = aggregate<array<f64, 10>, zero_fill=false>(index0 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(21)), index1 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(22)), index2 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(23)), index3 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(24)), index4 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), index5 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(26)), index6 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), index7 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), index8 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), index9 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), index2 = aggregate<array<f64, 10>, zero_fill=false>(index0 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(32)), index1 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(33)), index2 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(34)), index3 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(35)), index4 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(36)), index5 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(37)), index6 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), index7 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), index8 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), index9 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), index3 = aggregate<array<f64, 10>, zero_fill=false>(index0 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(43)), index1 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(44)), index2 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(45)), index3 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(46)), index4 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(47)), index5 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(48)), index6 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), index7 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), index8 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), index9 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), index4 = aggregate<array<f64, 10>, zero_fill=false>(index0 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(54)), index1 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(55)), index2 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(56)), index3 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(57)), index4 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(58)), index5 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(59)), index6 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), index7 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), index8 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))), index9 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))) [linkage=external];
// DEFAULT-NEXT:     global %1 tmp: array<array<f64, 6>, 5> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @test() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %4 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%3), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(6)>(deref(ptr_offset<ptr<array<f64, 6>>, subtract=false, element=array<f64, 6>, overflow=ub>(array_decay<ptr<array<f64, 6>>, length=Some(5)>(%1), read<i32>(%3)))), const<i32>(0))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(10)>(deref(ptr_offset<ptr<array<f64, 10>>, subtract=false, element=array<f64, 10>, overflow=ub>(array_decay<ptr<array<f64, 10>>, length=Some(5)>(%0), read<i32>(%3)))), const<i32>(0)))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(6)>(deref(ptr_offset<ptr<array<f64, 6>>, subtract=false, element=array<f64, 6>, overflow=ub>(array_decay<ptr<array<f64, 6>>, length=Some(5)>(%1), read<i32>(%3)))), const<i32>(1))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(10)>(deref(ptr_offset<ptr<array<f64, 10>>, subtract=false, element=array<f64, 10>, overflow=ub>(array_decay<ptr<array<f64, 10>>, length=Some(5)>(%0), read<i32>(%3)))), const<i32>(1)))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(6)>(deref(ptr_offset<ptr<array<f64, 6>>, subtract=false, element=array<f64, 6>, overflow=ub>(array_decay<ptr<array<f64, 6>>, length=Some(5)>(%1), read<i32>(%3)))), const<i32>(2))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(10)>(deref(ptr_offset<ptr<array<f64, 10>>, subtract=false, element=array<f64, 10>, overflow=ub>(array_decay<ptr<array<f64, 10>>, length=Some(5)>(%0), read<i32>(%3)))), const<i32>(2)))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(6)>(deref(ptr_offset<ptr<array<f64, 6>>, subtract=false, element=array<f64, 6>, overflow=ub>(array_decay<ptr<array<f64, 6>>, length=Some(5)>(%1), read<i32>(%3)))), const<i32>(3))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(10)>(deref(ptr_offset<ptr<array<f64, 10>>, subtract=false, element=array<f64, 10>, overflow=ub>(array_decay<ptr<array<f64, 10>>, length=Some(5)>(%0), read<i32>(%3)))), const<i32>(3)))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(6)>(deref(ptr_offset<ptr<array<f64, 6>>, subtract=false, element=array<f64, 6>, overflow=ub>(array_decay<ptr<array<f64, 6>>, length=Some(5)>(%1), read<i32>(%3)))), const<i32>(4))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(10)>(deref(ptr_offset<ptr<array<f64, 10>>, subtract=false, element=array<f64, 10>, overflow=ub>(array_decay<ptr<array<f64, 10>>, length=Some(5)>(%0), read<i32>(%3)))), const<i32>(4)))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(6)>(deref(ptr_offset<ptr<array<f64, 6>>, subtract=false, element=array<f64, 6>, overflow=ub>(array_decay<ptr<array<f64, 6>>, length=Some(5)>(%1), read<i32>(%3)))), const<i32>(5))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(10)>(deref(ptr_offset<ptr<array<f64, 10>>, subtract=false, element=array<f64, 10>, overflow=ub>(array_decay<ptr<array<f64, 10>>, length=Some(5)>(%0), read<i32>(%3)))), const<i32>(5)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 j: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %11
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%8), const<i32>(6))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %16: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                         let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%8, read<i32>(%17));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         if eq<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(6)>(deref(ptr_offset<ptr<array<f64, 6>>, subtract=false, element=array<f64, 6>, overflow=ub>(array_decay<ptr<array<f64, 6>>, length=Some(5)>(%1), read<i32>(%7)))), read<i32>(%8)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
