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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a1: i64;
// DEFAULT-NEXT:         field1 a2: ptr<f64>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:         field0 b1: ptr<void>;
// DEFAULT-NEXT:         field1 b2: f64;
// DEFAULT-NEXT:         field2 b3: f64;
// DEFAULT-NEXT:         field3 b4: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = struct {
// DEFAULT-NEXT:         field0 d1: i32;
// DEFAULT-NEXT:         field1 d2: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_C:[0-9]+]] C = struct {
// DEFAULT-NEXT:         field0 c1: ptr<@type[[TYPE_A]]>;
// DEFAULT-NEXT:         field1 c2: ptr<void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([115, 111, 109, 101, 116, 104, 105, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE_A]]>, %[[VALUE1:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2(%[[VALUE2:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE3:[0-9]+]] <unnamed>: ptr<const i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn3:[0-9]+]] @fn3(%[[VALUE4:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn4:[0-9]+]] @fn4(%[[VALUE5:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn5:[0-9]+]] @fn5(%[[VALUE6:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE7:[0-9]+]] <unnamed>: f64, %[[VALUE8:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_B]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: ptr<@type[[TYPE_C]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_C]]>, reason=assign>(read<ptr<void>>(field0(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x]])))));
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: ptr<@type[[TYPE_A]]> [storage=automatic] = read<ptr<@type[[TYPE_A]]>>(field0(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE_e]]))));
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: ptr<f64> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: f64 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(%[[VALUE_g]], call<i64, signature=fn(ptr<@type[[TYPE_A]]>, f64) -> i64>(%[[VALUE_fn1]], read<ptr<@type[[TYPE_A]]>>(%[[VALUE_f]]), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.5), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(field1(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x]])))), read<f64>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x]]))))))));
// DEFAULT-NEXT:         call<i64, signature=fn(ptr<@type[[TYPE_A]]>, f64) -> i64>(%[[VALUE_fn1]], read<ptr<@type[[TYPE_A]]>>(%[[VALUE_f]]), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.5), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(field1(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x]])))), read<f64>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x]])))))));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_h]], add<i64, overflow=ub>(read<i64>(%[[VALUE_g]]), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_i]], read<i64>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_f]])))));
// DEFAULT-NEXT:         write<ptr<f64>>(%[[VALUE_j]], read<ptr<f64>>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_f]])))));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_k]], read<f64>(field1(field3(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x]]))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ptr<const i8>) -> void>(%[[VALUE_fn2]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])));
// DEFAULT-NEXT:         if le<i64>(read<i64>(%[[VALUE_g]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_l:[0-9]+]] l: f64 [storage=automatic] = sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_j]]), const<i32>(2)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_j]]), const<i32>(1)))));
// DEFAULT-NEXT:                 if logical_and<bool>(gt<f64, exceptions=ignore>(read<f64>(%[[VALUE_l]]), const<f64>(0.0)), le<f64, exceptions=ignore>(read<f64>(%[[VALUE_l]]), const<f64>(0.02)))
// DEFAULT-NEXT:                     let %[[VALUE9:[0-9]+]]: f64 [synthetic];
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(field0(field3(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x]]))))), const<i32>(1))
// DEFAULT-NEXT:                         write<f64>(%[[VALUE9]], conditional<f64>(lt<f64, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), read<f64>(%[[VALUE_l]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), read<f64>(%[[VALUE_l]]))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<f64>(%[[VALUE9]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_fn3]], conditional<f64>(lt<f64, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), read<f64>(%[[VALUE_l]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), read<f64>(%[[VALUE_l]])))));
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_k]], read<f64>(%[[VALUE9]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_m:[0-9]+]] m: f64 [storage=automatic] = sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_j]]), read<i64>(%[[VALUE_h]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_j]]), read<i64>(%[[VALUE_g]])))));
// DEFAULT-NEXT:                 let %[[VALUE_n:[0-9]+]] n: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:                 let %[[VALUE_l_2:[0-9]+]] l: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:                 if gt<i64>(read<i64>(%[[VALUE_g]]), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_n]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_j]]), read<i64>(%[[VALUE_g]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_j]]), sub<i64, overflow=ub>(read<i64>(%[[VALUE_g]]), widen<i64, reason=usual_arith>(const<i32>(1))))))));
// DEFAULT-NEXT:                 if lt<i64>(read<i64>(%[[VALUE_h]]), read<i64>(%[[VALUE_i]]))
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_l_2]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_j]]), add<i64, overflow=ub>(read<i64>(%[[VALUE_h]]), widen<i64, reason=usual_arith>(const<i32>(1)))))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_j]]), read<i64>(%[[VALUE_h]]))))));
// DEFAULT-NEXT:                 if gt<f64, exceptions=ignore>(read<f64>(%[[VALUE_n]]), const<f64>(0.02))
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_n]], int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)));
// DEFAULT-NEXT:                 if gt<f64, exceptions=ignore>(read<f64>(%[[VALUE_m]]), const<f64>(0.02))
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_m]], int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)));
// DEFAULT-NEXT:                 if gt<f64, exceptions=ignore>(read<f64>(%[[VALUE_l_2]]), const<f64>(0.02))
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_l_2]], int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)));
// DEFAULT-NEXT:                 if lt<f64, exceptions=ignore>(read<f64>(%[[VALUE_m]]), read<f64>(%[[VALUE_n]]))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_o:[0-9]+]] o: f64 [storage=automatic] = read<f64>(%[[VALUE_m]]);
// DEFAULT-NEXT:                         write<f64>(%[[VALUE_m]], read<f64>(%[[VALUE_n]]));
// DEFAULT-NEXT:                         write<f64>(%[[VALUE_n]], read<f64>(%[[VALUE_o]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 if lt<f64, exceptions=ignore>(read<f64>(%[[VALUE_l_2]]), read<f64>(%[[VALUE_n]]))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_o_2:[0-9]+]] o: f64 [storage=automatic] = read<f64>(%[[VALUE_l_2]]);
// DEFAULT-NEXT:                         write<f64>(%[[VALUE_l_2]], read<f64>(%[[VALUE_n]]));
// DEFAULT-NEXT:                         write<f64>(%[[VALUE_n]], read<f64>(%[[VALUE_o_2]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 if lt<f64, exceptions=ignore>(read<f64>(%[[VALUE_l_2]]), read<f64>(%[[VALUE_m]]))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_o_3:[0-9]+]] o: f64 [storage=automatic] = read<f64>(%[[VALUE_l_2]]);
// DEFAULT-NEXT:                         write<f64>(%[[VALUE_l_2]], read<f64>(%[[VALUE_m]]));
// DEFAULT-NEXT:                         write<f64>(%[[VALUE_m]], read<f64>(%[[VALUE_o_3]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_n]]), const<f64>(0.0))
// DEFAULT-NEXT:                     let %[[VALUE10:[0-9]+]]: f64 [synthetic];
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(field0(field3(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x]]))))), const<i32>(1))
// DEFAULT-NEXT:                         write<f64>(%[[VALUE10]], conditional<f64>(lt<f64, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%[[VALUE_m]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%[[VALUE_m]]))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<f64>(%[[VALUE10]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_fn3]], conditional<f64>(lt<f64, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%[[VALUE_m]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%[[VALUE_m]])))));
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_k]], read<f64>(%[[VALUE10]]));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_m]]), const<f64>(0.0))
// DEFAULT-NEXT:                         let %[[VALUE11:[0-9]+]]: f64 [synthetic];
// DEFAULT-NEXT:                         if eq<i32>(read<i32>(field0(field3(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x]]))))), const<i32>(1))
// DEFAULT-NEXT:                             write<f64>(%[[VALUE11]], conditional<f64>(lt<f64, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_m]]), read<f64>(%[[VALUE_l_2]]))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_m]]), read<f64>(%[[VALUE_l_2]])))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64>(%[[VALUE11]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_fn3]], conditional<f64>(lt<f64, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_m]]), read<f64>(%[[VALUE_l_2]]))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_m]]), read<f64>(%[[VALUE_l_2]]))))));
// DEFAULT-NEXT:                         write<f64>(%[[VALUE_k]], read<f64>(%[[VALUE11]]));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_l_2]]), const<f64>(0.0))
// DEFAULT-NEXT:                             let %[[VALUE12:[0-9]+]]: f64 [synthetic];
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(field0(field3(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x]]))))), const<i32>(1))
// DEFAULT-NEXT:                                 write<f64>(%[[VALUE12]], conditional<f64>(lt<f64, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%[[VALUE_l_2]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%[[VALUE_l_2]]))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<f64>(%[[VALUE12]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_fn3]], conditional<f64>(lt<f64, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%[[VALUE_l_2]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25)), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%[[VALUE_l_2]])))));
// DEFAULT-NEXT:                             write<f64>(%[[VALUE_k]], read<f64>(%[[VALUE12]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: f64 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(field0(field3(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x]]))))), const<i32>(1))
// DEFAULT-NEXT:             write<f64>(%[[VALUE13]], read<f64>(%[[VALUE_k]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<f64>(%[[VALUE13]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_fn4]], read<f64>(%[[VALUE_k]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, f64, f64) -> i32>(%[[VALUE_fn5]], read<ptr<void>>(field1(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE_e]])))), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.5), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(field1(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x]])))), read<f64>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_x]])))))), read<f64>(%[[VALUE13]]));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
