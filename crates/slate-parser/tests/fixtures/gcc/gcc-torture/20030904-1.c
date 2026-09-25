// SLATE-FILECHECK-DEFINES DEFAULT

struct A
{
  long a1;
  double *a2;
};

struct B
{
  void *b1;
  double b2, b3;
  struct
  {
    int d1;
    double d2;
  } b4;
};

struct C
{
  struct A *c1;
  void *c2;
};

long fn1 (struct A *, double);
void fn2 (void *, const char *);
double fn3 (double);
double fn4 (double);
int fn5 (void *, double, double);

int
foo (struct B *x)
{
  struct C *e = x->b1;
  struct A *f = e->c1;
  long g, h, i;
  double *j, k;
  g = fn1 (f, 0.5 * (x->b2 + x->b3)), h = g + 1, i = f->a1;
  j = f->a2, k = x->b4.d2;
  fn2 (x, "something");
  if (g <= 0)
    {
      double l = j[2] - j[1];
      if (l > 0.0 && l <= 0.02)
        k = (x->b4.d1 == 1
             ? ((1.0 / l) < 25 ? 25 : (1.0 / l))
             : fn3 ((1.0 / l) < 25 ? 25 : (1.0 / l)));
    }
  else
    {
      double m = j[h] - j[g], n = 0.0, l = 0.0;
      if (g > 1)
        n = j[g] - j[g - 1];
      if (h < i)
        l = j[h + 1] - j[h];
      if (n > 0.02)
        n = 0;
      if (m > 0.02)
        m = 0;
      if (l > 0.02)
        l = 0;
      if (m < n)
        {
          double o = m;
          m = n;
          n = o;
        }
      if (l < n)
        {
          double o = l;
          l = n;
          n = o;
        }
      if (l < m)
        {
          double o = l;
          l = m;
          m = o;
        }
      if (n != 0.0)
        k = (x->b4.d1 == 1
             ? ((1 / m) < 25 ? 25 : (1 / m))
             : fn3 ((1 / m) < 25 ? 25 : (1 / m)));
      else if (m != 0.0)
        k = (x->b4.d1 == 1
             ? ((2 / (m + l)) < 25 ? 25 : (2 / (m + l)))
             : fn3 ((2 / (m + l)) < 25 ? 25 : (2 / (m + l))));
      else if (l != 0.0)
        k = (x->b4.d1 == 1
             ? ((1 / l) < 25 ? 25 : (1 / l))
             : fn3 ((1 / l) < 25 ? 25 : (1 / l)));
    }
  fn5 (e->c2, 0.5 * (x->b2 + x->b3), (x->b4.d1 == 1 ? k : fn4 (k)));
  return 1;
}

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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 a1: i64;
// DEFAULT-NEXT:         field1 a2: ptr<f64>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 B = struct {
// DEFAULT-NEXT:         field0 b1: ptr<void>;
// DEFAULT-NEXT:         field1 b2: f64;
// DEFAULT-NEXT:         field2 b3: f64;
// DEFAULT-NEXT:         field3 b4: @type2;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 d1: i32;
// DEFAULT-NEXT:         field1 d2: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type3 C = struct {
// DEFAULT-NEXT:         field0 c1: ptr<@type0>;
// DEFAULT-NEXT:         field1 c2: ptr<void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([115, 111, 109, 101, 116, 104, 105, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %4 @fn1(%25 <unnamed>: ptr<@type0>, %26 <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %5 @fn2(%27 <unnamed>: ptr<void>, %28 <unnamed>: ptr<const i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @fn3(%29 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %7 @fn4(%30 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %8 @fn5(%31 <unnamed>: ptr<void>, %32 <unnamed>: f64, %33 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @foo(%10 x: ptr<@type1>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 e: ptr<@type3> [storage=automatic] = pointer_cast<ptr<@type3>, reason=assign>(read<ptr<void>>(field0(deref(read<ptr<@type1>>(%10)))));
// DEFAULT-NEXT:         let %12 f: ptr<@type0> [storage=automatic] = read<ptr<@type0>>(field0(deref(read<ptr<@type3>>(%11))));
// DEFAULT-NEXT:         let %13 g: i64 [storage=automatic];
// DEFAULT-NEXT:         let %14 h: i64 [storage=automatic];
// DEFAULT-NEXT:         let %15 i: i64 [storage=automatic];
// DEFAULT-NEXT:         let %16 j: ptr<f64> [storage=automatic];
// DEFAULT-NEXT:         let %17 k: f64 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(%13, call<i64, signature=fn(ptr<@type0>, f64) -> i64>(%4, read<ptr<@type0>>(%12), mul<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(0.5), add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(field1(deref(read<ptr<@type1>>(%10)))), read<f64>(field2(deref(read<ptr<@type1>>(%10))))))));
// DEFAULT-NEXT:         call<i64, signature=fn(ptr<@type0>, f64) -> i64>(%4, read<ptr<@type0>>(%12), mul<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(0.5), add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(field1(deref(read<ptr<@type1>>(%10)))), read<f64>(field2(deref(read<ptr<@type1>>(%10)))))));
// DEFAULT-NEXT:         write<i64>(%14, add<i64, overflow=ub>(read<i64>(%13), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<i64>(%15, read<i64>(field0(deref(read<ptr<@type0>>(%12)))));
// DEFAULT-NEXT:         write<ptr<f64>>(%16, read<ptr<f64>>(field1(deref(read<ptr<@type0>>(%12)))));
// DEFAULT-NEXT:         write<f64>(%17, read<f64>(field1(field3(deref(read<ptr<@type1>>(%10))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ptr<const i8>) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%10)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%34)));
// DEFAULT-NEXT:         if le<i64>(read<i64>(%13), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %18 l: f64 [storage=automatic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%16), const<i32>(2)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%16), const<i32>(1)))));
// DEFAULT-NEXT:                 if logical_and<bool>(gt<f64, exceptions=ignore>(read<f64>(%18), const<f64>(0.0)), le<f64, exceptions=ignore>(read<f64>(%18), const<f64>(0.02)))
// DEFAULT-NEXT:                     let %35: f64 [synthetic];
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(field0(field3(deref(read<ptr<@type1>>(%10))))), const<i32>(1))
// DEFAULT-NEXT:                         write<f64>(%35, conditional<f64>(lt<f64, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), read<f64>(%18)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), div<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), read<f64>(%18))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<f64>(%35, call<f64, signature=fn(f64) -> f64>(%6, conditional<f64>(lt<f64, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), read<f64>(%18)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), div<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), read<f64>(%18)))));
// DEFAULT-NEXT:                     write<f64>(%17, read<f64>(%35));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %19 m: f64 [storage=automatic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%16), read<i64>(%14)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%16), read<i64>(%13)))));
// DEFAULT-NEXT:                 let %20 n: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:                 let %21 l: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:                 if gt<i64>(read<i64>(%13), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:                     write<f64>(%20, sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%16), read<i64>(%13)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%16), sub<i64, overflow=ub>(read<i64>(%13), widen<i64, reason=usual_arith>(const<i32>(1))))))));
// DEFAULT-NEXT:                 if lt<i64>(read<i64>(%14), read<i64>(%15))
// DEFAULT-NEXT:                     write<f64>(%21, sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%16), add<i64, overflow=ub>(read<i64>(%14), widen<i64, reason=usual_arith>(const<i32>(1)))))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%16), read<i64>(%14))))));
// DEFAULT-NEXT:                 if gt<f64, exceptions=ignore>(read<f64>(%20), const<f64>(0.02))
// DEFAULT-NEXT:                     write<f64>(%20, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)));
// DEFAULT-NEXT:                 if gt<f64, exceptions=ignore>(read<f64>(%19), const<f64>(0.02))
// DEFAULT-NEXT:                     write<f64>(%19, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)));
// DEFAULT-NEXT:                 if gt<f64, exceptions=ignore>(read<f64>(%21), const<f64>(0.02))
// DEFAULT-NEXT:                     write<f64>(%21, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)));
// DEFAULT-NEXT:                 if lt<f64, exceptions=ignore>(read<f64>(%19), read<f64>(%20))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %22 o: f64 [storage=automatic] = read<f64>(%19);
// DEFAULT-NEXT:                         write<f64>(%19, read<f64>(%20));
// DEFAULT-NEXT:                         write<f64>(%20, read<f64>(%22));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 if lt<f64, exceptions=ignore>(read<f64>(%21), read<f64>(%20))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %23 o: f64 [storage=automatic] = read<f64>(%21);
// DEFAULT-NEXT:                         write<f64>(%21, read<f64>(%20));
// DEFAULT-NEXT:                         write<f64>(%20, read<f64>(%23));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 if lt<f64, exceptions=ignore>(read<f64>(%21), read<f64>(%19))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %24 o: f64 [storage=automatic] = read<f64>(%21);
// DEFAULT-NEXT:                         write<f64>(%21, read<f64>(%19));
// DEFAULT-NEXT:                         write<f64>(%19, read<f64>(%24));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(read<f64>(%20), const<f64>(0.0))
// DEFAULT-NEXT:                     let %36: f64 [synthetic];
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(field0(field3(deref(read<ptr<@type1>>(%10))))), const<i32>(1))
// DEFAULT-NEXT:                         write<f64>(%36, conditional<f64>(lt<f64, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%19)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%19))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<f64>(%36, call<f64, signature=fn(f64) -> f64>(%6, conditional<f64>(lt<f64, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%19)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%19)))));
// DEFAULT-NEXT:                     write<f64>(%17, read<f64>(%36));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(%19), const<f64>(0.0))
// DEFAULT-NEXT:                         let %37: f64 [synthetic];
// DEFAULT-NEXT:                         if eq<i32>(read<i32>(field0(field3(deref(read<ptr<@type1>>(%10))))), const<i32>(1))
// DEFAULT-NEXT:                             write<f64>(%37, conditional<f64>(lt<f64, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%19), read<f64>(%21))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%19), read<f64>(%21)))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64>(%37, call<f64, signature=fn(f64) -> f64>(%6, conditional<f64>(lt<f64, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%19), read<f64>(%21))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%19), read<f64>(%21))))));
// DEFAULT-NEXT:                         write<f64>(%17, read<f64>(%37));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64>(%21), const<f64>(0.0))
// DEFAULT-NEXT:                             let %38: f64 [synthetic];
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(field0(field3(deref(read<ptr<@type1>>(%10))))), const<i32>(1))
// DEFAULT-NEXT:                                 write<f64>(%38, conditional<f64>(lt<f64, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%21)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%21))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f64>(%38, call<f64, signature=fn(f64) -> f64>(%6, conditional<f64>(lt<f64, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%21)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%21)))));
// DEFAULT-NEXT:                             write<f64>(%17, read<f64>(%38));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %39: f64 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(field0(field3(deref(read<ptr<@type1>>(%10))))), const<i32>(1))
// DEFAULT-NEXT:             write<f64>(%39, read<f64>(%17));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<f64>(%39, call<f64, signature=fn(f64) -> f64>(%7, read<f64>(%17)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, f64, f64) -> i32>(%8, read<ptr<void>>(field1(deref(read<ptr<@type3>>(%11)))), mul<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(0.5), add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(field1(deref(read<ptr<@type1>>(%10)))), read<f64>(field2(deref(read<ptr<@type1>>(%10)))))), read<f64>(%39));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
