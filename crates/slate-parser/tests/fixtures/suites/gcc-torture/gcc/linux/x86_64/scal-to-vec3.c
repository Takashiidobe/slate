#define vector(elcount, type)                                                  \
  __attribute__((vector_size((elcount) * sizeof(type)))) type

#define vidx(type, vec, idx) (*((type *)&(vec) + idx))

#define veccompare(type, count, v0, v1)                                        \
  do {                                                                         \
    int __i;                                                                   \
    for (__i = 0; __i < count; __i++) {                                        \
      if (vidx(type, v0, __i) != vidx(type, v1, __i))                          \
        __builtin_abort();                                                     \
    }                                                                          \
  } while (0)

int main(int argc, char *argv[]) {
#define fvec_2 (vector(4, float)){2., 2., 2., 2.}
#define dvec_2 (vector(2, double)){2., 2.}

  vector(4, float) f0 = {1., 2., 3., 4.};
  vector(4, float) f1, f2;

  vector(2, double) d0 = {1., 2.};
  vector(2, double) d1, d2;

  f1 = 2 + f0;
  f2 = fvec_2 + f0;
  veccompare(float, 4, f1, f2);
  f1 = 2 - f0;
  f2 = fvec_2 - f0;
  veccompare(float, 4, f1, f2);
  f1 = 2 * f0;
  f2 = fvec_2 * f0;
  veccompare(float, 4, f1, f2);
  f1 = 2 / f0;
  f2 = fvec_2 / f0;
  veccompare(float, 4, f1, f2);

  f1 = f0 + 2;
  f2 = f0 + fvec_2;
  veccompare(float, 4, f1, f2);
  f1 = f0 - 2;
  f2 = f0 - fvec_2;
  veccompare(float, 4, f1, f2);
  f1 = f0 * 2;
  f2 = f0 * fvec_2;
  veccompare(float, 4, f1, f2);
  f1 = f0 / 2;
  f2 = f0 / fvec_2;
  veccompare(float, 4, f1, f2);

  d1 = 2 + d0;
  d2 = dvec_2 + d0;
  veccompare(double, 2, d1, d2);
  d1 = 2 - d0;
  d2 = dvec_2 - d0;
  veccompare(double, 2, d1, d2);
  d1 = 2 * d0;
  d2 = dvec_2 * d0;
  veccompare(double, 2, d1, d2);
  d1 = 2 / d0;
  d2 = dvec_2 / d0;
  veccompare(double, 2, d1, d2);

  d1 = d0 + 2;
  d2 = d0 + dvec_2;
  veccompare(double, 2, d1, d2);
  d1 = d0 - 2;
  d2 = d0 - dvec_2;
  veccompare(double, 2, d1, d2);
  d1 = d0 * 2;
  d2 = d0 * dvec_2;
  veccompare(double, 2, d1, d2);
  d1 = d0 / 2;
  d2 = d0 / dvec_2;
  veccompare(double, 2, d1, d2);

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
// DEFAULT-NEXT:     fn %28 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %0 @main(%1 argc: i32, %2 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 f0: vector<f32, 4> [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(1.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(3.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(4.0)));
// DEFAULT-NEXT:         let %4 f1: vector<f32, 4> [storage=automatic];
// DEFAULT-NEXT:         let %5 f2: vector<f32, 4> [storage=automatic];
// DEFAULT-NEXT:         let %6 d0: vector<f64, 2> [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(1.0), index1 = const<f64>(2.0));
// DEFAULT-NEXT:         let %7 d1: vector<f64, 2> [storage=automatic];
// DEFAULT-NEXT:         let %8 d2: vector<f64, 2> [storage=automatic];
// DEFAULT-NEXT:         write<vector<f32, 4>>(%4, add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(vector_splat<vector<f32, 4>, reason=usual_arith>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))), read<vector<f32, 4>>(%3)));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%5, add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(compound_literal %25 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)))), read<vector<f32, 4>>(%3)));
// DEFAULT-NEXT:         do %26
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %9 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %27
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%9), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %74: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                         let %75: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%74), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%9, read<i32>(%75));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%4)), read<i32>(%9)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%5)), read<i32>(%9)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%4, sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(vector_splat<vector<f32, 4>, reason=usual_arith>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))), read<vector<f32, 4>>(%3)));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%5, sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(compound_literal %29 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)))), read<vector<f32, 4>>(%3)));
// DEFAULT-NEXT:         do %30
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %10 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %31
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%10), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %76: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                         let %77: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%76), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%10, read<i32>(%77));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%4)), read<i32>(%10)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%5)), read<i32>(%10)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%4, mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(vector_splat<vector<f32, 4>, reason=usual_arith>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))), read<vector<f32, 4>>(%3)));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%5, mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(compound_literal %32 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)))), read<vector<f32, 4>>(%3)));
// DEFAULT-NEXT:         do %33
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %11 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %34
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%11), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %78: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                         let %79: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%78), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%11, read<i32>(%79));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%4)), read<i32>(%11)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%5)), read<i32>(%11)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%4, div<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(vector_splat<vector<f32, 4>, reason=usual_arith>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))), read<vector<f32, 4>>(%3)));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%5, div<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(compound_literal %35 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)))), read<vector<f32, 4>>(%3)));
// DEFAULT-NEXT:         do %36
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %12 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %37
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%12, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%12), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %80: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                         let %81: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%80), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%12, read<i32>(%81));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%4)), read<i32>(%12)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%5)), read<i32>(%12)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%4, add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(%3), vector_splat<vector<f32, 4>, reason=usual_arith>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%5, add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(%3), read<vector<f32, 4>>(compound_literal %38 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0))))));
// DEFAULT-NEXT:         do %39
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %13 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %40
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%13, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%13), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %82: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                         let %83: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%82), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%13, read<i32>(%83));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%4)), read<i32>(%13)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%5)), read<i32>(%13)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%4, sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(%3), vector_splat<vector<f32, 4>, reason=usual_arith>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%5, sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(%3), read<vector<f32, 4>>(compound_literal %41 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0))))));
// DEFAULT-NEXT:         do %42
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %14 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %43
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%14, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%14), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %84: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                         let %85: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%84), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%14, read<i32>(%85));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%4)), read<i32>(%14)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%5)), read<i32>(%14)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%4, mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(%3), vector_splat<vector<f32, 4>, reason=usual_arith>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%5, mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(%3), read<vector<f32, 4>>(compound_literal %44 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0))))));
// DEFAULT-NEXT:         do %45
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %15 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %46
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%15, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%15), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %86: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                         let %87: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%86), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%15, read<i32>(%87));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%4)), read<i32>(%15)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%5)), read<i32>(%15)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%4, div<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(%3), vector_splat<vector<f32, 4>, reason=usual_arith>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%5, div<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(%3), read<vector<f32, 4>>(compound_literal %47 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0))))));
// DEFAULT-NEXT:         do %48
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %16 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %49
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%16), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %88: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                         let %89: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%88), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%16, read<i32>(%89));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%4)), read<i32>(%16)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%5)), read<i32>(%16)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%7, add<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(vector_splat<vector<f64, 2>, reason=usual_arith>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))), read<vector<f64, 2>>(%6)));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%8, add<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(compound_literal %50 [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0))), read<vector<f64, 2>>(%6)));
// DEFAULT-NEXT:         do %51
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %17 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %52
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%17, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%17), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %90: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                         let %91: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%90), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%17, read<i32>(%91));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%7)), read<i32>(%17)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%8)), read<i32>(%17)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%7, sub<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(vector_splat<vector<f64, 2>, reason=usual_arith>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))), read<vector<f64, 2>>(%6)));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%8, sub<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(compound_literal %53 [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0))), read<vector<f64, 2>>(%6)));
// DEFAULT-NEXT:         do %54
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %18 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %55
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%18, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%18), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %92: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:                         let %93: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%92), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%18, read<i32>(%93));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%7)), read<i32>(%18)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%8)), read<i32>(%18)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%7, mul<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(vector_splat<vector<f64, 2>, reason=usual_arith>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))), read<vector<f64, 2>>(%6)));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%8, mul<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(compound_literal %56 [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0))), read<vector<f64, 2>>(%6)));
// DEFAULT-NEXT:         do %57
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %19 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %58
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%19), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %94: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                         let %95: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%94), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%19, read<i32>(%95));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%7)), read<i32>(%19)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%8)), read<i32>(%19)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%7, div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(vector_splat<vector<f64, 2>, reason=usual_arith>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))), read<vector<f64, 2>>(%6)));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%8, div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(compound_literal %59 [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0))), read<vector<f64, 2>>(%6)));
// DEFAULT-NEXT:         do %60
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %20 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %61
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%20, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%20), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %96: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:                         let %97: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%96), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%20, read<i32>(%97));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%7)), read<i32>(%20)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%8)), read<i32>(%20)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%7, add<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(%6), vector_splat<vector<f64, 2>, reason=usual_arith>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%8, add<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(%6), read<vector<f64, 2>>(compound_literal %62 [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         do %63
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %21 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %64
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%21, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%21), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %98: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                         let %99: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%98), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%21, read<i32>(%99));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%7)), read<i32>(%21)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%8)), read<i32>(%21)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%7, sub<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(%6), vector_splat<vector<f64, 2>, reason=usual_arith>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%8, sub<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(%6), read<vector<f64, 2>>(compound_literal %65 [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         do %66
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %22 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %67
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%22, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%22), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %100: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:                         let %101: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%100), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%22, read<i32>(%101));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%7)), read<i32>(%22)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%8)), read<i32>(%22)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%7, mul<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(%6), vector_splat<vector<f64, 2>, reason=usual_arith>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%8, mul<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(%6), read<vector<f64, 2>>(compound_literal %68 [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         do %69
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %23 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %70
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%23, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%23), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %102: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:                         let %103: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%102), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%23, read<i32>(%103));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%7)), read<i32>(%23)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%8)), read<i32>(%23)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%7, div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(%6), vector_splat<vector<f64, 2>, reason=usual_arith>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%8, div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(%6), read<vector<f64, 2>>(compound_literal %71 [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         do %72
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %24 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %73
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%24, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%24), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %104: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:                         let %105: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%104), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%24, read<i32>(%105));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%7)), read<i32>(%24)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%8)), read<i32>(%24)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
