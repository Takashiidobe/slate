#define vector(elcount, type)                                                  \
  __attribute__((vector_size((elcount) * sizeof(type)))) type

#define vidx(type, vec, idx) (*((type *)&(vec) + idx))

#define operl(a, b, op) (a op b)
#define operr(a, b, op) (b op a)

#define check(type, count, vec0, vec1, num, op, lr)                            \
  do {                                                                         \
    int __i;                                                                   \
    for (__i = 0; __i < count; __i++) {                                        \
      if (vidx(type, vec1, __i) != oper##lr(num, vidx(type, vec0, __i), op))   \
        __builtin_abort();                                                     \
    }                                                                          \
  } while (0)

#define veccompare(type, count, v0, v1)                                        \
  do {                                                                         \
    int __i;                                                                   \
    for (__i = 0; __i < count; __i++) {                                        \
      if (vidx(type, v0, __i) != vidx(type, v1, __i))                          \
        __builtin_abort();                                                     \
    }                                                                          \
  } while (0)

volatile int one = 1;

int main(int argc, char *argv[]) {
#define fvec_2 (vector(4, float)){2., 2., 2., 2.}
#define dvec_2 (vector(2, double)){2., 2.}

  vector(8, short) v0 = {one, 1, 2, 3, 4, 5, 6, 7};
  vector(8, short) v1;

  vector(4, float) f0 = {1., 2., 3., 4.};
  vector(4, float) f1, f2;

  vector(2, double) d0 = {1., 2.};
  vector(2, double) d1, d2;

  v1 = 2 + v0;
  check(short, 8, v0, v1, 2, +, l);
  v1 = 2 - v0;
  check(short, 8, v0, v1, 2, -, l);
  v1 = 2 * v0;
  check(short, 8, v0, v1, 2, *, l);
  v1 = 2 / v0;
  check(short, 8, v0, v1, 2, /, l);
  v1 = 2 % v0;
  check(short, 8, v0, v1, 2, %, l);
  v1 = 2 ^ v0;
  check(short, 8, v0, v1, 2, ^, l);
  v1 = 2 & v0;
  check(short, 8, v0, v1, 2, &, l);
  v1 = 2 | v0;
  check(short, 8, v0, v1, 2, |, l);
  v1 = 2 << v0;
  check(short, 8, v0, v1, 2, <<, l);
  v1 = 2 >> v0;
  check(short, 8, v0, v1, 2, >>, l);

  v1 = v0 + 2;
  check(short, 8, v0, v1, 2, +, r);
  v1 = v0 - 2;
  check(short, 8, v0, v1, 2, -, r);
  v1 = v0 * 2;
  check(short, 8, v0, v1, 2, *, r);
  v1 = v0 / 2;
  check(short, 8, v0, v1, 2, /, r);
  v1 = v0 % 2;
  check(short, 8, v0, v1, 2, %, r);
  v1 = v0 ^ 2;
  check(short, 8, v0, v1, 2, ^, r);
  v1 = v0 & 2;
  check(short, 8, v0, v1, 2, &, r);
  v1 = v0 | 2;
  check(short, 8, v0, v1, 2, |, r);

  f1 = 2. + f0;
  f2 = fvec_2 + f0;
  veccompare(float, 4, f1, f2);
  f1 = 2. - f0;
  f2 = fvec_2 - f0;
  veccompare(float, 4, f1, f2);
  f1 = 2. * f0;
  f2 = fvec_2 * f0;
  veccompare(float, 4, f1, f2);
  f1 = 2. / f0;
  f2 = fvec_2 / f0;
  veccompare(float, 4, f1, f2);

  f1 = f0 + 2.;
  f2 = f0 + fvec_2;
  veccompare(float, 4, f1, f2);
  f1 = f0 - 2.;
  f2 = f0 - fvec_2;
  veccompare(float, 4, f1, f2);
  f1 = f0 * 2.;
  f2 = f0 * fvec_2;
  veccompare(float, 4, f1, f2);
  f1 = f0 / 2.;
  f2 = f0 / fvec_2;
  veccompare(float, 4, f1, f2);

  d1 = 2. + d0;
  d2 = dvec_2 + d0;
  veccompare(double, 2, d1, d2);
  d1 = 2. - d0;
  d2 = dvec_2 - d0;
  veccompare(double, 2, d1, d2);
  d1 = 2. * d0;
  d2 = dvec_2 * d0;
  veccompare(double, 2, d1, d2);
  d1 = 2. / d0;
  d2 = dvec_2 / d0;
  veccompare(double, 2, d1, d2);

  d1 = d0 + 2.;
  d2 = d0 + dvec_2;
  veccompare(double, 2, d1, d2);
  d1 = d0 - 2.;
  d2 = d0 - dvec_2;
  veccompare(double, 2, d1, d2);
  d1 = d0 * 2.;
  d2 = d0 * dvec_2;
  veccompare(double, 2, d1, d2);
  d1 = d0 / 2.;
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
// DEFAULT-NEXT:     global %[[VALUE_one:[0-9]+]] one: volatile i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_v0:[0-9]+]] v0: vector<i16, 8> [storage=automatic] = aggregate<vector<i16, 8>, zero_fill=false>(index0 = truncate<i16, reason=assign, fits=unknown>(read<i32, volatile>(%[[VALUE_one]])), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(1)), index2 = truncate<i16, reason=assign, fits=always>(const<i32>(2)), index3 = truncate<i16, reason=assign, fits=always>(const<i32>(3)), index4 = truncate<i16, reason=assign, fits=always>(const<i32>(4)), index5 = truncate<i16, reason=assign, fits=always>(const<i32>(5)), index6 = truncate<i16, reason=assign, fits=always>(const<i32>(6)), index7 = truncate<i16, reason=assign, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         let %[[VALUE_v1:[0-9]+]] v1: vector<i16, 8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_f0:[0-9]+]] f0: vector<f32, 4> [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(1.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(3.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(4.0)));
// DEFAULT-NEXT:         let %[[VALUE_f1:[0-9]+]] f1: vector<f32, 4> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_f2:[0-9]+]] f2: vector<f32, 4> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d0:[0-9]+]] d0: vector<f64, 2> [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(1.0), index1 = const<f64>(2.0));
// DEFAULT-NEXT:         let %[[VALUE_d1:[0-9]+]] d1: vector<f64, 2> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d2:[0-9]+]] d2: vector<f64, 2> [storage=automatic];
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], add<vector<i16, 8>, elementwise=true, overflow=wrap>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%[[VALUE_v0]])));
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i]]);
// DEFAULT-NEXT:                         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i]]))))), add<i32, overflow=ub>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i]])))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], sub<vector<i16, 8>, elementwise=true, overflow=wrap>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%[[VALUE_v0]])));
// DEFAULT-NEXT:         do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_2:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_2]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_2]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_2]]);
// DEFAULT-NEXT:                         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_2]]))))), sub<i32, overflow=ub>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_2]])))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], mul<vector<i16, 8>, elementwise=true, overflow=wrap>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%[[VALUE_v0]])));
// DEFAULT-NEXT:         do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_3:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_3]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_3]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_3]]);
// DEFAULT-NEXT:                         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_3]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_3]]))))), mul<i32, overflow=ub>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_3]])))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], div<vector<i16, 8>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%[[VALUE_v0]])));
// DEFAULT-NEXT:         do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_4:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_4]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_4]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_4]]);
// DEFAULT-NEXT:                         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_4]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_4]]))))), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_4]])))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], rem<vector<i16, 8>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%[[VALUE_v0]])));
// DEFAULT-NEXT:         do %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_5:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_5]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_5]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_5]]);
// DEFAULT-NEXT:                         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_5]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_5]]))))), rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_5]])))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], xor<vector<i16, 8>, elementwise=true>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%[[VALUE_v0]])));
// DEFAULT-NEXT:         do %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_6:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_6]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_6]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_6]]);
// DEFAULT-NEXT:                         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_6]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_6]]))))), xor<i32>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_6]])))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], and<vector<i16, 8>, elementwise=true>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%[[VALUE_v0]])));
// DEFAULT-NEXT:         do %[[VALUE24:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_7:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_7]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_7]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_7]]);
// DEFAULT-NEXT:                         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_7]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_7]]))))), and<i32>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_7]])))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], or<vector<i16, 8>, elementwise=true>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%[[VALUE_v0]])));
// DEFAULT-NEXT:         do %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_8:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE29:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_8]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_8]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE30:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_8]]);
// DEFAULT-NEXT:                         let %[[VALUE31:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE30]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_8]], read<i32>(%[[VALUE31]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_8]]))))), or<i32>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_8]])))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], shl<vector<i16, 8>, elementwise=true, overflow=wrap, amount_out_of_range=ub, negative_left=ub>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%[[VALUE_v0]])));
// DEFAULT-NEXT:         do %[[VALUE32:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_9:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE33:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_9]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_9]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE34:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_9]]);
// DEFAULT-NEXT:                         let %[[VALUE35:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE34]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_9]], read<i32>(%[[VALUE35]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_9]]))))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_9]])))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], shr<vector<i16, 8>, elementwise=true, amount_out_of_range=ub, fill=sign_extend>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%[[VALUE_v0]])));
// DEFAULT-NEXT:         do %[[VALUE36:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_10:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE37:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_10]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_10]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE38:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_10]]);
// DEFAULT-NEXT:                         let %[[VALUE39:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE38]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_10]], read<i32>(%[[VALUE39]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_10]]))))), shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_10]])))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], add<vector<i16, 8>, elementwise=true, overflow=wrap>(read<vector<i16, 8>>(%[[VALUE_v0]]), vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         do %[[VALUE40:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_11:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE41:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_11]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_11]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE42:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_11]]);
// DEFAULT-NEXT:                         let %[[VALUE43:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE42]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_11]], read<i32>(%[[VALUE43]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_11]]))))), add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_11]]))))), const<i32>(2)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], sub<vector<i16, 8>, elementwise=true, overflow=wrap>(read<vector<i16, 8>>(%[[VALUE_v0]]), vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         do %[[VALUE44:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_12:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE45:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_12]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_12]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE46:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_12]]);
// DEFAULT-NEXT:                         let %[[VALUE47:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE46]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_12]], read<i32>(%[[VALUE47]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_12]]))))), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_12]]))))), const<i32>(2)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], mul<vector<i16, 8>, elementwise=true, overflow=wrap>(read<vector<i16, 8>>(%[[VALUE_v0]]), vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         do %[[VALUE48:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_13:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE49:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_13]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_13]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE50:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_13]]);
// DEFAULT-NEXT:                         let %[[VALUE51:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE50]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_13]], read<i32>(%[[VALUE51]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_13]]))))), mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_13]]))))), const<i32>(2)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], div<vector<i16, 8>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(read<vector<i16, 8>>(%[[VALUE_v0]]), vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         do %[[VALUE52:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_14:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE53:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_14]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_14]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE54:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_14]]);
// DEFAULT-NEXT:                         let %[[VALUE55:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE54]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_14]], read<i32>(%[[VALUE55]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_14]]))))), div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_14]]))))), const<i32>(2)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], rem<vector<i16, 8>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(read<vector<i16, 8>>(%[[VALUE_v0]]), vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         do %[[VALUE56:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_15:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE57:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_15]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_15]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE58:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_15]]);
// DEFAULT-NEXT:                         let %[[VALUE59:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE58]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_15]], read<i32>(%[[VALUE59]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_15]]))))), rem<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_15]]))))), const<i32>(2)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], xor<vector<i16, 8>, elementwise=true>(read<vector<i16, 8>>(%[[VALUE_v0]]), vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         do %[[VALUE60:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_16:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE61:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_16]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_16]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE62:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_16]]);
// DEFAULT-NEXT:                         let %[[VALUE63:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE62]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_16]], read<i32>(%[[VALUE63]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_16]]))))), xor<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_16]]))))), const<i32>(2)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], and<vector<i16, 8>, elementwise=true>(read<vector<i16, 8>>(%[[VALUE_v0]]), vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         do %[[VALUE64:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_17:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE65:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_17]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_17]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE66:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_17]]);
// DEFAULT-NEXT:                         let %[[VALUE67:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE66]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_17]], read<i32>(%[[VALUE67]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_17]]))))), and<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_17]]))))), const<i32>(2)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_v1]], or<vector<i16, 8>, elementwise=true>(read<vector<i16, 8>>(%[[VALUE_v0]]), vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         do %[[VALUE68:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_18:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE69:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_18]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_18]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE70:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_18]]);
// DEFAULT-NEXT:                         let %[[VALUE71:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE70]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_18]], read<i32>(%[[VALUE71]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v1]])), read<i32>(%[[VALUE___i_18]]))))), or<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_v0]])), read<i32>(%[[VALUE___i_18]]))))), const<i32>(2)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f1]], add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(vector_splat<vector<f32, 4>, reason=usual_arith>(float_narrow<f32, reason=usual_arith, rounding=nearest_even, exceptions=observable>(const<f64>(2.0))), read<vector<f32, 4>>(%[[VALUE_f0]])));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f2]], add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(compound_literal %[[VALUE72:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)))), read<vector<f32, 4>>(%[[VALUE_f0]])));
// DEFAULT-NEXT:         do %[[VALUE73:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_19:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE74:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_19]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_19]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE75:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_19]]);
// DEFAULT-NEXT:                         let %[[VALUE76:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE75]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_19]], read<i32>(%[[VALUE76]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f1]])), read<i32>(%[[VALUE___i_19]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f2]])), read<i32>(%[[VALUE___i_19]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f1]], sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(vector_splat<vector<f32, 4>, reason=usual_arith>(float_narrow<f32, reason=usual_arith, rounding=nearest_even, exceptions=observable>(const<f64>(2.0))), read<vector<f32, 4>>(%[[VALUE_f0]])));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f2]], sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(compound_literal %[[VALUE77:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)))), read<vector<f32, 4>>(%[[VALUE_f0]])));
// DEFAULT-NEXT:         do %[[VALUE78:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_20:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE79:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_20]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_20]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE80:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_20]]);
// DEFAULT-NEXT:                         let %[[VALUE81:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE80]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_20]], read<i32>(%[[VALUE81]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f1]])), read<i32>(%[[VALUE___i_20]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f2]])), read<i32>(%[[VALUE___i_20]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f1]], mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(vector_splat<vector<f32, 4>, reason=usual_arith>(float_narrow<f32, reason=usual_arith, rounding=nearest_even, exceptions=observable>(const<f64>(2.0))), read<vector<f32, 4>>(%[[VALUE_f0]])));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f2]], mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(compound_literal %[[VALUE82:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)))), read<vector<f32, 4>>(%[[VALUE_f0]])));
// DEFAULT-NEXT:         do %[[VALUE83:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_21:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE84:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_21]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_21]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE85:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_21]]);
// DEFAULT-NEXT:                         let %[[VALUE86:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE85]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_21]], read<i32>(%[[VALUE86]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f1]])), read<i32>(%[[VALUE___i_21]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f2]])), read<i32>(%[[VALUE___i_21]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f1]], div<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(vector_splat<vector<f32, 4>, reason=usual_arith>(float_narrow<f32, reason=usual_arith, rounding=nearest_even, exceptions=observable>(const<f64>(2.0))), read<vector<f32, 4>>(%[[VALUE_f0]])));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f2]], div<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(compound_literal %[[VALUE87:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)))), read<vector<f32, 4>>(%[[VALUE_f0]])));
// DEFAULT-NEXT:         do %[[VALUE88:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_22:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE89:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_22]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_22]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE90:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_22]]);
// DEFAULT-NEXT:                         let %[[VALUE91:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE90]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_22]], read<i32>(%[[VALUE91]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f1]])), read<i32>(%[[VALUE___i_22]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f2]])), read<i32>(%[[VALUE___i_22]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f1]], add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(%[[VALUE_f0]]), vector_splat<vector<f32, 4>, reason=usual_arith>(float_narrow<f32, reason=usual_arith, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f2]], add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(%[[VALUE_f0]]), read<vector<f32, 4>>(compound_literal %[[VALUE92:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0))))));
// DEFAULT-NEXT:         do %[[VALUE93:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_23:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE94:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_23]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_23]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE95:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_23]]);
// DEFAULT-NEXT:                         let %[[VALUE96:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE95]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_23]], read<i32>(%[[VALUE96]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f1]])), read<i32>(%[[VALUE___i_23]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f2]])), read<i32>(%[[VALUE___i_23]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f1]], sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(%[[VALUE_f0]]), vector_splat<vector<f32, 4>, reason=usual_arith>(float_narrow<f32, reason=usual_arith, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f2]], sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(%[[VALUE_f0]]), read<vector<f32, 4>>(compound_literal %[[VALUE97:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0))))));
// DEFAULT-NEXT:         do %[[VALUE98:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_24:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE99:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_24]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_24]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE100:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_24]]);
// DEFAULT-NEXT:                         let %[[VALUE101:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE100]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_24]], read<i32>(%[[VALUE101]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f1]])), read<i32>(%[[VALUE___i_24]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f2]])), read<i32>(%[[VALUE___i_24]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f1]], mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(%[[VALUE_f0]]), vector_splat<vector<f32, 4>, reason=usual_arith>(float_narrow<f32, reason=usual_arith, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f2]], mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(%[[VALUE_f0]]), read<vector<f32, 4>>(compound_literal %[[VALUE102:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0))))));
// DEFAULT-NEXT:         do %[[VALUE103:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_25:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE104:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_25]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_25]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE105:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_25]]);
// DEFAULT-NEXT:                         let %[[VALUE106:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE105]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_25]], read<i32>(%[[VALUE106]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f1]])), read<i32>(%[[VALUE___i_25]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f2]])), read<i32>(%[[VALUE___i_25]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f1]], div<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(%[[VALUE_f0]]), vector_splat<vector<f32, 4>, reason=usual_arith>(float_narrow<f32, reason=usual_arith, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_f2]], div<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 4>>(%[[VALUE_f0]]), read<vector<f32, 4>>(compound_literal %[[VALUE107:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(2.0))))));
// DEFAULT-NEXT:         do %[[VALUE108:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_26:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE109:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_26]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_26]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE110:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_26]]);
// DEFAULT-NEXT:                         let %[[VALUE111:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE110]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_26]], read<i32>(%[[VALUE111]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f1]])), read<i32>(%[[VALUE___i_26]])))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%[[VALUE_f2]])), read<i32>(%[[VALUE___i_26]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d1]], add<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0)), read<vector<f64, 2>>(%[[VALUE_d0]])));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d2]], add<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(compound_literal %[[VALUE112:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0))), read<vector<f64, 2>>(%[[VALUE_d0]])));
// DEFAULT-NEXT:         do %[[VALUE113:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_27:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE114:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_27]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_27]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE115:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_27]]);
// DEFAULT-NEXT:                         let %[[VALUE116:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE115]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_27]], read<i32>(%[[VALUE116]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d1]])), read<i32>(%[[VALUE___i_27]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d2]])), read<i32>(%[[VALUE___i_27]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d1]], sub<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0)), read<vector<f64, 2>>(%[[VALUE_d0]])));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d2]], sub<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(compound_literal %[[VALUE117:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0))), read<vector<f64, 2>>(%[[VALUE_d0]])));
// DEFAULT-NEXT:         do %[[VALUE118:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_28:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE119:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_28]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_28]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE120:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_28]]);
// DEFAULT-NEXT:                         let %[[VALUE121:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE120]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_28]], read<i32>(%[[VALUE121]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d1]])), read<i32>(%[[VALUE___i_28]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d2]])), read<i32>(%[[VALUE___i_28]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d1]], mul<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0)), read<vector<f64, 2>>(%[[VALUE_d0]])));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d2]], mul<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(compound_literal %[[VALUE122:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0))), read<vector<f64, 2>>(%[[VALUE_d0]])));
// DEFAULT-NEXT:         do %[[VALUE123:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_29:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE124:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_29]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_29]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE125:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_29]]);
// DEFAULT-NEXT:                         let %[[VALUE126:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE125]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_29]], read<i32>(%[[VALUE126]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d1]])), read<i32>(%[[VALUE___i_29]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d2]])), read<i32>(%[[VALUE___i_29]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d1]], div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0)), read<vector<f64, 2>>(%[[VALUE_d0]])));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d2]], div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(compound_literal %[[VALUE127:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0))), read<vector<f64, 2>>(%[[VALUE_d0]])));
// DEFAULT-NEXT:         do %[[VALUE128:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_30:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE129:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_30]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_30]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE130:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_30]]);
// DEFAULT-NEXT:                         let %[[VALUE131:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE130]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_30]], read<i32>(%[[VALUE131]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d1]])), read<i32>(%[[VALUE___i_30]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d2]])), read<i32>(%[[VALUE___i_30]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d1]], add<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(%[[VALUE_d0]]), vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0))));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d2]], add<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(%[[VALUE_d0]]), read<vector<f64, 2>>(compound_literal %[[VALUE132:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         do %[[VALUE133:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_31:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE134:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_31]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_31]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE135:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_31]]);
// DEFAULT-NEXT:                         let %[[VALUE136:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE135]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_31]], read<i32>(%[[VALUE136]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d1]])), read<i32>(%[[VALUE___i_31]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d2]])), read<i32>(%[[VALUE___i_31]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d1]], sub<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(%[[VALUE_d0]]), vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0))));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d2]], sub<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(%[[VALUE_d0]]), read<vector<f64, 2>>(compound_literal %[[VALUE137:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         do %[[VALUE138:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_32:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE139:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_32]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_32]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE140:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_32]]);
// DEFAULT-NEXT:                         let %[[VALUE141:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE140]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_32]], read<i32>(%[[VALUE141]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d1]])), read<i32>(%[[VALUE___i_32]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d2]])), read<i32>(%[[VALUE___i_32]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d1]], mul<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(%[[VALUE_d0]]), vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0))));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d2]], mul<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(%[[VALUE_d0]]), read<vector<f64, 2>>(compound_literal %[[VALUE142:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         do %[[VALUE143:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_33:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE144:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_33]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_33]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE145:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_33]]);
// DEFAULT-NEXT:                         let %[[VALUE146:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE145]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_33]], read<i32>(%[[VALUE146]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d1]])), read<i32>(%[[VALUE___i_33]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d2]])), read<i32>(%[[VALUE___i_33]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d1]], div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(%[[VALUE_d0]]), vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0))));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%[[VALUE_d2]], div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f64, 2>>(%[[VALUE_d0]]), read<vector<f64, 2>>(compound_literal %[[VALUE147:[0-9]+]] [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         do %[[VALUE148:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_34:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE149:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_34]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_34]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE150:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_34]]);
// DEFAULT-NEXT:                         let %[[VALUE151:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE150]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_34]], read<i32>(%[[VALUE151]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d1]])), read<i32>(%[[VALUE___i_34]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%[[VALUE_d2]])), read<i32>(%[[VALUE___i_34]])))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
