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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_f0:[0-9]+]] f0: vector<f32, 4> [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(3.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(4.0)));
// DEFAULT-NEXT:         let %[[VALUE_f1:[0-9]+]] f1: vector<f32, 4> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_f2:[0-9]+]] f2: vector<f32, 4> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d0:[0-9]+]] d0: vector<f64, 2> [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(1.0), index1 = const<f64>(2.0));
// DEFAULT-NEXT:         let %[[VALUE_d1:[0-9]+]] d1: vector<f64, 2> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d2:[0-9]+]] d2: vector<f64, 2> [storage=automatic];
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f1]], add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(vector_splat<vector<f32, 4>, reason=usual_arith>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), read<vector<f32, 4>>(%[[VALUE_f0]])));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f2]], add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)))), read<vector<f32, 4>>(%[[VALUE_f0]])));
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i]]);
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f1]])), read<i32>(%[[VALUE___i]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f2]])), read<i32>(%[[VALUE___i]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f1]], sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(vector_splat<vector<f32, 4>, reason=usual_arith>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), read<vector<f32, 4>>(%[[VALUE_f0]])));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f2]], sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(compound_literal %[[VALUE5:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)))), read<vector<f32, 4>>(%[[VALUE_f0]])));
// DEFAULT-NEXT:         do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_2:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_2]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_2]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_2]]);
// DEFAULT-NEXT:                         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_2]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f1]])), read<i32>(%[[VALUE___i_2]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f2]])), read<i32>(%[[VALUE___i_2]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f1]], mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(vector_splat<vector<f32, 4>, reason=usual_arith>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), read<vector<f32, 4>>(%[[VALUE_f0]])));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f2]], mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(compound_literal %[[VALUE10:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)))), read<vector<f32, 4>>(%[[VALUE_f0]])));
// DEFAULT-NEXT:         do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_3:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_3]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_3]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_3]]);
// DEFAULT-NEXT:                         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_3]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f1]])), read<i32>(%[[VALUE___i_3]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f2]])), read<i32>(%[[VALUE___i_3]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f1]], div<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(vector_splat<vector<f32, 4>, reason=usual_arith>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), read<vector<f32, 4>>(%[[VALUE_f0]])));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f2]], div<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(compound_literal %[[VALUE15:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)))), read<vector<f32, 4>>(%[[VALUE_f0]])));
// DEFAULT-NEXT:         do %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_4:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_4]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_4]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_4]]);
// DEFAULT-NEXT:                         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_4]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f1]])), read<i32>(%[[VALUE___i_4]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f2]])), read<i32>(%[[VALUE___i_4]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f1]], add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE_f0]]), vector_splat<vector<f32, 4>, reason=usual_arith>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f2]], add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE_f0]]), read<vector<f32, 4>>(compound_literal %[[VALUE20:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0))))));
// DEFAULT-NEXT:         do %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_5:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_5]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_5]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_5]]);
// DEFAULT-NEXT:                         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE23]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_5]], read<i32>(%[[VALUE24]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f1]])), read<i32>(%[[VALUE___i_5]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f2]])), read<i32>(%[[VALUE___i_5]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f1]], sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE_f0]]), vector_splat<vector<f32, 4>, reason=usual_arith>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f2]], sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE_f0]]), read<vector<f32, 4>>(compound_literal %[[VALUE25:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0))))));
// DEFAULT-NEXT:         do %[[VALUE26:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_6:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE27:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_6]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_6]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE28:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_6]]);
// DEFAULT-NEXT:                         let %[[VALUE29:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE28]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_6]], read<i32>(%[[VALUE29]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f1]])), read<i32>(%[[VALUE___i_6]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f2]])), read<i32>(%[[VALUE___i_6]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f1]], mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE_f0]]), vector_splat<vector<f32, 4>, reason=usual_arith>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f2]], mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE_f0]]), read<vector<f32, 4>>(compound_literal %[[VALUE30:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0))))));
// DEFAULT-NEXT:         do %[[VALUE31:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_7:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE32:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_7]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_7]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE33:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_7]]);
// DEFAULT-NEXT:                         let %[[VALUE34:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE33]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_7]], read<i32>(%[[VALUE34]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f1]])), read<i32>(%[[VALUE___i_7]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f2]])), read<i32>(%[[VALUE___i_7]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f1]], div<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE_f0]]), vector_splat<vector<f32, 4>, reason=usual_arith>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f2]], div<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE_f0]]), read<vector<f32, 4>>(compound_literal %[[VALUE35:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0))))));
// DEFAULT-NEXT:         do %[[VALUE36:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_8:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE37:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_8]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_8]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE38:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_8]]);
// DEFAULT-NEXT:                         let %[[VALUE39:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE38]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_8]], read<i32>(%[[VALUE39]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f1]])), read<i32>(%[[VALUE___i_8]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f2]])), read<i32>(%[[VALUE___i_8]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d1]], add<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(vector_splat<vector<f64, 2>, reason=usual_arith>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), read<vector<f64, 2>>(%[[VALUE_d0]])));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d2]], add<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(compound_literal %[[VALUE40:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0))), read<vector<f64, 2>>(%[[VALUE_d0]])));
// DEFAULT-NEXT:         do %[[VALUE41:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_9:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE42:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_9]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_9]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE43:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_9]]);
// DEFAULT-NEXT:                         let %[[VALUE44:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE43]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_9]], read<i32>(%[[VALUE44]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d1]])), read<i32>(%[[VALUE___i_9]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d2]])), read<i32>(%[[VALUE___i_9]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d1]], sub<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(vector_splat<vector<f64, 2>, reason=usual_arith>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), read<vector<f64, 2>>(%[[VALUE_d0]])));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d2]], sub<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(compound_literal %[[VALUE45:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0))), read<vector<f64, 2>>(%[[VALUE_d0]])));
// DEFAULT-NEXT:         do %[[VALUE46:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_10:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE47:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_10]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_10]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE48:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_10]]);
// DEFAULT-NEXT:                         let %[[VALUE49:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE48]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_10]], read<i32>(%[[VALUE49]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d1]])), read<i32>(%[[VALUE___i_10]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d2]])), read<i32>(%[[VALUE___i_10]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d1]], mul<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(vector_splat<vector<f64, 2>, reason=usual_arith>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), read<vector<f64, 2>>(%[[VALUE_d0]])));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d2]], mul<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(compound_literal %[[VALUE50:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0))), read<vector<f64, 2>>(%[[VALUE_d0]])));
// DEFAULT-NEXT:         do %[[VALUE51:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_11:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE52:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_11]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_11]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE53:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_11]]);
// DEFAULT-NEXT:                         let %[[VALUE54:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE53]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_11]], read<i32>(%[[VALUE54]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d1]])), read<i32>(%[[VALUE___i_11]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d2]])), read<i32>(%[[VALUE___i_11]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d1]], div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(vector_splat<vector<f64, 2>, reason=usual_arith>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), read<vector<f64, 2>>(%[[VALUE_d0]])));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d2]], div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(compound_literal %[[VALUE55:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0))), read<vector<f64, 2>>(%[[VALUE_d0]])));
// DEFAULT-NEXT:         do %[[VALUE56:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_12:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE57:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_12]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_12]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE58:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_12]]);
// DEFAULT-NEXT:                         let %[[VALUE59:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE58]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_12]], read<i32>(%[[VALUE59]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d1]])), read<i32>(%[[VALUE___i_12]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d2]])), read<i32>(%[[VALUE___i_12]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d1]], add<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%[[VALUE_d0]]), vector_splat<vector<f64, 2>, reason=usual_arith>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d2]], add<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%[[VALUE_d0]]), read<vector<f64, 2>>(compound_literal %[[VALUE60:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         do %[[VALUE61:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_13:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE62:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_13]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_13]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE63:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_13]]);
// DEFAULT-NEXT:                         let %[[VALUE64:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE63]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_13]], read<i32>(%[[VALUE64]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d1]])), read<i32>(%[[VALUE___i_13]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d2]])), read<i32>(%[[VALUE___i_13]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d1]], sub<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%[[VALUE_d0]]), vector_splat<vector<f64, 2>, reason=usual_arith>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d2]], sub<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%[[VALUE_d0]]), read<vector<f64, 2>>(compound_literal %[[VALUE65:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         do %[[VALUE66:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_14:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE67:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_14]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_14]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE68:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_14]]);
// DEFAULT-NEXT:                         let %[[VALUE69:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE68]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_14]], read<i32>(%[[VALUE69]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d1]])), read<i32>(%[[VALUE___i_14]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d2]])), read<i32>(%[[VALUE___i_14]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d1]], mul<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%[[VALUE_d0]]), vector_splat<vector<f64, 2>, reason=usual_arith>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d2]], mul<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%[[VALUE_d0]]), read<vector<f64, 2>>(compound_literal %[[VALUE70:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         do %[[VALUE71:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_15:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE72:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_15]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_15]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE73:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_15]]);
// DEFAULT-NEXT:                         let %[[VALUE74:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE73]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_15]], read<i32>(%[[VALUE74]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d1]])), read<i32>(%[[VALUE___i_15]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d2]])), read<i32>(%[[VALUE___i_15]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d1]], div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%[[VALUE_d0]]), vector_splat<vector<f64, 2>, reason=usual_arith>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d2]], div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%[[VALUE_d0]]), read<vector<f64, 2>>(compound_literal %[[VALUE75:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         do %[[VALUE76:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_16:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE77:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_16]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_16]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE78:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_16]]);
// DEFAULT-NEXT:                         let %[[VALUE79:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE78]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_16]], read<i32>(%[[VALUE79]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d1]])), read<i32>(%[[VALUE___i_16]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d2]])), read<i32>(%[[VALUE___i_16]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
