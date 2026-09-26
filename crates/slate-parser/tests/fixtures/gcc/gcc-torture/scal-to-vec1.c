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
// DEFAULT-NEXT:     global %0 one: volatile i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     fn %1 @main(%2 argc: i32, %3 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 v0: vector<i16, 8> [storage=automatic] = aggregate<vector<i16, 8>, zero_fill=false>(index0 = truncate<i16, reason=assign, fits=unknown>(read<i32, volatile>(%0)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(1)), index2 = truncate<i16, reason=assign, fits=always>(const<i32>(2)), index3 = truncate<i16, reason=assign, fits=always>(const<i32>(3)), index4 = truncate<i16, reason=assign, fits=always>(const<i32>(4)), index5 = truncate<i16, reason=assign, fits=always>(const<i32>(5)), index6 = truncate<i16, reason=assign, fits=always>(const<i32>(6)), index7 = truncate<i16, reason=assign, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         let %5 v1: vector<i16, 8> [storage=automatic];
// DEFAULT-NEXT:         let %6 f0: vector<f32, 4> [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(3.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(4.0)));
// DEFAULT-NEXT:         let %7 f1: vector<f32, 4> [storage=automatic];
// DEFAULT-NEXT:         let %8 f2: vector<f32, 4> [storage=automatic];
// DEFAULT-NEXT:         let %9 d0: vector<f64, 2> [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(1.0), index1 = const<f64>(2.0));
// DEFAULT-NEXT:         let %10 d1: vector<f64, 2> [storage=automatic];
// DEFAULT-NEXT:         let %11 d2: vector<f64, 2> [storage=automatic];
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, add<vector<i16, 8>, elementwise=true, overflow=wrap>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%4)));
// DEFAULT-NEXT:         do %46
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %12 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %47
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%12, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%12), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %130: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                         let %131: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%130), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%12, read<i32>(%131));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%12))))), add<i32, overflow=ub>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%12)))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, sub<vector<i16, 8>, elementwise=true, overflow=wrap>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%4)));
// DEFAULT-NEXT:         do %48
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %13 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %49
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%13, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%13), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %132: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                         let %133: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%132), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%13, read<i32>(%133));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%13))))), sub<i32, overflow=ub>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%13)))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, mul<vector<i16, 8>, elementwise=true, overflow=wrap>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%4)));
// DEFAULT-NEXT:         do %50
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %14 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %51
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%14, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%14), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %134: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                         let %135: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%134), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%14, read<i32>(%135));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%14))))), mul<i32, overflow=ub>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%14)))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, div<vector<i16, 8>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%4)));
// DEFAULT-NEXT:         do %52
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %15 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %53
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%15, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%15), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %136: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                         let %137: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%136), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%15, read<i32>(%137));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%15))))), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%15)))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, rem<vector<i16, 8>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%4)));
// DEFAULT-NEXT:         do %54
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %16 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %55
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%16), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %138: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                         let %139: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%138), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%16, read<i32>(%139));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%16))))), rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%16)))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, xor<vector<i16, 8>, elementwise=true>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%4)));
// DEFAULT-NEXT:         do %56
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %17 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %57
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%17, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%17), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %140: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                         let %141: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%140), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%17, read<i32>(%141));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%17))))), xor<i32>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%17)))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, and<vector<i16, 8>, elementwise=true>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%4)));
// DEFAULT-NEXT:         do %58
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %18 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %59
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%18, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%18), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %142: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:                         let %143: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%142), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%18, read<i32>(%143));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%18))))), and<i32>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%18)))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, or<vector<i16, 8>, elementwise=true>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%4)));
// DEFAULT-NEXT:         do %60
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %19 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %61
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%19), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %144: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                         let %145: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%144), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%19, read<i32>(%145));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%19))))), or<i32>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%19)))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, shl<vector<i16, 8>, elementwise=true, overflow=wrap, amount_out_of_range=ub, negative_left=ub>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%4)));
// DEFAULT-NEXT:         do %62
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %20 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %63
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%20, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%20), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %146: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:                         let %147: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%146), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%20, read<i32>(%147));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%20))))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%20)))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, shr<vector<i16, 8>, elementwise=true, amount_out_of_range=ub, fill=sign_extend>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2))), read<vector<i16, 8>>(%4)));
// DEFAULT-NEXT:         do %64
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %21 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %65
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%21, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%21), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %148: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                         let %149: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%148), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%21, read<i32>(%149));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%21))))), shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%21)))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, add<vector<i16, 8>, elementwise=true, overflow=wrap>(read<vector<i16, 8>>(%4), vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         do %66
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %22 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %67
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%22, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%22), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %150: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:                         let %151: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%150), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%22, read<i32>(%151));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%22))))), add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%22))))), const<i32>(2)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, sub<vector<i16, 8>, elementwise=true, overflow=wrap>(read<vector<i16, 8>>(%4), vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         do %68
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %23 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %69
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%23, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%23), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %152: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:                         let %153: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%152), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%23, read<i32>(%153));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%23))))), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%23))))), const<i32>(2)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, mul<vector<i16, 8>, elementwise=true, overflow=wrap>(read<vector<i16, 8>>(%4), vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         do %70
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %24 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %71
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%24, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%24), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %154: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:                         let %155: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%154), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%24, read<i32>(%155));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%24))))), mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%24))))), const<i32>(2)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, div<vector<i16, 8>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(read<vector<i16, 8>>(%4), vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         do %72
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %25 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %73
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%25, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%25), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %156: i32 [synthetic] = read<i32>(%25);
// DEFAULT-NEXT:                         let %157: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%156), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%25, read<i32>(%157));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%25))))), div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%25))))), const<i32>(2)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, rem<vector<i16, 8>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(read<vector<i16, 8>>(%4), vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         do %74
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %26 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %75
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%26, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%26), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %158: i32 [synthetic] = read<i32>(%26);
// DEFAULT-NEXT:                         let %159: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%158), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%26, read<i32>(%159));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%26))))), rem<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%26))))), const<i32>(2)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, xor<vector<i16, 8>, elementwise=true>(read<vector<i16, 8>>(%4), vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         do %76
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %27 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %77
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%27, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%27), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %160: i32 [synthetic] = read<i32>(%27);
// DEFAULT-NEXT:                         let %161: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%160), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%27, read<i32>(%161));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%27))))), xor<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%27))))), const<i32>(2)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, and<vector<i16, 8>, elementwise=true>(read<vector<i16, 8>>(%4), vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         do %78
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %28 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %79
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%28, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%28), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %162: i32 [synthetic] = read<i32>(%28);
// DEFAULT-NEXT:                         let %163: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%162), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%28, read<i32>(%163));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%28))))), and<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%28))))), const<i32>(2)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%5, or<vector<i16, 8>, elementwise=true>(read<vector<i16, 8>>(%4), vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         do %80
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %29 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %81
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%29, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%29), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %164: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:                         let %165: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%164), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%29, read<i32>(%165));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%5)), read<i32>(%29))))), or<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%4)), read<i32>(%29))))), const<i32>(2)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%7, add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(vector_splat<vector<f32, 4>, reason=usual_arith>(float_narrow<f32, reason=usual_arith, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0))), read<vector<f32, 4>>(%6)));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%8, add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(compound_literal %82 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)))), read<vector<f32, 4>>(%6)));
// DEFAULT-NEXT:         do %83
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %30 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %84
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%30, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%30), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %166: i32 [synthetic] = read<i32>(%30);
// DEFAULT-NEXT:                         let %167: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%166), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%30, read<i32>(%167));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%7)), read<i32>(%30)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%8)), read<i32>(%30)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%7, sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(vector_splat<vector<f32, 4>, reason=usual_arith>(float_narrow<f32, reason=usual_arith, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0))), read<vector<f32, 4>>(%6)));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%8, sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(compound_literal %85 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)))), read<vector<f32, 4>>(%6)));
// DEFAULT-NEXT:         do %86
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %31 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %87
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%31, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%31), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %168: i32 [synthetic] = read<i32>(%31);
// DEFAULT-NEXT:                         let %169: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%168), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%31, read<i32>(%169));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%7)), read<i32>(%31)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%8)), read<i32>(%31)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%7, mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(vector_splat<vector<f32, 4>, reason=usual_arith>(float_narrow<f32, reason=usual_arith, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0))), read<vector<f32, 4>>(%6)));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%8, mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(compound_literal %88 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)))), read<vector<f32, 4>>(%6)));
// DEFAULT-NEXT:         do %89
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %32 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %90
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%32, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%32), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %170: i32 [synthetic] = read<i32>(%32);
// DEFAULT-NEXT:                         let %171: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%170), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%32, read<i32>(%171));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%7)), read<i32>(%32)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%8)), read<i32>(%32)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%7, div<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(vector_splat<vector<f32, 4>, reason=usual_arith>(float_narrow<f32, reason=usual_arith, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0))), read<vector<f32, 4>>(%6)));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%8, div<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(compound_literal %91 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)))), read<vector<f32, 4>>(%6)));
// DEFAULT-NEXT:         do %92
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %33 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %93
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%33, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%33), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %172: i32 [synthetic] = read<i32>(%33);
// DEFAULT-NEXT:                         let %173: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%172), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%33, read<i32>(%173));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%7)), read<i32>(%33)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%8)), read<i32>(%33)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%7, add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%6), vector_splat<vector<f32, 4>, reason=usual_arith>(float_narrow<f32, reason=usual_arith, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%8, add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%6), read<vector<f32, 4>>(compound_literal %94 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0))))));
// DEFAULT-NEXT:         do %95
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %34 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %96
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%34, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%34), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %174: i32 [synthetic] = read<i32>(%34);
// DEFAULT-NEXT:                         let %175: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%174), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%34, read<i32>(%175));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%7)), read<i32>(%34)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%8)), read<i32>(%34)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%7, sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%6), vector_splat<vector<f32, 4>, reason=usual_arith>(float_narrow<f32, reason=usual_arith, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%8, sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%6), read<vector<f32, 4>>(compound_literal %97 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0))))));
// DEFAULT-NEXT:         do %98
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %35 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %99
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%35, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%35), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %176: i32 [synthetic] = read<i32>(%35);
// DEFAULT-NEXT:                         let %177: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%176), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%35, read<i32>(%177));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%7)), read<i32>(%35)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%8)), read<i32>(%35)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%7, mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%6), vector_splat<vector<f32, 4>, reason=usual_arith>(float_narrow<f32, reason=usual_arith, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%8, mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%6), read<vector<f32, 4>>(compound_literal %100 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0))))));
// DEFAULT-NEXT:         do %101
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %36 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %102
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%36, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%36), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %178: i32 [synthetic] = read<i32>(%36);
// DEFAULT-NEXT:                         let %179: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%178), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%36, read<i32>(%179));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%7)), read<i32>(%36)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%8)), read<i32>(%36)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%7, div<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%6), vector_splat<vector<f32, 4>, reason=usual_arith>(float_narrow<f32, reason=usual_arith, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%8, div<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%6), read<vector<f32, 4>>(compound_literal %103 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0))))));
// DEFAULT-NEXT:         do %104
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %37 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %105
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%37, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%37), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %180: i32 [synthetic] = read<i32>(%37);
// DEFAULT-NEXT:                         let %181: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%180), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%37, read<i32>(%181));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%7)), read<i32>(%37)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(pointer_cast<ptr<f32>, reason=explicit>(addr_of<ptr<vector<f32, 4>>>(%8)), read<i32>(%37)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%10, add<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0)), read<vector<f64, 2>>(%9)));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%11, add<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(compound_literal %106 [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0))), read<vector<f64, 2>>(%9)));
// DEFAULT-NEXT:         do %107
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %38 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %108
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%38, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%38), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %182: i32 [synthetic] = read<i32>(%38);
// DEFAULT-NEXT:                         let %183: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%182), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%38, read<i32>(%183));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%10)), read<i32>(%38)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%11)), read<i32>(%38)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%10, sub<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0)), read<vector<f64, 2>>(%9)));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%11, sub<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(compound_literal %109 [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0))), read<vector<f64, 2>>(%9)));
// DEFAULT-NEXT:         do %110
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %39 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %111
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%39, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%39), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %184: i32 [synthetic] = read<i32>(%39);
// DEFAULT-NEXT:                         let %185: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%184), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%39, read<i32>(%185));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%10)), read<i32>(%39)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%11)), read<i32>(%39)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%10, mul<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0)), read<vector<f64, 2>>(%9)));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%11, mul<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(compound_literal %112 [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0))), read<vector<f64, 2>>(%9)));
// DEFAULT-NEXT:         do %113
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %40 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %114
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%40, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%40), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %186: i32 [synthetic] = read<i32>(%40);
// DEFAULT-NEXT:                         let %187: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%186), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%40, read<i32>(%187));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%10)), read<i32>(%40)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%11)), read<i32>(%40)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%10, div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0)), read<vector<f64, 2>>(%9)));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%11, div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(compound_literal %115 [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0))), read<vector<f64, 2>>(%9)));
// DEFAULT-NEXT:         do %116
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %41 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %117
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%41, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%41), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %188: i32 [synthetic] = read<i32>(%41);
// DEFAULT-NEXT:                         let %189: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%188), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%41, read<i32>(%189));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%10)), read<i32>(%41)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%11)), read<i32>(%41)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%10, add<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%9), vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0))));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%11, add<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%9), read<vector<f64, 2>>(compound_literal %118 [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         do %119
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %42 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %120
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%42, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%42), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %190: i32 [synthetic] = read<i32>(%42);
// DEFAULT-NEXT:                         let %191: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%190), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%42, read<i32>(%191));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%10)), read<i32>(%42)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%11)), read<i32>(%42)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%10, sub<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%9), vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0))));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%11, sub<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%9), read<vector<f64, 2>>(compound_literal %121 [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         do %122
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %43 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %123
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%43, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%43), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %192: i32 [synthetic] = read<i32>(%43);
// DEFAULT-NEXT:                         let %193: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%192), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%43, read<i32>(%193));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%10)), read<i32>(%43)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%11)), read<i32>(%43)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%10, mul<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%9), vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0))));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%11, mul<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%9), read<vector<f64, 2>>(compound_literal %124 [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         do %125
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %44 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %126
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%44, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%44), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %194: i32 [synthetic] = read<i32>(%44);
// DEFAULT-NEXT:                         let %195: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%194), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%44, read<i32>(%195));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%10)), read<i32>(%44)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%11)), read<i32>(%44)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%10, div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%9), vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0))));
// DEFAULT-NEXT:         write<vector<f64, 2>>(%11, div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%9), read<vector<f64, 2>>(compound_literal %127 [storage=automatic] = aggregate<vector<f64, 2>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(2.0)))));
// DEFAULT-NEXT:         do %128
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %45 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %129
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%45, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%45), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %196: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:                         let %197: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%196), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%45, read<i32>(%197));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%10)), read<i32>(%45)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(pointer_cast<ptr<f64>, reason=explicit>(addr_of<ptr<vector<f64, 2>>>(%11)), read<i32>(%45)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
