/* PR rtl-optimization/70542 */
/* { dg-do run } */
/* { dg-require-effective-target int32plus } */

int   a[113], d[113];
short b[113], c[113], e[113];

int
main() {
  int  i;
  long j;
  for (i = 0; i < 113; ++i) {
    a[i] = -636544305;
    b[i] = -31804;
  }
  for (j = 1; j <= 112; ++j) {
    c[j] = b[j] >> ((a[j] & 1587842570) - 1510214139);
    if (a[j])
      d[j] = j;
    e[j] = 7 << ((2312631697 - b[j]) - 2312663500);
  }
  asm volatile("" : : : "memory");
  if (c[0] || d[0] || e[0])
    __builtin_abort();
  for (i = 1; i <= 112; ++i)
    if (c[i] != -1 || d[i] != i || e[i] != 14)
      __builtin_abort();
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
// DEFAULT-NEXT:     global %0 a: array<i32, 113> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %1 d: array<i32, 113> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %2 b: array<i16, 113> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %3 c: array<i16, 113> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %4 e: array<i16, 113> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %10 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 j: i64 [storage=automatic];
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%6), const<i32>(113))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(113)>(%0), read<i32>(%6))), neg<i32, overflow=ub>(const<i32>(636544305)));
// DEFAULT-NEXT:                     write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(113)>(%2), read<i32>(%6))), truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(31804))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i64>(%7, widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:             condition: le<i64>(read<i64>(%7), widen<i64, reason=usual_arith>(const<i32>(112)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: i64 [synthetic] = read<i64>(%7);
// DEFAULT-NEXT:                 let %15: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%14), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(%7, read<i64>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(113)>(%3), read<i64>(%7))), truncate<i16, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(113)>(%2), read<i64>(%7))))), sub<i32, overflow=ub>(and<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(113)>(%0), read<i64>(%7)))), const<i32>(1587842570)), const<i32>(1510214139)))));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(113)>(%0), read<i64>(%7)))), const<i32>(0))
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(113)>(%1), read<i64>(%7))), truncate<i32, reason=assign, fits=unknown>(read<i64>(%7)));
// DEFAULT-NEXT:                     write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(113)>(%4), read<i64>(%7))), truncate<i16, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(7), sub<i64, overflow=ub>(sub<i64, overflow=ub>(const<i64>(2312631697), widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(113)>(%2), read<i64>(%7))))))), const<i64>(2312663500)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i16>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(113)>(%3), const<i32>(0)))), const<i16>(0)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(113)>(%1), const<i32>(0)))), const<i32>(0))), ne<i16>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(113)>(%4), const<i32>(0)))), const<i16>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%6, const<i32>(1));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%6), const<i32>(112))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(113)>(%3), read<i32>(%6))))), neg<i32, overflow=ub>(const<i32>(1))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(113)>(%1), read<i32>(%6)))), read<i32>(%6))), ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(113)>(%4), read<i32>(%6))))), const<i32>(14)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
