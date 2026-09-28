typedef struct creal_T {
  double re;
  double im;
} creal_T;

#define N 16
int main() {
  int     k;
  int     i;
  int     j;
  creal_T t2[N];
  double  inval;

  inval = 1.0;
  for (j = 0; j < N; ++j) {
    t2[j].re = 0;
    t2[j].im = 0;
  }

  for (j = 0; j < N / 4; j++) {
    i        = j * 4;
    t2[i].re = inval;
    t2[i].im = inval;
    k        = i + 3;
    t2[k].re = inval;
    t2[k].im = inval;
    t2[i]    = t2[k];
    t2[k].re = inval;
  }

  for (i = 0; i < 2; ++i)
    if (t2[i].re != !i || t2[i].im != !i)
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
// DEFAULT-NEXT:     type @type0 creal_T = struct {
// DEFAULT-NEXT:         field0 re: f64;
// DEFAULT-NEXT:         field1 im: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 creal_T = @type0;
// DEFAULT-NEXT:     fn %11 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 k: i32 [storage=automatic];
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %5 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 t2: array<@type0, 16> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %7 inval: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%7, const<f64>(1.0));
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(16)>(%6), read<i32>(%5)))), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)));
// DEFAULT-NEXT:                     write<f64>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(16)>(%6), read<i32>(%5)))), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(16), const<i32>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%4, mul<i32, overflow=ub>(read<i32>(%5), const<i32>(4)));
// DEFAULT-NEXT:                     write<f64>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(16)>(%6), read<i32>(%4)))), read<f64>(%7));
// DEFAULT-NEXT:                     write<f64>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(16)>(%6), read<i32>(%4)))), read<f64>(%7));
// DEFAULT-NEXT:                     write<i32>(%3, add<i32, overflow=ub>(read<i32>(%4), const<i32>(3)));
// DEFAULT-NEXT:                     write<f64>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(16)>(%6), read<i32>(%3)))), read<f64>(%7));
// DEFAULT-NEXT:                     write<f64>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(16)>(%6), read<i32>(%3)))), read<f64>(%7));
// DEFAULT-NEXT:                     write<@type0>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(16)>(%6), read<i32>(%4))), copy<@type0, reason=assign>(read<@type0>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(16)>(%6), read<i32>(%3))))));
// DEFAULT-NEXT:                     write<f64>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(16)>(%6), read<i32>(%3)))), read<f64>(%7));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(16)>(%6), read<i32>(%4))))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(from_bool<i32, reason=promotion>(not<bool>(ne<i32>(read<i32>(%4), const<i32>(0)))))), ne<f64, exceptions=observable>(read<f64>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(16)>(%6), read<i32>(%4))))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(from_bool<i32, reason=promotion>(not<bool>(ne<i32>(read<i32>(%4), const<i32>(0)))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
